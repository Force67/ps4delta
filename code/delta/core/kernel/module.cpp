
// Copyright (C) Force67 2019

#include <utl/file.h>
#include <utl/mem.h>

#include "runtime/vprx/vprx.h"
#include "runtime/code_lift.h"

#include "module.h"
#include "proc.h"

namespace krnl
{
	elfModule::elfModule(krnl::proc *process) :
		process(process)
	{
		/*-1 = no tls used*/
		info.handle = -1;
		info.tlsSlot = -1;

		/*set size from process config*/
		info.ripZoneSize = process->getEnv().ripZoneSize;
	}

	bool elfModule::fromFile(const std::string& path)
	{
		utl::File file(path);
		if (!file.IsOpen())
			return false;

		ELFHeader diskHeader{};
		file.Read(diskHeader);

		if (diskHeader.magic == ELF_MAGIC && diskHeader.machine == ELF_MACHINE_X86_64) {
			file.Seek(0, utl::seekMode::seek_set);
			
			data = std::make_unique<uint8_t[]>(file.GetSize());
			file.Read(data.get(), file.GetSize());

			// this is the main module
			if (diskHeader.type != ET_SCE_DYNAMIC) {
				info.name = "#main";
				LOG_INFO("Loading proc main module");
			}
			else {
				// attempt to gather module name from file path
				auto pos = path.find_last_of('\\');
				if (pos != std::string::npos) {
					info.name = path.substr(pos + 1, path.length() - pos);
					//LOG_INFO("Loading {}", name);
				}
				else {
					info.name = "#unk";
					LOG_WARNING("Unknown module name?");
				}
			}

			return fromMem(std::move(data));
		}

		return false;
	}

	bool elfModule::fromMem(std::unique_ptr<uint8_t[]> data)
	{
		/*TODO: figure out a way of getting rid of the back buffer*/
		this->data = std::move(data);

		elf = getOffset<ELFHeader>(0);
		segments = getOffset<ELFPgHeader>(elf->phoff);

		if (!mapImage()) {
			LOG_ERROR("elfModule: Failed to map image");
			return false;
		}

		doDynamics();
		setupTLS();

		if (!isSprx()) {
			auto* seg = getSegment(ElfSegType::PT_SCE_PROCPARAM);
			if (seg) {
				info.procParam = getAddress<uint8_t>(seg->vaddr);
				info.procParamSize = seg->filesz;
			}

			logDbgInfo();
		}

		if (!applyRelocations()) {
			LOG_ERROR("elfModule: Failed to relocate image");
			return false;
		}

		if (!resolveImports()) {
			LOG_ERROR("elfModule: Failed to resolve symbols");
			return false;
		}

		installEHFrame();

		if (elf->entry == 0) 
			info.entry = nullptr;
		else
			info.entry = getAddress<uint8_t>(elf->entry);

		return true;
	}

	bool elfModule::unload()
	{
		data.reset();

		//todo: unload memory from VMA
		return true;
	}

