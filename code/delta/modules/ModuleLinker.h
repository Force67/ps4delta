#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <unordered_map>

namespace mlink
{
	struct FunctionInfo
	{
		const int32_t index;
		const void* address;
		const char* name;

		FunctionInfo(const int32_t index, const void *handler, const char *name) :
			index(index),
			address(handler),
			name(name)
		{}
	};

	struct ModuleInfo
	{
		FunctionInfo *functions;
		size_t funccount;
		const char* name;
	};

	void init_modules();
	void register_module(const ModuleInfo*);

	uintptr_t get_import(const char *lib, const char *sym);
}

#define MODULE_INIT(tname) \
void init_##tname() { \
static mlink::ModuleInfo info{ \
(mlink::FunctionInfo*)&functions, (sizeof(functions) / sizeof(mlink::FunctionInfo)), #tname}; \
mlink::register_module(&info); }
