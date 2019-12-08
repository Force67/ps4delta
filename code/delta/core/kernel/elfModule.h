#pragma once

// Copyright (C) Force67 2019

#include <ELFTypes.h>
#include <SCETypes.h>
#include "proc.h"

namespace utl {
	class File;
}

namespace krnl
{
	class elfModule final : public kObj
	{
		friend class proc;
	public:
		explicit elfModule(proc*);
		bool fromFile(const std::string&);

	private:
		//bool loadInternal();
		bool initSegments();
		void doDynamics();
		void logDbgInfo();
		void installEHFrame();
		bool setupTLS();
		bool mapImage();
		bool resolveImports();
		bool applyRelocations();

		template<typename Type, typename TAdd>
		Type* getOffset(const TAdd dist) {
			return (Type*)(data.get() + dist);
		}

		template<typename Type, typename TAdd>
		Type* getAddress(const TAdd dist) {
			return (Type*)(base + dist);
		}

		template<typename Type = ELFPgHeader>
		Type* getSegment(ElfSegType type) {
			for (uint16_t i = 0; i < elf->phnum; i++) {
				auto s = &segments[i];
				if (s->type == type)
					return reinterpret_cast<Type*>(s);
			}

			return nullptr;
		}
	private:
		proc* owner;
		std::unique_ptr<uint8_t[]> data;

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

		uint32_t codeSize;
	};
}