	void elfModule::doDynamics()
	{
		const auto* dynS = getSegment(ElfSegType::PT_DYNAMIC);
		const auto* dyldS = getSegment(ElfSegType::PT_SCE_DYNLIBDATA);

		uint8_t *dynldPtr = getOffset<uint8_t>(dyldS->offset);
		uint8_t* dynldAddr = getAddress<uint8_t>(dyldS->vaddr);
		//std::printf("addr = %p\n", dynldAddr);
		ELFDyn* dynamics = getOffset<ELFDyn>(dynS->offset);
		for (int32_t i = 0; i < (dynS->filesz / sizeof(ELFDyn)); i++) {
			auto* d = &dynamics[i];

			switch (d->tag) {
			case DT_SCE_HASH:
				hashes = reinterpret_cast<uint8_t*>(dynldPtr + d->un.value);
				break;
			case DT_INIT:
				info.initAddr = reinterpret_cast<uint8_t*>(dynldAddr + d->un.ptr);
				break;
			case DT_FINI:
				info.finiAddr = reinterpret_cast<uint8_t*>(dynldAddr + d->un.ptr);
				break;
			/*case DT_INIT_ARRAY:
				init_array = reinterpret_cast<linker_function*>(dynldAddr + d->un.ptr);
				break;
			case DT_INIT_ARRAYSZ:
				numInitArray = static_cast<uint32_t>(d->un.value / sizeof(uintptr_t));
				break;
			case DT_FINI_ARRAY:
				fini_array = reinterpret_cast<linker_function*>(dynldAddr + d->un.ptr);
				break;
			case DT_FINI_ARRAYSZ:
				numFiniArray = static_cast<uint32_t>(d->un.value / sizeof(uintptr_t));
				break;*/
			case DT_SCE_JMPREL:
				jmpslots = (ElfRel*)(dynldPtr + d->un.ptr);
				break;
			case DT_PLTRELSZ:
			case DT_SCE_PLTRELSZ:
				numJmpSlots = static_cast<uint32_t>(d->un.value / sizeof(ElfRel));
				break;
			case DT_SCE_STRTAB:
				strtab.ptr = (char*)(dynldPtr + d->un.ptr);
				break;
			case DT_STRSZ:
			case DT_SCE_STRSIZE:
				strtab.size = d->un.value;
				break;
			case DT_SCE_EXPLIB:
			case DT_SCE_IMPLIB:
				break;
			case DT_SCE_SYMTAB:
				symbols = reinterpret_cast<ElfSym*>(dynldPtr + d->un.ptr);
				break;
			case DT_SCE_SYMTABSZ:
				numSymbols = static_cast<uint32_t>(d->un.value / sizeof(ElfSym));
				break;
			case DT_SCE_RELA:
				rela = reinterpret_cast<ElfRel*>(dynldPtr + d->un.ptr);
				break;
			case DT_RELASZ:
			case DT_SCE_RELASZ:
				numRela = static_cast<uint32_t>(d->un.value / sizeof(ElfRel));
				break;
			case DT_SCE_NEEDED_MODULE:
			{
				auto& e = implibs.emplace_back();
				e.name = (const char*)(strtab.ptr + (d->un.value & 0xFFFFFFFF));
				e.modid = d->un.value >> 48;
				break;
			}
			}
		}
	}

	bool elfModule::mapImage()
	{
		// calculate total aligned code size
		uint32_t codeSize = 0;
		for (uint16_t i = 0; i < elf->phnum; ++i) {
			const auto* p = &segments[i];
			if (p->type == PT_LOAD || p->type == PT_SCE_RELRO) {
				codeSize += align_up(p->memsz, p->align);
			}
		}

		// could also check if INTERP exists
		if (codeSize == 0)
			return false;

		// reserve a region from xxxxxxxx00000000 - xxxxxxxxFFFFFFFF
		constexpr size_t one_mb = 1024ull * 1024ull;
		constexpr size_t eight_gb = 8ull * 1024ull * one_mb;

		info.base = static_cast<uint8_t*>(utl::allocMem(nullptr,
			eight_gb, utl::pageProtection::w, utl::allocationType::reserve));

		if (!info.base)
			return false;

		// immediately take module memory + rip Zone memory
		utl::allocMem(info.base, codeSize + info.ripZoneSize, utl::pageProtection::w, utl::allocationType::commit);

		info.codeSize = codeSize;
		info.ripZone = info.base + codeSize;

		std::memset(info.ripZone, 0xCC, info.ripZoneSize);
		utl::protectMem(info.ripZone, info.ripZoneSize, utl::pageProtection::rwx);

#ifdef _DEBUG
		std::printf("Mapped %s @ %p\n", info.name.c_str(), info.base);
#endif

		// step 0: map data
		for (uint16_t i = 0; i < elf->phnum; i++) {
			const auto *s = &segments[i];
			if (s->type == PT_LOAD || s->type == PT_SCE_RELRO) {
				void* target = elf->type == ET_SCE_EXEC ? 
					reinterpret_cast<void*>(s->vaddr) :
					getAddress<void>(s->paddr);

				auto* seg = s->flags & PF_X ? &info.textSeg : &info.dataSeg;
				seg->addr = static_cast<uint8_t*>(target);
				seg->size = s->memsz;

				std::memcpy(target, getOffset<void>(s->offset), s->filesz);
			}
		}

		// step 1: lift code
		for (uint16_t i = 0; i < elf->phnum; i++) {
			const auto *s = &segments[i];
			uint32_t perm = s->flags & (PF_R | PF_W | PF_X);
			if (s->type == PT_LOAD && perm == (PF_R | PF_X)) {
				runtime::codeLift lift(info.ripZone);
				LOG_ASSERT(lift.init());
				lift.transform(getAddress<uint8_t>(s->vaddr), s->filesz);
			}
		}

#ifdef _DEBUG
		// temp hack: raise 5.05 kernel debug msg level
		if (info.name == "libkernel.sprx") {
			*getAddress<uint32_t>(0x68264) = UINT32_MAX;
			LOG_WARNING("Enabling libkernel debug messages");
		}
#endif

		//step 2: apply page protections
		for (uint16_t i = 0; i < elf->phnum; i++) {
			const auto *s = &segments[i];
			if (s->type == PT_LOAD) {
				uint32_t perm = s->flags & (PF_R | PF_W | PF_X);
				auto trans_perm = [](uint32_t op)
				{
					switch (op) {
					case (PF_R | PF_X): return utl::pageProtection::rx;
					case (PF_R | PF_W): return utl::pageProtection::w;
					case (PF_R): return utl::pageProtection::r;
					default: return utl::pageProtection::priv;
					/*todo: invalid parameter bugcheck*/
					}
				};

				utl::protectMem(getAddress<void>(s->vaddr), s->filesz, trans_perm(perm));
			}
		}

		return true;
	}

