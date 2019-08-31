#pragma once

// Copyright (C) Force67 2019

#include <cstdint>
#include <vector>

namespace krnl
{
	class VMAccessMgr
	{
		struct VmHeapInfo 
		{
			void* addr;
			uint32_t size;
		};

		std::vector<VmHeapInfo> heapInfos;
		size_t heapUsedTotal;

	public:

		VMAccessMgr();

		void* AllocateSeg(uint32_t);
	};
}