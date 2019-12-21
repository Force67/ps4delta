#pragma once

// Copyright (C) Force67 2019

#include <base.h>

namespace runtime
{
	int PS4ABI sys_is_in_sandbox();
	int PS4ABI sys_get_authinfo(int value);
	int PS4ABI sys_sysctl(int* name, uint32_t namelen, void* oldp, size_t* oldlenp, const void* newp, size_t newlen);
}