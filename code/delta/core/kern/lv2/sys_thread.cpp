
// Copyright (C) Force67 2019

#include <string>
#include <base.h>

#include <intrin.h>

#include "../proc.h"

namespace krnl
{
	int PS4ABI sys_thr_self(void** arg)
	{
		*arg = (void*)357;
		return 135;
	}

	int PS4ABI sys_rtprio_thread(int, uint64_t, void*)
	{
		return 0;
	}

	int PS4ABI sys_umtx_op(void* ptr, int, uint32_t, void*, void*)
	{
		std::printf("locking kmutex! %p\n", ptr);

		//TODO:
		return 0;
	}
}