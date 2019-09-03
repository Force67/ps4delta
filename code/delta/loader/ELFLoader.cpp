
// Copyright (C) Force67 2019

#include "loader/ELFLoader.h"

namespace loaders
{
	// based on: https://github.com/idc/uplift/blob/master/uplift/src/program_info.cpp

	ELF_Loader::ELF_Loader(std::unique_ptr<utl::File> file) :
		AppLoader(std::move(file))
	{}

	FileType ELF_Loader::IdentifyType(utl::File& file)
	{
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
		file->Seek(0, utl::seekMode::seek_set);

		// read headers
		file->Read(elf);

		// the segment descriptors
		segments.resize(elf.phnum);
		file->Read(segments);

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
			}
		}

		return LoadErrorCode::SUCCESS;
	}

	void ELF_Loader::DoDynamics(ELFPgHeader& s)
	{
		std::vector<ELFDyn> dynamics(s.filesz / sizeof(ELFDyn));

		file->Seek(s.offset, utl::seekMode::seek_set);
		file->Read(dynamics);

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