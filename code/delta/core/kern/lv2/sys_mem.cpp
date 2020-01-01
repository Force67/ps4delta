
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <utl/mem.h>
#include <logger/logger.h>

#include "../proc.h"
#include "sys_mem.h"
#include "error_table.h"

namespace krnl 
{
	using ppt = utl::pageProtection;
	using alt = utl::allocationType;

	uint8_t* PS4ABI sys_mmap(void* addr, size_t size, uint32_t prot, uint32_t flags, uint32_t fd, size_t offset)
	{
		auto* proc = proc::getActive();
		if (!proc)
			return reinterpret_cast<uint8_t*>(-1);

		if (flags & mFlags::stack || flags & mFlags::noextend)
			flags |= mFlags::anon;

		if (flags & mFlags::fixed) {
			__debugbreak();
		}
		else if (!addr) {
			__debugbreak();
			addr = reinterpret_cast<void*>(0x200000000);
		}

		/*align the page*/
		size = (size + 0x3FFF) & 0xFFFFFFFFFFFFC000LL;

		if (fd > -1) {
			auto* obj = proc::getActive()->getObjTable().get(fd);
			return static_cast<device*>(obj)->map(addr, size, prot, flags, offset);
		}

		void* ptr = utl::allocMem(addr, size, ppt::w, alt::reservecommit);
		if (!ptr) {
			// maybe a previously reserved page?
			ptr = utl::allocMem(addr, size, ppt::w, alt::commit);
			if (!ptr && !(flags & 0x10)) {
				ptr = utl::allocMem(nullptr, size, ppt::w, alt::reservecommit);
				if (!ptr) {
					return reinterpret_cast<uint8_t*>(-1);
				}
			}
		}

		auto tprot = ppt::r;
		if (prot & mprotFlags::write)
			tprot |= ppt::w;
		if (prot & mprotFlags::exec)
			tprot |= ppt::x;

		if (flags & mFlags::anon)
			std::memset(ptr, 0, size);

		proc->getVma().add(static_cast<uint8_t*>(ptr), size, tprot);

		// now we apply target protection
		utl::protectMem(static_cast<void*>(ptr), size, tprot);

		std::printf("mmap %p, %x, %p\n", addr, size, _ReturnAddress());
		//LOG_WARNING("addr={}, len={}, requested by {}", fmt::ptr(addr), len, fmt::ptr(_ReturnAddress()));

		if (flags & mFlags::stack)
			return &static_cast<uint8_t*>(ptr)[size];

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

		LOG_WARNING("tagged {} with name {}", fmt::ptr(ptr), name);
		info->name = name;
		return 0;
	}
}