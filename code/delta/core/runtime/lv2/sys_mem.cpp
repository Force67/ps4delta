
// Copyright (C) Force67 2019

#include <base.h>
#include <utl/mem.h>
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

		/*TODO: should we allocate aligned?*/

		auto &vma = proc::getActive()->getVma();
		uint8_t *ptr = vma.mapMemory(static_cast<uint8_t*>(addr), len, utl::pageProtection::w);
		if (!ptr) {
			__debugbreak();
			return reinterpret_cast<uint8_t*>(-1);
		}

		auto tprot = utl::pageProtection::r;
		if (prot & mprotFlags::write)
			tprot |= utl::pageProtection::w;
		if (prot & mprotFlags::exec)
			tprot |= utl::pageProtection::x;

		if (flags & mFlags::anon)
			std::memset(ptr, 0, len);

		/*we apply target protection later so we can setup the memory
		properly beforehand*/
		utl::protectMem(static_cast<void*>(ptr), len, tprot);

		std::printf("mmap: addr=%p, len=%I64x, allocated @ %p\n", addr, len, ptr);

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