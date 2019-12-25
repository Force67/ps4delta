
// Copyright (C) Force67 2019

#include <base.h>
#include <utl/mem.h>
#include <logger/logger.h>

#include "../proc.h"
#include "sys_mem.h"

namespace krnl 
{
	using ppt = utl::pageProtection;
	using alt = utl::allocationType;

	uint8_t* PS4ABI sys_mmap(void* addr, size_t len, mprotFlags prot, mFlags flags, uint32_t fd, size_t offset)
	{
		if (fd != -1)
			__debugbreak(); // TODO: object table

		auto* proc = proc::getActive();
		if (!proc)
			return reinterpret_cast<uint8_t*>(-1);

		/*TODO: should we allocate aligned?*/

		void* ptr = utl::allocMem(addr, len, ppt::w, alt::reservecommit);
		if (!ptr) {

			// maybe a previously reserved page?
			ptr = utl::allocMem(addr, len, ppt::w, alt::commit);
			if (!ptr) {
				__debugbreak();
				return reinterpret_cast<uint8_t*>(-1);
			}
		}

		auto tprot = ppt::r;
		if (prot & mprotFlags::write)
			tprot |= ppt::w;
		if (prot & mprotFlags::exec)
			tprot |= ppt::x;

		if (flags & mFlags::anon)
			std::memset(ptr, 0, len);

		proc->getVma().add(static_cast<uint8_t*>(ptr), len, tprot);

		// now we apply target protection
		utl::protectMem(static_cast<void*>(ptr), len, tprot);

		LOG_TRACE("addr={}, len={}, requested by {}", fmt::ptr(addr), len, fmt::ptr(_ReturnAddress()));

		if (flags & mFlags::stack)
			return &static_cast<uint8_t*>(ptr)[len];

		return static_cast<uint8_t*>(ptr);
	}

	int PS4ABI sys_mname(uint8_t* ptr, size_t len, const char* name, void*)
	{
		auto* proc = proc::getActive();
		if (!proc)
			return -1;

		auto* info = proc->getVma().get(ptr);
		if (!info) {
			LOG_WARNING("attempted to tag unknown memory ({}, {})", fmt::ptr(ptr), name);
			return -1;
		}

		LOG_TRACE("tagged {} with name {}", fmt::ptr(ptr), name);
		info->name = name;
		return 0;
	}
}