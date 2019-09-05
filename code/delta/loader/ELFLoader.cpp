
// Copyright (C) Force67 2019

#include "loader/ELFLoader.h"
#include "kernel/Process.h"

#include "modules/Module.h"

namespace loaders
{
	// TODO: move + proper imp
	int decode_base64(const char* str, int* a2)
	{
		char chr; // dl@1
		int v3; // rcx@1
		const char* v4; // rdi@2
		int v5; // rcx@3
		int result; // rax@11

		chr = *str;
		v3 = 0LL;
		if (*str) {
			v4 = str + 1;
			v3 = 0LL;
			do {
				v5 = v3 << 6;
				if ((unsigned __int8)(chr - 0x61) > 0x19u) {
					if ((unsigned __int8)(chr - 0x41) > 0x19u) {
						if ((unsigned __int8)(chr - 0x30) > 9u) {
							if (chr == '-')
								v3 = v5 | 0x3F;
							else {
								result = 22LL;
								if (chr != '+')
									return result;
								v3 = v5 | 0x3E;
							}
						}
						else {
							v3 = chr + (v5 | 4);
						}
					}
					else {
						v3 = v5 + chr - 0x41;
					}
				}
				else {
					v3 = v5 + chr - 0x47;
				}
				chr = *v4++;
			} while (chr);
		}
		*a2 = v3;
		return 0LL;
	}

	// based on: https://github.com/idc/uplift/blob/master/uplift/src/program_info.cpp

	ELF_Loader::ELF_Loader(std::unique_ptr<uint8_t[]> d) :
		AppLoader(std::move(d))
	{}

	FileType ELF_Loader::IdentifyType(utl::File& file)
	{
		ELFHeader elf{};
		file.Read(elf);

		if (elf.magic == ELF_MAGIC && elf.machine == ELF_MACHINE_X86_64) {
			std::puts("[+] Identified file as plain ELF");
			return FileType::ELF;
		}

		return FileType::UNKNOWN;
	}

