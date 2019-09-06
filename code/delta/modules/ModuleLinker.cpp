
// Copyright (C) Force67 2019

#include "ModuleLinker.h"
#include <intrin.h>
#include <xbyak/xbyak.h>

#include <Windows.h>

extern void init_libc();
extern void init_libkernel();

namespace mlink
{
	class SymStub : public Xbyak::CodeGenerator
	{
		const char* name;
		const char* lib;
		const void* func;

		static void PrintName(const char* str, const char *lib)
		{
			char buf[256]{};
			snprintf(buf, _countof(buf), "Invoking %s!%s\n", lib, str);

			OutputDebugStringA(buf);
		}

	public:

		SymStub(const void* addr, const char *lib, const char* name) :
			func(addr),
			lib(lib),
			name(name)
		{
			push(rcx);
			push(rdx);
			push(r8);
			push(r9);

			sub(rsp, 0x28);

			mov(rcx, (uint64_t)name);
			mov(rdx, (uint64_t)lib);
			mov(rax, (uint64_t)PrintName);

			call(rax);

			add(rsp, 0x28);

			pop(r9);
			pop(r8);
			pop(rdx);
			pop(rcx);

			mov(rax, (uint64_t)func);
			jmp(rax);
		}
	};

	static std::vector<const ModuleInfo*> g_tables;

	void register_module(const ModuleInfo* info) {
		g_tables.push_back(info);
	}

	void init_modules()
	{
		init_libc();
		init_libkernel();

		// and load the cache
	}

	static void debugstub() {
		std::printf("DEBUGSTUB: %p\n", _ReturnAddress());
		__debugbreak();
	}

	uintptr_t get_import(const char* lib, const char* symm) {

		const ModuleInfo* table = nullptr;

		// find the right table
		for (const auto &t : g_tables) {
			if (std::strcmp(lib, t->name) == 0) {
				table = t;
				break;
			}
		}

		auto wrap = [&](const void *addr, const char *name)
		{
#ifdef _DEBUG
			auto* stub = new SymStub(addr, table ? table->name : "unresolved", name);
			return reinterpret_cast<uintptr_t>(stub->getCode<void*>());
#else
			return reinterpret_cast<uintptr_t>(addr);
#endif
		};

		if (table) {
			// search the table
			for (int i = 0; i < table->funccount; i++) {
				auto* f = &table->functions[i];
				if (std::strcmp(symm, f->name) == 0) {
					return wrap(f->address, f->name);
				}
			}
		}

		return wrap(&debugstub, symm);
	}
}