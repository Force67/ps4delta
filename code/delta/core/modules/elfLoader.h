#pragma once

// Copyright (C) Force67 2019

#include <ELFTypes.h>
#include <SCETypes.h>

#include "kernel/Process.h"

namespace utl {
	class File;
}

namespace modules
{
	class elfLoader
	{
	public:
		explicit elfLoader(std::unique_ptr<uint8_t[]>);
		bool loadinProc(krnl::Process&);

		static bool canLoad(utl::File&);

	private:
		std::unique_ptr<uint8_t[]> data;

		void doDynamics();
		void logDbgInfo();
		void installEHFrame();
		bool SetupTLS(krnl::Process&);
		bool MapImage(krnl::Process&);
		bool ResolveImports();
		bool ProcessRelocations();

		struct table
		{
			char* ptr;
			size_t size;
		};

		struct impLib
		{
			const char* name;
			int32_t modid;
		};
		std::vector<impLib> implibs;

		// headers
		ELFHeader* elf;
		ELFPgHeader* segments;
		ElfRel* jmpslots;
		ElfRel* rela;
		ElfSym* symbols;

		table strtab;
		table symtab;
		table dynld;

		uint32_t numJmpSlots;
		uint32_t numSymbols;
		uint32_t numRela;
		uint8_t* targetbase;

		uint16_t tlsslot;
		uint32_t totalimage;

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
	};
}