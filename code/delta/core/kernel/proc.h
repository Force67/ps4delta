#pragma once

// Copyright (C) Force67 2019

#include <memory>
#include <vector>

#include "module.h"
#include "VMA.h"
#include <utl/object_ref.h>

namespace krnl
{
	struct procEnv
	{
		bool ripZoneEnabled{ true };
		uint32_t ripZoneSize = 1024;
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

	private:
		moduleList modules;

		VMAccessMgr vmem;
		procEnv env;
		uint32_t handleCounter = 1;

		uint32_t nextFreeTLS() {
			return -1;
		}
	};
}