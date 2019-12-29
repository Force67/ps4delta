#pragma once

// Copyright (C) Force67 2019

#include <base.h>

namespace krnl
{
	enum mprotFlags : uint32_t {
		none,
		exec = 1,
		write = 2,
		read = 4
	};

	enum mFlags : uint32_t {
		fixed = 0x10,
		stack = 0x400,
		noextend = 0x100,
		anon = 0x1000,
	};

	uint8_t* PS4ABI sys_mmap(void* addr, size_t size, uint32_t prot, uint32_t flags, uint32_t fd, size_t offset);
	int PS4ABI sys_mname(uint8_t*, size_t len, const char* name, void*);
}