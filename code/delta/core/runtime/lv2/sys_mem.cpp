
// Copyright (C) Force67 2019

#include <base.h>
#include <logger/logger.h>

#include "kernel/proc.h"
#include "sys_mem.h"

namespace runtime 
{
	using namespace krnl;

	uint8_t* PS4ABI sys_mmap(void* addr, size_t len, mprotFlags prot, mFlags flags, uint32_t fd, size_t offset)
	{
		if (fd != -1)
			__debugbreak(); // TODO: object table

		/*should we allocate aligned?*/

		auto vma = proc::getActive()->getVma();
		uint8_t* ptr = vma.mapRtMemory(len);
		if (!ptr)
			return reinterpret_cast<uint8_t*>(-1);

		std::printf("mmap: addr=%p, len=%I64x, allocated @ %p\n", addr, len, ptr);
	
		/*ensure memory is zero'd*/
		if (flags & mFlags::anon)
			std::memset(ptr, 0, len);

		if (flags & mFlags::stack)
			return &static_cast<uint8_t*>(ptr)[len];

		return ptr;
	}

	int PS4ABI sys_mname(uint8_t*, size_t len, const char* name, void*)
	{
		std::printf("Sys_mname : %s\n", name);
		return 0;
	}
}