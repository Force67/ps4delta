#pragma once

// Copyright (C) Force67 2019

#include <chrono>
#include <memory>
#include <string>
#include <kernel/VMA.h>

namespace krnl
{
	struct Module
	{
		std::string name;
		uint32_t type;
		uintptr_t* base;
		size_t size;
		uintptr_t *entry;
		//todo: store section pointers
	};

	using ModuleHandle = std::shared_ptr<Module>;
	using ModuleCallback = void(*)(ModuleHandle&);

	class Process
	{
		// the virtual memory owned by the process
		VMAccessMgr vmem;

		std::vector<ModuleHandle> modulelist;
		std::vector<ModuleCallback> modulenotification;

		std::chrono::steady_clock::time_point creationStamp;

	public:

		Process();
		void Start();

		// like dlls
		void RegisterModule(ModuleHandle);
		void DumpModule(const std::string&);
		void RegisterModuleNotifaction(ModuleCallback);

		inline VMAccessMgr& GetVirtualMemory() {
			return vmem;
		}

		inline ModuleHandle& GetMainModule() {
			return modulelist[0];
		}
	};
}