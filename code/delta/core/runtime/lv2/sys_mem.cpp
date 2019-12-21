
// Copyright (C) Force67 2019

#include <base.h>
#include <utl/mem.h>
#include <logger/logger.h>

#include "kernel/proc.h"
#include "sys_mem.h"

namespace runtime 
{
	using namespace krnl;

	using ppt = utl::pageProtection;
	using alt = utl::allocationType;

	uint8_t* PS4ABI sys_mmap(void* addr, size_t len, mprotFlags prot, mFlags flags, uint32_t fd, size_t offset)
	{
		if (fd != -1)
			__debugbreak(); // TODO: object table

		/*TODO: should we allocate aligned?*/

		/*attempt to allocate in one batch*/
		void* ptr = utl::allocMem(addr, len, ppt::w, alt::reservecommit);
		if (!ptr) {

			/*maybe we try to allocate in a previously reserved region?*/
			ptr = utl::allocMem(addr, len, ppt::w, alt::commit);
			if (!ptr) {
				__debugbreak();
				return reinterpret_cast<uint8_t*>(-1);
			}
		}

		/*todo: register with mem mng*/

		auto tprot = ppt::r;
		if (prot & mprotFlags::write)
			tprot |= ppt::w;
		if (prot & mprotFlags::exec)
			tprot |= ppt::x;

		if (flags & mFlags::anon)
			std::memset(ptr, 0, len);

		/*we apply target protection later so we can setup the memory
		properly beforehand*/
		utl::protectMem(static_cast<void*>(ptr), len, tprot);

		std::printf("mmap: addr=%p, len=%I64x, allocated @ %p\n", addr, len, ptr);

		if (flags & mFlags::stack)
			return &static_cast<uint8_t*>(ptr)[len];

		return static_cast<uint8_t*>(ptr);
	}

	int PS4ABI sys_mname(uint8_t*, size_t len, const char* name, void*)
	{
		std::printf("Sys_mname : %s\n", name);
		return 0;
	}
}