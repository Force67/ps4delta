
// Copyright (C) Force67 2019

#include <kernel/Process.h>

namespace krnl
{
	Process::Process() :
		creationStamp(std::chrono::steady_clock::now())
	{}

	void Process::Start()
	{
		if (modulelist.size() == 0) {
			//TODO: assert
		}

		auto& main = GetMainModule();

		static struct InitModule
		{
			int64_t ofs = 0;
			const char* commandline = "DUMMY";
		} g_module;
	
		// the other 3 are just some bs...
		((void(*)(InitModule*,__int64,__int64,__int64))main->entry)(&g_module,0,0,0);
	}

	void Process::RegisterModule(ModuleHandle handle)
	{
		// fire module load callbacks
		for (auto& cb : modulenotification) {
			cb(handle);
		}

		modulelist.emplace_back(handle);
	}

	void Process::DumpModule(const std::string &name)
	{
		for (auto& e : modulelist) {
			if (e->name == name) {
			
			}
		}
	}

	void Process::RegisterModuleNotifaction(ModuleCallback cb)
	{
		modulenotification.push_back(cb);
	}
}