
// Copyright (C) Force67 2019

#include <logger/logger.h>
#include "../vprx/vprx.h"

namespace runtime
{
	int PS4ABI sys_mdbg_service()
	{
		__debugbreak();
		return 0;
	}

	int PS4ABI sys_sysctl(const int* name, uint32_t namelen, void* oldp, size_t* oldlenp, const void* newp, size_t newlen)
	{
		return 0;
	}

	int PS4ABI sys_getpid()
	{
		return 0x1337;
	}

	bool PS4ABI sys_write(uint32_t fd, const void* buf, size_t nbytes)
	{
		if (fd == 1 || fd == 2) // stdout, stderr
		{
			auto b = static_cast<const char*>(buf);
			for (size_t i = 0; i < nbytes; ++i, ++b)
			{
				printf("%c", *b);
			}
			return true;
		}

		return false;
	}
}