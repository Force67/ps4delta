#pragma once

// Copyright (C) Force67 2019

#include <chrono>
#include <kernel/VMA.h>

namespace krnl
{
	class Process
	{
		// the virtual memory owned by the process
		VMAccessMgr vmem;

		// proc creation time
		std::chrono::steady_clock::time_point creationStamp;

	public:

		Process();

		void Start();

		inline VMAccessMgr& GetVirtualMemory() {
			return vmem;
		}
	};
}