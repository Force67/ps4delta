
// Copyright (C) Force67 2019

#include "loader/ELFLoader.h"

namespace loaders
{
	// based on: https://github.com/idc/uplift/blob/master/uplift/src/program_info.cpp

	ELF_Loader::ELF_Loader(utl::File file) :
		AppLoader(file)
	{}

	FileType ELF_Loader::IdentifyType(utl::File& file)
	{
		// pretend success, for now :)
		return FileType::ELF;

		ELFHeader elf{};
		file.Read(elf);
		//__debugbreak();
		if (elf.magic == ELF_MAGIC && elf.machine == ELF_MACHINE_X86_64) {
			std::puts("[+] Identified file as plain ELF");
			return FileType::ELF;
		}

		return FileType::UNKNOWN;
	}

	LoadErrorCode ELF_Loader::Load(krnl::VMAccessMgr& vma)
	{

		file.Seek(0, utl::seekMode::seek_set);

		// read headers
		file.Read(elf);

		// the segment descriptors
		segments.resize(elf.phnum);
		file.Read(segments);

		for (auto& s : segments) {
			switch (s.type) {
			case PT_LOAD:
			{
				// test alignment
				if (s.align & 0x3FFFull || s.vaddr & 0x3FFFull || s.offset & 0x3FFFull)
					return LoadErrorCode::BADALIGN;

				if (s.filesz > s.memsz)
					return LoadErrorCode::BADSEG;
				break;
			}
			case PT_TLS:
			{
				if (s.filesz > s.memsz)
					return LoadErrorCode::BADSEG;

				if (s.align > 32)
					return LoadErrorCode::BADALIGN;

				break;
			}
			case PT_DYNAMIC:
			{
				if (s.filesz > s.memsz)
					return LoadErrorCode::BADSEG;

				DoDynamics(s);
				break;
			}
			case PT_SCE_MODULEPARAM:
			{
				std::printf("FOUND PROCPARAM %llx (%llx byts big)\n", s.offset, s.filesz);

			} break;

			// like the PDB path on windows
			case PT_SCE_COMMENT:
			{
				file.Seek(s.offset, utl::seekMode::seek_set);

				SCEComment comment{};
				file.Read(comment);
				
				std::string name;
				name.resize(comment.pathLength);
				file.Read(name.data(), comment.pathLength);

				std::printf("Comment %s, %d\n", name.c_str(), comment.unk);
				break;
			}
			case PT_SCE_LIBVERSION:
			{
				file.Seek(s.offset, utl::seekMode::seek_set);
				std::vector<uint8_t> sec(s.filesz);
				file.Read(sec);

				// count entries
				int32_t index = 0;
				while (index <= s.filesz) {

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
							uint8_t* vptr = (uint8_t*)&version;

							std::printf("lib <%s>, version %x.%x.%x.%x\n", name.c_str(), vptr[0], vptr[1], vptr[2], vptr[3]);
							break;
						}
					}

					// skip forward
					index += cb;
				}
				break;
			}
			case PT_SCE_DYNLIBDATA:
			{
				std::printf("FOUND DYNLIBDATA %llx (%llx byts big)\n", s.offset, s.filesz);
				break;
			}
			}
		}

		return LoadErrorCode::SUCCESS;
	}

	void ELF_Loader::DoDynamics(ELFPgHeader& s)
	{
		std::vector<ELFDyn> dynamics(s.filesz / sizeof(ELFDyn));

		file.Seek(s.offset, utl::seekMode::seek_set);
		file.Read(dynamics);

		for (auto& d : dynamics) {
			switch (d.tag) {
			case DT_SCE_STRTAB: 
			{
				stringTable = d.value;
				break;
			}
			case DT_SCE_STRSIZE:
			{
				strTableSize = d.value;
				break;
			}
			case DT_SCE_EXPLIB:
			case DT_SCE_IMPLIB:
			{

				break;
			}
			}
		}

	}

	LoadErrorCode ELF_Loader::Unload()
	{
		return LoadErrorCode::UNKNOWN;
	}
}