	bool elfModule::setupTLS()
	{
		auto* p = getSegment(PT_TLS);
		if (p) {

			info.tlsAddr = getAddress<uint8_t>(p->vaddr);
			info.tlsalign = p->align;
			info.tlsSizeFile = p->filesz;
			info.tlsSizeMem = p->memsz;
			info.tlsSlot = process->nextFreeTLS();
		}

		return true;
	}

	// pltrela_table_offset 
	bool elfModule::resolveImports()
	{
		for (uint32_t i = 0; i < numJmpSlots; i++) {
			const auto* r = &jmpslots[i];

			int32_t type = ELF64_R_TYPE(r->info);
			int32_t isym = ELF64_R_SYM(r->info);

			ElfSym* sym = &symbols[isym];

			if (type != R_X86_64_JUMP_SLOT) {
				LOG_WARNING("resolveImports: bad jump slot {}", i);
				continue;
			}

			if ((uint32_t)isym >= numSymbols ||
				sym->st_name >= strtab.size) {
				LOG_WARNING("resolveImports: bad symbol index {} for relocation {}", isym, i);
				continue;
			}

			const char* name = &strtab.ptr[sym->st_name];
			// example: weDug8QD-lE#L#M
			//				^      ^
			// see above: libid, modid

			int32_t binding = ELF64_ST_BIND(sym->st_info);

			if (binding == STB_LOCAL) {
				*getAddress<uintptr_t>(r->offset) = (uintptr_t)getAddress<uintptr_t>(sym->st_value);
				continue;
			}

			if (std::strlen(name) == 15) {

				auto *ptr = &name[14];

				uint64_t modid = 0;
				runtime::decode_nid(ptr, 1, modid);

				uint64_t hid = 0;
				if (!runtime::decode_nid(name, 11, hid)) {
					LOG_ERROR("resolveImports: cant handle NID");
					return false;
				}
	
				for (auto& imp : implibs) {
					if (imp.modid == static_cast<int32_t>(modid)) {
						auto mod = process->loadModule(imp.name);
						if (!mod) {
							LOG_ERROR("resolveImports: Unknown module {} ({}) requestd", imp.name, imp.modid);
							return false;
						}

						*getAddress<uintptr_t>(r->offset) = mod->getSymbol(hid);
						break;
					}
				}
			}
		}

		return true;
	}

