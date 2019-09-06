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
		struct Table
		{
			char* ptr;
			size_t size;
		};

		struct ImportLib
		{
			const char* name;
			int32_t modid;
		};
		std::vector<ImportLib> implibs;

		// headers
		ELFHeader *elf;
		ELFPgHeader* segments;
		ElfRel* jmpslots;
		ElfRel* rela;
		ElfSym* symbols;

		Table strtab;
		Table symtab;
		Table dynld;

		uint32_t numJmpSlots;
		uint32_t numSymbols;
		uint32_t numRela;
		char* targetbase;

		template<typename Type, typename TAdd>
		Type* GetOffset(const TAdd dist) {
			return (Type*)(data.get() + dist);
		}

		template<typename Type, typename TAdd>
		Type* GetAddress(const TAdd dist) {
			return (Type*)(targetbase + dist);
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
		void InstallExceptionHandlers();
		bool MapImage(krnl::Process&);
		bool ResolveImports();
		bool ProcessRelocations();

	public:

		explicit ELF_Loader(std::unique_ptr<uint8_t[]>);

		static FileType IdentifyType(utl::File&);

		FileType GetFileType() override {
			return FileType::ELF;
		}

		LoadErrorCode Load(krnl::Process&) override;
	};
}