	LoadErrorCode ELF_Loader::Load(krnl::Process& proc)
	{
		elf = GetOffset<ELFHeader>(0);
		segments = GetOffset<ELFPgHeader>(elf->phoff);

		for (uint16_t i = 0; i < elf->phnum; i++) {
			auto s = &segments[i];
			switch (s->type) {
			case PT_LOAD:
			{
				// test alignment
				if (s->align & 0x3FFFull || s->vaddr & 0x3FFFull || s->offset & 0x3FFFull)
					return LoadErrorCode::BADALIGN;

				if (s->filesz > s->memsz)
					return LoadErrorCode::BADSEG;
				break;
			}
			case PT_TLS:
			{
				if (s->filesz > s->memsz)
					return LoadErrorCode::BADSEG;

				if (s->align > 32)
					return LoadErrorCode::BADALIGN;

				break;
			}
			case PT_SCE_DYNLIBDATA:
			{
				if (s->filesz == 0) {
					//__debugbreak();
					return LoadErrorCode::BADSEG;
				}

				dynld.ptr = GetOffset<char>(s->offset);
				dynld.size = s->filesz;
				break;
			}
			case PT_DYNAMIC:
			{
				if (s->filesz > s->memsz)
					return LoadErrorCode::BADSEG;
				break;
			}
			case PT_SCE_MODULEPARAM:
			{
				std::printf("FOUND PROCPARAM %llx (%llx byts big)\n", s->offset, s->filesz);

			} break;

			// like the PDB path on windows
			case PT_SCE_COMMENT:
			{
				auto* comment = GetOffset<SCEComment>(s->offset);

				std::string name;
				name.resize(comment->pathLength);
				memcpy(name.data(), GetOffset<void>(s->offset + sizeof(SCEComment)), comment->pathLength);

				std::printf("Comment %s, %d\n", name.c_str(), comment->unk);
				break;
			}
			case PT_SCE_LIBVERSION:
			{
				uint8_t* sec = GetOffset<uint8_t>(s->offset);

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

		DoDynamics();

		if (!MapImage(proc)) {
			return LoadErrorCode::BADMAP;
		}

		if (!ProcessRelocations()) {
			return LoadErrorCode::BADREL;
		}

		if (!ResolveImports()) {
			return LoadErrorCode::BADIMP;
		}

		InstallExceptionHandlers();

		return LoadErrorCode::SUCCESS;
	}

	void ELF_Loader::DoDynamics()
	{
		auto* s = GetSegment(ElfSegType::PT_DYNAMIC);
		ELFDyn* dynamics = GetOffset<ELFDyn>(s->offset);
		for (int32_t i = 0; i < (s->filesz / sizeof(ELFDyn)); i++) {
			auto* d = &dynamics[i];

			switch (d->tag) {
			case DT_SCE_JMPREL:
			{
				if (d->un.ptr > dynld.size) {
					std::printf("[!] bad JMPREL offset: %llx\n", d->un.ptr);
					continue;
				}

				jmpslots = (ElfRel*)(dynld.ptr + d->un.ptr);
				break;
			}
			case DT_SCE_PLTRELSZ:
			{
				numJmpSlots = d->un.value / sizeof(ElfRel);
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
				numSymbols = d->un.value / sizeof(ElfSym);
				break;
			}
			case DT_SCE_RELA:
			{
				rela = (ElfRel*)(dynld.ptr + d->un.ptr);
				break;
			}
			case DT_SCE_RELASZ:
			{
				numRela = d->un.value / sizeof(ElfRel);
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
					break;
				}
			}
		}
	}

	bool ELF_Loader::MapImage(krnl::Process& proc)
	{
		// the stuff we actually care about
		uint32_t totalimage = 0;
		for (uint16_t i = 0; i < elf->phnum; ++i) {
			const auto* p = &segments[i];
			if (p->type == PT_LOAD /*|| PT_SCE_RELRO*/) {
				totalimage += (p->memsz + 0xFFF) & ~0xFFF;
			}
		}

		// could also check if INTERP exists
		if (totalimage == 0)
			return false;

		// reserve segment
		targetbase = (char*)proc.GetVirtualMemory().AllocateSeg(totalimage);

		// and move it!
		for (uint16_t i = 0; i < elf->phnum; i++) {
			auto s = &segments[i];
			if (s->type == PT_LOAD /*|| PT_SCE_RELRO*/) {
				uint32_t perm = s->flags & (PF_R | PF_W | PF_X);
				switch (perm) {

				// todo: module segment collection :D
				case (PF_R | PF_X):
				{
					// its a codepage
					std::memcpy(targetbase + s->vaddr, GetOffset<void>(s->offset), s->filesz);
					break;
				}
				case (PF_R):
				{
					//Reserve a rdata segment
					std::memcpy(targetbase + s->vaddr, GetOffset<void>(s->offset), s->filesz);
					break;
				}
				case (PF_R | PF_W):
				{
					// reserve a read write data seg
					std::memcpy(targetbase + s->vaddr, GetOffset<void>(s->offset), s->filesz);
					break;
				}
				default:
				{
					std::puts("UNKNOWN param!");

				}
				}
			}
		}

		// kind of a hack tbh
		static int32_t tlsIdx{0};

		// register it to module chain
		auto entry = std::make_shared<krnl::Module>();
		entry->base = reinterpret_cast<uint8_t*>(targetbase);
		entry->entry = reinterpret_cast<uint8_t*>(targetbase + elf->entry);
		entry->sizeCode = totalimage;
		entry->name = "#MAIN#";
		entry->tlsSlot = tlsIdx;
		proc.RegisterModule(std::move(entry));

#if 0
		{
			utl::File f(LR"(C:\Users\vince\Desktop\.nomad\ps4delta\bin\Debug\proc.bin)", utl::fileMode::write);
			f.Write(targetbase, totalimage);
		}
#endif

		return true;
	}

	bool ELF_Loader::ResolveImports()
	{
		for (int32_t i = 0; i < numJmpSlots; i++) {
			auto* r = &jmpslots[i];

			int32_t type = ELF64_R_TYPE(r->info);
			int32_t isym = ELF64_R_SYM(r->info);

			ElfSym* sym = &symbols[isym];

			if (isym >= numSymbols ||
				sym->st_name >= strtab.size) {
				std::printf("Bad symbol idx %d for relocation %d\n", isym, i);
				continue;
			}

			const char* name = &strtab.ptr[sym->st_name];
			//std::printf("SYMBOL binding %s, BINDING: %x, TYPE: %x\n", name, sym->st_info >> 4, sym->st_info & 0x0f);

			// example: weDug8QD-lE#L#M
			//					    ^ ^
			// see above: libid, modid

			if (std::strlen(name) == 15) {

				auto *ptr = &name[14];

				int32_t modid = 0;
				decode_base64(ptr, &modid);

				char hashname[12]{};
				strncpy(hashname, name, 11);

				// fetch the import module name
				for (auto& imp : implibs) {
					if (imp.modid == modid) {

					// ... and set the import address
					*GetAddress<uintptr_t>(r->offset) = modules::get_import(imp.name, hashname);
					break;
				}
				}
			}
		}

		return true;
	}

	bool ELF_Loader::ProcessRelocations()
	{
		for (size_t i = 0; i < numRela; i++) {
			auto* r = &rela[i];

			uint32_t isym = ELF64_R_SYM(r->info);
			int32_t type = ELF64_R_TYPE(r->info);

			if (isym >= numSymbols) {
				std::printf("[!] Invalid symbol index %d\n", isym);
				continue;
			}

			switch (type) {
			case R_X86_64_NONE: break;
			case R_X86_64_64:
			{
				*GetAddress<uint64_t>(r->offset) = symbols[isym].st_value + r->addend;
				break;
			}
			case R_X86_64_RELATIVE: /* base + ofs*/
			{
				*GetAddress<int64_t>(r->offset) = (int64_t)GetAddress<int64_t>(r->addend);
				break;
			}
			case R_X86_64_GLOB_DAT:
			{
				*GetAddress<uint64_t>(r->offset) = symbols[isym].st_value;
				break;
			}
			case R_X86_64_PC32:
			{
				*GetAddress<uint32_t>(r->offset) = (uint32_t)(symbols[isym].st_value + r->addend - (uint64_t)GetAddress<uint64_t>(r->offset));
				break;
			}
			case R_X86_64_TPOFF64:
			case R_X86_64_TPOFF32:
			default: return false;

			}
		}

		return true;
	}

	// taken from idc's "uplift" project
	void ELF_Loader::InstallExceptionHandlers()
	{
		const auto* p = GetSegment(PT_GNU_EH_FRAME);
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

		auto* info = GetOffset<GnuExceptionInfo>(p->offset);

		if (info->version != 1)
			return;

		uint8_t* data_buffer = nullptr;
		uint8_t* current = &info->first;

		if (info->encoding == 0x03) // relative to base address
		{
			auto offset = *reinterpret_cast<uint32_t*>(current);
			current += 4;
			data_buffer = (uint8_t*)&targetbase[offset];
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

	LoadErrorCode ELF_Loader::Unload()
	{
		return LoadErrorCode::UNKNOWN;
	}
}