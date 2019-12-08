
// Copyright (C) Force67 2019

#include "vmodLinker.h"
#include <intrin.h>
#include <xbyak/xbyak.h>

#include <Windows.h>

namespace modules
{
#if 1
	class SymStub : public Xbyak::CodeGenerator
	{
		const char* name;
		const char* lib;
		const void* func;

		static void PrintName(const char* lib, const char *fun)
		{
			char buf[256]{};
			snprintf(buf, _countof(buf), "Invoking %s!%s\n", lib, fun);

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

			mov(rcx, (uint64_t)lib);
			mov(rdx, (uint64_t)name);
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
#endif

	static std::vector<const modInfo*> g_tables;

	void registerModule(const modInfo* info) {
		g_tables.push_back(info);
	}

	void initVMods()
	{
		// register modules
		utl::init_function::init();

		// and load the cache
	}

	static void debugstub() {
		std::printf("DEBUGSTUB: %p\n", _ReturnAddress());
		__debugbreak();
	}

	// base64 fast lookup
	bool decodeNid(const char* subset, size_t len, uint64_t &out)
	{
		const char lookup[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

		for (size_t i = 0; i < len; i++) {
			auto pos = std::strchr(lookup, subset[i]);

			// invalid NID?
			if (!pos) {
				return false;
			}

			auto offset = static_cast<uint32_t>(pos - lookup);

			// max NID is 11
			if (i < 10) {
				out <<= 6;
				out |= offset;
			}
			else {
				out <<= 4;
				out |= (offset >> 2);
			}
		}

		return true;
	}

	uintptr_t getImport(const char* lib, uint64_t hid) {
		const modInfo* table = nullptr;

		// find the right table
		for (const auto &t : g_tables) {
			if (std::strcmp(lib, t->namePtr) == 0) {
				table = t;
				break;
			}
		}

		auto wrap = [&](const void *addr, const char *name)
		{
#ifdef _DEBUG
			auto* stub = new SymStub(addr, table ? table->namePtr : "unresolved", name);
			return reinterpret_cast<uintptr_t>(stub->getCode<void*>());
#else
			return reinterpret_cast<uintptr_t>(addr);
#endif
		};

		if (table) {
			// search the table
			for (int i = 0; i < table->funcCount; i++) {
				auto* f = &table->funcNodes[i];
				if (f->hashId == hid) {
					return reinterpret_cast<uintptr_t>(f->address);
				}
			}
		}

		return 0;
		//return  reinterpret_cast<uintptr_t>(&debugstub);
	}
}