	uintptr_t elfModule::getSymbol(uint64_t nid)
	{
		// are there any overrides for me?
		auto imp = runtime::vprx_get(info.name.c_str(), nid);
		if (imp != 0)
			return imp;

		for (uint32_t i = 0; i < numSymbols; i++) {
			const auto* s = &symbols[i];

			if (!s->st_value)
				continue;

			// if the symbol is exported
			//int32_t binding = ELF64_ST_BIND(s->st_info);

			const char* name = &strtab.ptr[s->st_name];

			uint64_t hid = 0;
			if (!runtime::decode_nid(name, 11, hid)) {
				LOG_ERROR("resolveExport: cant handle NID");
				return 0;
			}

			if (nid == hid) {
				return getAddressNPTR<uintptr_t>(s->st_value);
			}
		}

		return 0;
	}

	uintptr_t elfModule::getSymbol(const char *name)
	{
		//TODO: fix elf hash lookup

		auto elfHash = [](const char *name)
		{
			auto p = (const uint8_t*)name;
			uint32_t h = 0;
			uint32_t g;
			while (*p != '\0')
			{
				h = (h << 4) + *p++;
				if ((g = h & 0xF0000000ull) != 0)
				{
					h ^= g >> 24;
				}
				h &= ~g;
			}
			return h;
		};

		auto hash = elfHash(name);

		auto* htab = reinterpret_cast<uint32_t*>(hashes);
		uint32_t nbucket = htab[0];
		uint32_t nchain = htab[1];
		uint32_t* bucket = &htab[2];
		uint32_t* chain = &bucket[nbucket];

		char nameOut[11]{};
		runtime::encode_nid("module_start", reinterpret_cast<uint8_t*>(&nameOut));
	
		for (uint32_t i = bucket[hash % nbucket]; i; i = chain[i]) {
			const auto* s = &symbols[i];

			if (i > nchain)
				return 0;

			if (!s->st_value)
				continue;

			const char* sname = &strtab.ptr[s->st_name];
			if (std::strncmp(sname, nameOut, 10) == 0) {
				return s->st_value;
			}
		}

		return 0;
	}

	uintptr_t elfModule::getSymbol2(const char* name)
	{
		for (uint32_t i = 0; i < numSymbols; i++) {
			const auto* s = &symbols[i];

			if (!s->st_value)
				continue;

			const char* sname = &strtab.ptr[s->st_name];
			if (std::strncmp(sname, name, 10) == 0) {
				return getAddressNPTR<uintptr_t>(s->st_value);
			}
		}

		return 0;
	}

	static PS4ABI void IDontDoNuffin()
	{
		__debugbreak();
	}

	bool elfModule::applyRelocations()
	{
		for (size_t i = 0; i < numRela; i++) {
			auto* r = &rela[i];

			uint32_t isym = ELF64_R_SYM(r->info);
			int32_t type = ELF64_R_TYPE(r->info);

			ElfSym* sym = &symbols[isym];

			if (isym >= numSymbols) {
				LOG_ERROR("Invalid symbol index {}", isym);
				continue;
			}

#if 1
			bool hack = false;

			//f7uOxY9mM1U
			switch (type) {
			case R_X86_64_64:
			case R_X86_64_GLOB_DAT: {
				auto bind = ELF_ST_BIND(sym->st_info);
				if (bind == STB_GLOBAL || STB_LOCAL) {
					const char* name = &strtab.ptr[sym->st_name];
					if (strstr(name, "f7uOxY9mM1U")) {
						//std::printf("name %s, bind %d, type %d\n", name, bind, type);
						*getAddress<uint64_t>(r->offset) = (uint64_t)&IDontDoNuffin;
						hack = true;
					}
				}
			}
			default: break;
			}

			if (hack)
				continue;
#endif

			switch (type) {
			case R_X86_64_NONE: break;
			case R_X86_64_64:
				*getAddress<uint64_t>(r->offset) = sym->st_value + r->addend;
				break;
			case R_X86_64_RELATIVE: /* base + ofs*/
				*getAddress<int64_t>(r->offset) = (int64_t)getAddress<int64_t>(r->addend);
				break;
			case R_X86_64_GLOB_DAT:
				*getAddress<uint64_t>(r->offset) = sym->st_value;
				break;
			case R_X86_64_PC32:
				*getAddress<uint32_t>(r->offset) = (uint32_t)(sym->st_value + r->addend - (uint64_t)getAddress<uint64_t>(r->offset));
				break;
			case R_X86_64_DTPMOD64:
				*getAddress<uint16_t>(r->offset) = info.tlsSlot;
				break;
			default:
				continue;
			}
		}

		return true;
	}

