#pragma once

// Copyright (C) Force67 2019

#include <base.h>

namespace runtime
{
	enum mprotFlags : uint32_t {
		none,
		exec = 1,
		write = 2,
		read = 4,
		all = read | write | exec,
	};

	enum mFlags : uint32_t {
		stack = 0x400,
		anon = 0x1000,
	};

	uint8_t* PS4ABI sys_mmap(void* addr, size_t len, mprotFlags prot, mFlags flags, uint32_t fd, size_t offset);
	int PS4ABI sys_mname(uint8_t*, size_t len, const char* name, void*);
}