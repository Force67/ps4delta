#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <utl/InitFunction.h>
#include <logger/logger.h>

#define PS4ABI __attribute__((sysv_abi))

namespace modules
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

	void initVMods();
	void registerModule(const modInfo*);

	bool decodeNid(const char* subset, size_t len, uint64_t&);
	uintptr_t getImport(const char* lib, uint64_t hid);
}

#define MODULE_INIT(tname) \
static const modules::modInfo info_##tname{(modules::funcInfo*)&functions, (sizeof(functions) / sizeof(modules::funcInfo)), #tname}; \
static utl::init_function init_##tname([](){ modules::registerModule(&info_##tname); })