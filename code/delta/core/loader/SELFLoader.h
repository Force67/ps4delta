#pragma once

// Copyright (C) Force67 2019

#include "Loader.h"

#include <ELFTypes.h>
#include <SCETypes.h>

namespace utl {
	class File;
}

namespace loaders
{
#if 0
	class SELF_Loader final : public AppLoader
	{
		ELFHeader elf;
		std::vector<ELFPgHeader> segments;
	
		// ensure correct pointer casts
		template<typename Type, typename TAdd>
		Type* GetElfOfs(const TAdd dist) {
			return (Type*)((uint8_t*)elf + dist);
		}

		template<typename Type, typename TAdd>
		Type* GetOffset(const TAdd dist) {
			return (Type*)(data + dist);
		}

		// image setup
		void SetupDynamics(const ELFPgHeader*);
		void SetupTLS(const ELFPgHeader*);
		bool MapSegments(krnl::VMAccessMgr&);

	public:

		explicit SELF_Loader(std::unique_ptr<utl::File> file);

		static FileType IdentifyType(utl::File&);

		LoadErrorCode Load(krnl::VMAccessMgr&) override;
		LoadErrorCode Unload() override;
	};
#endif
}