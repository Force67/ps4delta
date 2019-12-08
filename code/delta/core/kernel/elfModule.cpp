
// Copyright (C) Force67 2019

#include <utl/File.h>
#include "modules/vmodLinker.h"

#include "elfModule.h"
#include "proc.h"

namespace krnl
{
	elfModule::elfModule(krnl::proc *owner) :
		owner(owner)
	{
		/*-1 = no tls used*/
		tlsSlot = -1;
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
			LOG_TRACE("Identified file as plain ELF");
			
			data = std::make_unique<uint8_t[]>(file.GetSize());
			file.Read(data.get(), file.GetSize());

			elf = getOffset<ELFHeader>(0);
			segments = getOffset<ELFPgHeader>(elf->phoff);

			if (!initSegments()) {
				LOG_ERROR("initFile: unable to init segments");
				return false;
			}

			doDynamics();
			logDbgInfo();

			if (!mapImage() ||
				!setupTLS()) {
				LOG_ERROR("elfModule: Failed to map image");
				return false;
			}

			if (!applyRelocations()) {
				LOG_ERROR("elfModule: Failed to relocate image");
				return false;
			}

			if (!resolveImports()) {
				LOG_ERROR("elfModule: Failed to resolve imports");
				return false;
			}

			installEHFrame();

			// attempt to gather module name from file path
			auto pos = path.find_last_of('\\');
			if (pos != std::string::npos) {
				name = path.substr(pos + 1, path.length() - pos);
				LOG_INFO("Loading {}", name);
			}
			else
				name = "#unk";

			if (elf->entry == 0)
				entry = nullptr;
			else
				entry = base + elf->entry;

			return true;
		}

