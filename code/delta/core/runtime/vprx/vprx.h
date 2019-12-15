#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <utl/InitFunction.h>
#include <logger/logger.h>

#include <base.h>

namespace runtime
{
	struct funcInfo {
		uint64_t hashId;
		const void* address;
	};

	struct modInfo {
		funcInfo* funcNodes;
		size_t funcCount;
		const char* namePtr;
	};

	void vprx_init();
	void vprx_reg(const modInfo*);
	uintptr_t vprx_get(const char* lib, uint64_t hid);

	bool decode_nid(const char* subset, size_t len, uint64_t&);
}

#define MODULE_INIT(tname) \
static const runtime::modInfo info_##tname{(runtime::funcInfo*)&functions, (sizeof(functions) / sizeof(runtime::funcInfo)), #tname}; \
static utl::init_function init_##tname([](){ runtime::vprx_reg(&info_##tname); })