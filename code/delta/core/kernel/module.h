#pragma once

// Copyright (C) Force67 2019

#include <ELFTypes.h>
#include <SCETypes.h>
#include "proc.h"
#include <base.h>

namespace utl {
	class File;
}

namespace krnl
{
	//PS4ABI
	using linker_function = void(*)();

	class elfModule
	{
		friend class proc;
	public:
		explicit elfModule(proc*);
		bool fromFile(const std::string&);
		bool fromMem(std::unique_ptr<uint8_t[]>);
		uintptr_t getExport(uint64_t);

		bool unload();

	public:
		std::string name;
		uint32_t type;
		uint32_t handle;
		uint8_t* base;
		uint8_t* entry;
		uint8_t* procParam{ nullptr };
		uint32_t procParamSize = 0;
		uint16_t tlsSlot;
		uint32_t codeSize;

	private:
		void doDynamics();
		void logDbgInfo();
		void installEHFrame();
		bool setupTLS();
		bool mapImage();
		bool resolveImports();
		bool applyRelocations();
		void callConstructors();
		void callDestructors();

		template<typename Type, typename TAdd>
		Type* getOffset(const TAdd dist) {
			return (Type*)(data.get() + dist);
		}

		template<typename Type, typename TAdd>
		Type* getAddress(const TAdd dist) {
			return (Type*)(base + dist);
		}

		template<typename Type, typename TAdd>
		Type getAddressNPTR(const TAdd dist) {
			return (Type)(base + dist);
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

		inline bool isSprx() {
			return elf->type == ET_SCE_DYNAMIC;
		}
	private:
		std::unique_ptr<uint8_t[]> data;

	private:
		proc* process;
		ELFHeader* elf;
		ELFPgHeader* segments;

		struct impLib
		{
			const char* name;
			int32_t modid;
		};

		std::vector<impLib> implibs;

		ElfRel* jmpslots;
		ElfRel* rela;
		ElfSym* symbols;

		struct table
		{
			char* ptr;
			size_t size;
		};

		table strtab;
		table symtab;

		uint32_t numJmpSlots;
		uint32_t numSymbols;
		uint32_t numRela;

		linker_function init_func{ nullptr };
		linker_function fini_func{ nullptr };

		linker_function* preinit_array{ nullptr };
		linker_function* init_array{ nullptr };
		linker_function* fini_array{ nullptr };

		uint32_t numPreInitArray = 0;
		uint32_t numInitArray = 0;
		uint32_t numFiniArray = 0;
	};
}