		return false;
	}

	bool elfModule::initSegments()
	{
		for (uint16_t i = 0; i < elf->phnum; i++) {
			auto s = &segments[i];
			//LOG_TRACE("SEGMENT {}, {},{} -> TYPE {}, size {}", i, s->offset, s->vaddr, SegTypeToString(s->type), s->filesz);

			switch (s->type) {
			case PT_LOAD:
			{
				if (s->align & 0x3FFFull || s->vaddr & 0x3FFFull || s->offset & 0x3FFFull) {
					LOG_ERROR("elfModule: Bad section alignment");
					return false;
				}

				LOG_ASSERT(s->filesz > s->memsz);
				break;
			}
			case PT_TLS:
			{
				LOG_ASSERT(s->filesz > s->memsz);

				if (s->align > 32) {
					LOG_ERROR("elfModule: Bad TLS seg alignment");
					return false;
				}

				break;
			}
			case PT_SCE_DYNLIBDATA:
			{
				LOG_ASSERT(s->filesz == 0);

				dynld.ptr = getOffset<char>(s->offset);
				dynld.size = s->filesz;
				break;
			}
			case PT_DYNAMIC:
			{
				LOG_ASSERT(s->filesz > s->memsz);
				break;
			}
			}
		}

		return true;
	}

	void elfModule::doDynamics()
	{
		auto* s = getSegment(ElfSegType::PT_DYNAMIC);
		ELFDyn* dynamics = getOffset<ELFDyn>(s->offset);
		for (int32_t i = 0; i < (s->filesz / sizeof(ELFDyn)); i++) {
			auto* d = &dynamics[i];

			switch (d->tag) {
			case DT_INIT:
			case DT_INIT_ARRAY:
			{
				LOG_TRACE("DT_INIT {}", d->un.value);
				break;
			}
			case DT_SCE_JMPREL:
			{
				if (d->un.ptr > dynld.size) {
					std::printf("[!] bad JMPREL offset: %llx\n", d->un.ptr);
					continue;
				}

				jmpslots = (ElfRel*)(dynld.ptr + d->un.ptr);
				break;
			}
			case DT_PLTRELSZ:
			case DT_SCE_PLTRELSZ:
			{
				numJmpSlots = static_cast<uint32_t>(d->un.value / sizeof(ElfRel));
				break;
			}
			case DT_SCE_STRTAB:
			{
				if (d->un.ptr > dynld.size) {
					std::printf("[!] bad STRTAB offset: %llx", d->un.ptr);
					continue;
				}

				strtab.ptr = (char*)(dynld.ptr + d->un.ptr);
				break;
			}
			case DT_STRSZ:
			case DT_SCE_STRSIZE:
			{
				strtab.size = d->un.value;
				break;
			}
			case DT_SCE_EXPLIB:
			case DT_SCE_IMPLIB:
			{

				break;
			}
			case DT_SCE_SYMTAB:
			{
				if (d->un.ptr > dynld.size) {
					std::printf("[!] bad SYMTAB offset: %llx", d->un.ptr);
					continue;
				}

				symbols = (ElfSym*)(dynld.ptr + d->un.ptr);
				break;
			}
			case DT_SCE_SYMTABSZ:
			{
				numSymbols = static_cast<uint32_t>(d->un.value / sizeof(ElfSym));
				break;
			}
			case DT_SCE_RELA:
			{
				rela = (ElfRel*)(dynld.ptr + d->un.ptr);
				break;
			}
			case DT_RELASZ:
			case DT_SCE_RELASZ:
			{
				numRela = static_cast<uint32_t>(d->un.value / sizeof(ElfRel));
				break;
			}
			}
		}

		for (int32_t i = 0; i < (s->filesz / sizeof(ELFDyn)); i++) {
			auto* d = &dynamics[i];
			switch (d->tag) {
				/*case DT_NEEDED:
				{
					std::printf("%i: DT_NEEDED %s\n", i, (char*)(strtab.ptr + d->un.value));
					break;
				}*/
				case DT_SCE_NEEDED_MODULE:
				{
					auto& e = implibs.emplace_back();
					e.name = (const char*)(strtab.ptr + (d->un.value & 0xFFFFFFFF));
					e.modid = d->un.value >> 48;

				//	std::printf("%s\n", e.name);
					break;
				}
			}
		}
	}

	bool elfModule::mapImage()
	{
	/*	auto* it = getSegment(PT_SCE_RELRO);
		if (it) {
			for (int i = 0; i < it->filesz; i += 8) {
				std::printf("RELRO entry %llx\n", *getOffset<uintptr_t>(it->offset + i));
			}
		}*/

		// the stuff we actually care about
		codeSize = 0;
		for (uint16_t i = 0; i < elf->phnum; ++i) {
			const auto* p = &segments[i];
			if (p->type == PT_LOAD) {
				codeSize += (p->memsz + 0xFFF) & ~0xFFF;
			}
		}

		// could also check if INTERP exists
		if (codeSize == 0)
			return false;

		// reserve segment
		base = (uint8_t*)owner->vmem.AllocateSeg(codeSize);
		if (!base)
			return false;

		// and move it!
		for (uint16_t i = 0; i < elf->phnum; i++) {
			auto s = &segments[i];
			if (s->type == PT_LOAD) {
				uint32_t perm = s->flags & (PF_R | PF_W | PF_X);
				switch (perm) {

				// todo: module segment collection :D
				case (PF_R | PF_X):
				{
					// its a codepage
					std::memcpy(getAddress<void>(s->vaddr), getOffset<void>(s->offset), s->filesz);
					break;
				}
				case (PF_R):
				{
					//Reserve a rdata segment
					std::memcpy(getAddress<void>(s->vaddr), getOffset<void>(s->offset), s->filesz);
					break;
				}
				case (PF_R | PF_W):
				{
					// reserve a read write data seg
					std::memcpy(getAddress<void>(s->vaddr), getOffset<void>(s->offset), s->filesz);
					break;
				}
				default:
				{
					std::puts("UNKNOWN param!");

				}
				}
			}
			else if (s->type == PT_SCE_RELRO) {
				std::memcpy(getAddress<void>(s->vaddr), getOffset<void>(s->offset), s->filesz);
			}
		}

		return true;
	}

	bool elfModule::setupTLS()
	{
		auto* p = getSegment(PT_TLS);
		if (p) {

			tlsSlot = owner->nextFreeTLS();
		}

		return true;
	}

	// pltrela_table_offset 
	bool elfModule::resolveImports()
	{
		for (uint32_t i = 0; i < numJmpSlots; i++) {
			auto* r = &jmpslots[i];

			int32_t type = ELF64_R_TYPE(r->info);
			int32_t isym = ELF64_R_SYM(r->info);

			ElfSym* sym = &symbols[isym];

			if (type != R_X86_64_JUMP_SLOT) {
				LOG_WARNING("resolveImports: bad jump slot {}", i);
				continue;;
			}

			if ((uint32_t)isym >= numSymbols ||
				sym->st_name >= strtab.size) {
				LOG_WARNING("resolveImports: bad symbol index {} for relocation {}", isym, i);
				continue;
			}

			const char* name = &strtab.ptr[sym->st_name];
			//std::printf("SYMBOL binding %s, BINDING: %d, TYPE: %d\n", name, sym->st_info >> 4, sym->st_info & 0x0f);

			// example: weDug8QD-lE#L#M
			//					    ^ ^
			// see above: libid, modid

			int32_t binding = ELF64_ST_BIND(sym->st_info);
			if (binding == STB_LOCAL) {
				*getAddress<uintptr_t>(r->offset) = (uintptr_t)getAddress<uintptr_t>(sym->st_value);
				break;
			}

			if (std::strlen(name) == 15) {

				auto *ptr = &name[14];

				uint64_t modid = 0;
				modules::decodeNid(ptr, 1, modid);

				uint64_t hid = 0;
				if (!modules::decodeNid(name, 11, hid)) {
					LOG_ERROR("resolveImports: cant handle NID");
					return false;
				}
	
				for (auto& imp : implibs) {
					if (imp.modid == static_cast<int32_t>(modid)) {

					LOG_WARNING("MODULE {}", imp.name);

					// ... and set the import address
					*getAddress<uintptr_t>(r->offset) = modules::getImport(imp.name, hid);
					break;
				}
				}
			} 
		}

		return true;
	}

	bool elfModule::applyRelocations()
	{
		for (size_t i = 0; i < numRela; i++) {
			auto* r = &rela[i];

			uint32_t isym = ELF64_R_SYM(r->info);
			int32_t type = ELF64_R_TYPE(r->info);

			ElfSym* sym = &symbols[isym];

			if (isym >= numSymbols) {
				std::printf("[!] Invalid symbol index %d\n", isym);
				continue;
			}

			switch (type) {
			case R_X86_64_NONE: break;
			case R_X86_64_64:
			{
				// class type info and such..
				//std::printf("RELOCATION SHIT %s\n", (char*)& strtab.ptr[sym->st_name]);
				*getAddress<uint64_t>(r->offset) = sym->st_value + r->addend;
				break;
			}
			case R_X86_64_RELATIVE: /* base + ofs*/
			{
				*getAddress<int64_t>(r->offset) = (int64_t)getAddress<int64_t>(r->addend);
				break;
			}
			case R_X86_64_GLOB_DAT:
			{
				*getAddress<uint64_t>(r->offset) = sym->st_value;
				break;
			}
			case R_X86_64_PC32:
			{
				*getAddress<uint32_t>(r->offset) = (uint32_t)(sym->st_value + r->addend - (uint64_t)getAddress<uint64_t>(r->offset));
				break;
			}
			case R_X86_64_DTPMOD64:
			{
				// set tls index for image
				*getAddress<uint16_t>(r->offset) = tlsSlot;
				break;
			}
			case R_X86_64_TPOFF64:
			case R_X86_64_TPOFF32: 
			default:
			{
				std::printf("unknown relocation type %d\n", type);
				return false;
			}
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

		// custom struct for eh_frame_hdr 
		struct GnuExceptionInfo
		{
			uint8_t version;
			uint8_t encoding;
			uint8_t fdeCount;
			uint8_t encodingTable;
			uint8_t first;
		};

		auto* info = getOffset<GnuExceptionInfo>(p->offset);

		if (info->version != 1)
			return;

		uint8_t* data_buffer = nullptr;
		uint8_t* current = &info->first;

		if (info->encoding == 0x03) // relative to base address
		{
			auto offset = *reinterpret_cast<uint32_t*>(current);
			current += 4;
			data_buffer = (uint8_t*)&base[offset];
		}
		else if (info->encoding == 0x1B) // relative to eh_frame
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
		if (info->fdeCount == 0x03) // absolute
		{
			fde_count = *reinterpret_cast<uint32_t*>(current);
			current += 4;
		}
		else
		{
			return;
		}

		if (info->encodingTable != 0x3B) // relative to eh_frame
		{
			return;
		}
	}

	void elfModule::logDbgInfo()
	{
		for (uint16_t i = 0; i < elf->phnum; i++) {
			auto s = &segments[i];
			switch (s->type) {
			case PT_SCE_PROCPARAM:
			{
				struct SCEPROC
				{
					uint32_t length;
					uint32_t unk;
					uint32_t magic;
					uint32_t version;
					uint32_t ofs;
				};

				SCEPROC* pr = getOffset<SCEPROC>(s->offset);


				//std::printf("FOUND PROCPARAM %llx (%llx byts big) proc ofs %llx, %llx\n", s->offset, s->filesz, pr->ofs, getOffset<uintptr_t>(pr->ofs));

			} break;
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