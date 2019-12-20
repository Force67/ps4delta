
// Copyright (C) Force67 2019

#include <logger/logger.h>
#include <base.h>

#include <kernel/proc.h>

namespace runtime 
{
	int PS4ABI sys_mmap(void* addr, size_t len, uint32_t prot, uint32_t flags, uint32_t fd, size_t offset)
	{
		std::printf("mmap: addr=%p, len=%I64x, prot=%x, flags=%x, fd=%d, offset=%I64x", addr, len, prot, flags, fd, offset);
		return 0;
	}
}