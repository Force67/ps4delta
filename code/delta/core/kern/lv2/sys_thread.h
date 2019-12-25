#pragma once

// Copyright (C) Force67 2019

#include <base.h>

namespace krnl
{
	int PS4ABI sys_thr_self(void**);
	int PS4ABI sys_rtprio_thread(int, uint64_t, void*);
	int PS4ABI sys_umtx_op(void*, int, uint32_t, void*, void*);
}