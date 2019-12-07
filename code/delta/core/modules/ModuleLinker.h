#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <utl/InitFunction.h>

namespace mlink
{
	struct FunctionInfo
	{
		uint64_t hid;
		const void* address;
	};

	struct ModuleInfo
	{
		FunctionInfo *functions;
		size_t fcount;
		const char* name;
	};

	void init_modules();
	void register_module(const ModuleInfo*);

	bool decode_nid(const char* subset, size_t len, uint64_t&);
	uintptr_t get_import(const char *lib, uint64_t hid);
}

#define MODULE_INIT(tname) \
static const mlink::ModuleInfo info_##tname{(mlink::FunctionInfo*)&functions, (sizeof(functions) / sizeof(mlink::FunctionInfo)), #tname}; \
static utl::init_function init_##tname([](){ mlink::register_module(&info_##tname); })

#define UNIMPLEMENTED_FUNC 