
// Copyright (C) Force67 2019

#include <logger/logger.h>
#include "../vprx/vprx.h"

namespace runtime 
{
	int PS4ABI sys_dynlib_process_needed_and_relocate()
	{
		// we always return success here as we
		// automatically relocate everything on load
		return 0;
	}
}