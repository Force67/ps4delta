
// Copyright (C) Force67 2019

#include <string>
#include <base.h>

#include <intrin.h>

#include "kernel/proc.h"

namespace runtime
{
	using namespace krnl;

	int PS4ABI sys_mdbg_service(uint32_t op, void*, void*)
	{
		__debugbreak();
		return 0;
	}
}