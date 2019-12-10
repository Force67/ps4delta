
// Copyright (C) Force67 2019

#include <logger/logger.h>

namespace runtime
{
	int sys_getpid()
	{
		std::puts("GETPID");
		return 0x1337;
	}
}