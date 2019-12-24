#pragma once

// Copyright (C) Force67 2019

#include <memory>
#include <vector>

#include "module.h"
#include "vm_manager.h"
#include <utl/object_ref.h>

namespace krnl
{
	struct procInfo
	{
		uint32_t ripZoneSize = 5 * 1024;
		uint8_t* userStack = nullptr;
		size_t userStackSize = 20 * 1024 * 1024;
		void* fsBase = nullptr;
	};

	class proc
	{
		friend class elfModule;
	public:
		using moduleList = std::vector<std::shared_ptr<elfModule>>;

		proc();
		bool create(const std::string&);
		void start();

		static proc* getActive();

		inline std::shared_ptr<elfModule> getMainModule() { return modules[0]; }
		inline moduleList& getModuleList() { return modules; }

		std::shared_ptr<elfModule> loadModule(std::string_view);
		std::shared_ptr<elfModule> getModule(std::string_view);
		std::shared_ptr<elfModule> getModule(uint32_t);

		inline vmManager& getVma() { return vmem; }
		inline procInfo& getEnv() { return env; }

	private:
		vmManager vmem;
		procInfo env;
		moduleList modules;
		uint32_t handleCounter = 1;

		uint32_t nextFreeTLS() {
			return -1;
		}
	};
}