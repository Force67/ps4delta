#pragma once

// Copyright (C) Force67 2019

#include <chrono>
#include <memory>
#include <string>
#include <kernel/VMA.h>

namespace krnl
{
	struct kObj
	{
		std::string name;
		uint32_t type;
		uint32_t handle;
		uint8_t* base;
		uint8_t* entry;
		uint16_t tlsSlot;
		uint32_t codeSize;
	};

	struct procEnv
	{
		bool ripZoneEnabled{ true };
		uint32_t ripZoneSize = 1024;
	};

	class proc
	{
		friend class elfModule;
	public:
		using moduleList = std::vector<std::unique_ptr<kObj>>;

		proc();
		void addObj(std::unique_ptr<kObj>);
		bool create(const std::string&);
		void start();

		static proc* getActive();

		inline kObj& getMainModule() { return *modules[0]; }
		inline moduleList& getModuleList() { return modules; }

		kObj* loadModule(std::string_view);
		kObj* getModule(std::string_view);
		kObj* getModule(uint32_t);

	private:
		VMAccessMgr vmem;
		procEnv env;
		uint32_t handleCounter = 1;

		uint32_t nextFreeTLS() {
			return -1;
		}

		moduleList modules;
	};
}