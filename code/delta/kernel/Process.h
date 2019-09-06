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
		uint8_t* base;
		uint8_t* entry;
		uint16_t tlsSlot;
		size_t sizeCode;
		//todo: store section pointers
	};

	using ModuleHandle = std::shared_ptr<Module>;
	using ModuleCallback = void(*)(ModuleHandle&);

	class Process
	{
		// the virtual memory owned by the process
		VMAccessMgr vmem;

		// the owned modules
		uint16_t tlsSlots;
		std::vector<ModuleHandle> modulelist;
		std::vector<ModuleCallback> modulenotification;

		std::chrono::steady_clock::time_point creationStamp;

	public:

		Process();
		void Start(const std::vector<std::string>&);

		// like dlls
		void RegisterModule(ModuleHandle);
		ModuleHandle GetModule(const std::string &name);
		void RegisterModuleNotifaction(ModuleCallback);

		inline VMAccessMgr& GetVirtualMemory() {
			return vmem;
		}

		inline ModuleHandle& GetMainModule() {
			return modulelist[0];
		}

		inline uint16_t GetNextFreeTls() {
			return tlsSlots;
		}
	};
}