
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <string>
#include <base.h>

#include <intrin.h>

namespace krnl
{
	int PS4ABI sys_mdbg_service(uint32_t op, void*, void*)
	{
		__debugbreak();
		return 0;
	}
}