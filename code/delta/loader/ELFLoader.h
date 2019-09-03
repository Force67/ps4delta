#pragma once

// Copyright (C) Force67 2019

#include "SELFLoader.h"

namespace loaders
{
	// a loader for loading decrypted ps4
	// elf images
	class ELF_Loader final : public AppLoader
	{
		ELFHeader elf;
		std::vector<ELFPgHeader> segments;

		uint64_t stringTable;
		uint64_t strTableSize;

		struct LibraryInfo
		{
			uint64_t offset;
			const char* name;
			bool exp;
		};

		//std::

		void DoDynamics(ELFPgHeader&);

	public:

		explicit ELF_Loader(std::unique_ptr<utl::File>);

		static FileType IdentifyType(utl::File&);

		LoadErrorCode Load(krnl::VMAccessMgr&) override;
		LoadErrorCode Unload() override;
	};
}