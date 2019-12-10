
// Copyright (C) Force67 2019

#include <logger/logger.h>

namespace runtime 
{
	int sys_dynlib_process_needed_and_relocate()
	{
		std::puts("rel proc?");

		// we always return success here as we
		// automatically relocate everything on load
		return 0;
	}
}