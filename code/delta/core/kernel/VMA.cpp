
// Copyright (C) Force67 2019

#include <kernel/VMA.h>
#include <Windows.h>

namespace krnl
{
	VMAccessMgr::VMAccessMgr() :
		heapUsedTotal(0)
	{}

	void* VMAccessMgr::AllocateSeg(uint32_t size)
	{
		heapUsedTotal += size;

		void *phaddr = VirtualAlloc(nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

		auto& info = heapInfos.emplace_back();
		info.size = size;
		info.addr = phaddr;

		return phaddr;
	}
}