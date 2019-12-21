
// Copyright (C) Force67 2019

#include <string>
#include <base.h>

#include <intrin.h>

#include "kernel/proc.h"

namespace runtime
{
	using namespace krnl;

	int PS4ABI sys_thr_self(void** arg)
	{
		*arg = (void*)357;
		return 135;
	}
}