	// taken from idc's "uplift" project
	void elfModule::installEHFrame()
	{
		const auto* p = getSegment(PT_GNU_EH_FRAME);
		if (p->filesz > p->memsz)
			return;

		info.ehFrameAddr = getAddress<uint8_t>(p->vaddr);
		info.ehFrameSize = p->memsz;

		// custom struct for eh_frame_hdr 
		struct GnuExceptionInfo
		{
			uint8_t version;
			uint8_t encoding;
			uint8_t fdeCount;
			uint8_t encodingTable;
			uint8_t first;
		};

		auto* exinfo = getOffset<GnuExceptionInfo>(p->offset);

		if (exinfo->version != 1)
			return;

		uint8_t* data_buffer = nullptr;
		uint8_t* current = &exinfo->first;

		if (exinfo->encoding == 0x03) // relative to base address
		{
			auto offset = *reinterpret_cast<uint32_t*>(current);
			current += 4;

			data_buffer = (uint8_t*)&info.base[offset];
		}
		else if (exinfo->encoding == 0x1B) // relative to eh_frame
		{
			auto offset = *reinterpret_cast<int32_t*>(current);
			current += 4;
			data_buffer = &current[offset];
		}
		else
		{
			return;
		}

		if (!data_buffer)
		{
			return;
		}

		uint8_t* data_buffer_end = data_buffer;
		while (true)
		{
			size_t size = *reinterpret_cast<int32_t*>(data_buffer_end);
			if (size == 0)
			{
				data_buffer_end = &data_buffer_end[4];
				break;
			}
			if (size == -1)
			{
				size = 12 + *reinterpret_cast<size_t*>(&data_buffer_end[4]);
			}
			else
			{
				size = 4 + size;
			}
			data_buffer_end = &data_buffer_end[size];
		}

		size_t fde_count;
		if (exinfo->fdeCount == 0x03) // absolute
		{
			fde_count = *reinterpret_cast<uint32_t*>(current);
			current += 4;
		}
		else
		{
			return;
		}

		if (exinfo->encodingTable != 0x3B) // relative to eh_frame
		{
			return;
		}

		info.ehFrameheaderAddr = data_buffer;
		info.ehFrameheaderSize = data_buffer_end - data_buffer;
	}

	void elfModule::logDbgInfo()
	{
		for (uint16_t i = 0; i < elf->phnum; i++) {
			auto s = &segments[i];
			switch (s->type) {
			case PT_SCE_COMMENT:
			{
				// this is similar to the windows pdb path
				auto* comment = getOffset<SCEComment>(s->offset);

				std::string name;
				name.resize(comment->pathLength);
				memcpy(name.data(), getOffset<void>(s->offset + sizeof(SCEComment)), comment->pathLength);

				LOG_INFO("Starting: {}", name);
				break;
			}
			case PT_SCE_LIBVERSION:
			{
				uint8_t* sec = getOffset<uint8_t>(s->offset);

				// count entries
				int32_t index = 0;
				while (index <= s->filesz) {

					int8_t cb = sec[index];

					// skip control byte
					index++;

					for (int i = index; i < (index + cb); i++)
					{
						if (sec[i] == 0x3A) {

							size_t length = i - index;

							std::string name;
							name.resize(length);
							memcpy(name.data(), &sec[index], length);

							uint32_t version = *(uint32_t*)& sec[i + 1];
							uint8_t* vptr = (uint8_t*)& version;

							//std::printf("lib <%s>, version %x.%x.%x.%x\n", name.c_str(), vptr[0], vptr[1], vptr[2], vptr[3]);
							break;
						}
					}

					// skip forward
					index += cb;
				}
				break;
			}
			}
		}
	}
}