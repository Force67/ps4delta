
// Copyright (C) Force67 2019

#include <logger/logger.h>
#include <runtime/vprx/vprx.h>

#include "../proc.h"
#include "error_table.h"

namespace krnl
{
	int PS4ABI sys_open(const char* path, uint32_t flags, uint32_t mode)
	{
		if (!path)
			return SysError::eINVAL;

		if (std::strncmp(path, "/dev/", 5) == 0) {

		}


		std::printf("open: %s, %x, %x\n", path, flags, mode);
		__debugbreak();
		return 0;
	}
}