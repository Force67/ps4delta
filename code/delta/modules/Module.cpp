
// Copyright (C) Force67 2019

#include "Module.h"
#include <intrin.h>

extern void init_libc();

namespace modules
{
	static std::vector<const ModuleInfo*> g_tables;

	void register_module(const ModuleInfo* info) {
		g_tables.push_back(info);
	}

	void init_modules()
	{
		init_libc();

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

		if (table) {
			// search the table
			for (int i = 0; i < table->funccount; i++) {
				auto* f = &table->functions[i];
				if (std::strcmp(symm, f->name) == 0) {
					return reinterpret_cast<uintptr_t>(f->address);
				}
			}
		}

		//std::printf("no such import table for %s!%s\n", lib, symm);
		return reinterpret_cast<uintptr_t>(&debugstub);
	}
}