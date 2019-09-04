#pragma once

// Copyright (C) Force67 2019

#include <ELFTypes.h>
#include <SCETypes.h>

#include "kernel/Process.h"
#include "loader/Loader.h"

namespace loaders
{
	// a loader for loading decrypted ps4
	// elf images
	class ELF_Loader final : public AppLoader
	{
		// headers
		ELFHeader *elf;
		ELFPgHeader* segments;
		ElfRel* jmpslots;
		ElfSym* symbols;

		struct Table
		{
			char* ptr;
			size_t size;
		};

		Table strtab;
		Table symtab;
		Table dynld;

		int32_t numJmpSlots;
		int32_t numSymbols;


		struct LibraryInfo
		{
			uint64_t offset;
			const char* name;
			bool exp;
		};

		template<typename Type, typename TAdd>
		Type* GetOffset(const TAdd dist) {
			return (Type*)(data.get() + dist);
		}

		template<typename Type = ELFPgHeader>
		Type* GetSegment(ElfSegType type) {
			for (uint16_t i = 0; i < elf->phnum; i++) {
				auto s = &segments[i];
				if (s->type == type)
					return reinterpret_cast<Type*>(s);
			}

			return nullptr;
		}

		void DoDynamics();
		bool MapImage(krnl::Process&);
		bool ResolveImports();

	public:

		explicit ELF_Loader(std::unique_ptr<uint8_t[]>);

		static FileType IdentifyType(utl::File&);

		LoadErrorCode Load(krnl::Process&) override;
		LoadErrorCode Unload() override;
	};
}