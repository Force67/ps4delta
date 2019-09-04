
// Copyright (C) Force67 2019

#include <kernel/Process.h>

namespace krnl
{
	Process::Process() :
		creationStamp(std::chrono::steady_clock::now())
	{
	}

	void Process::Start()
	{
		// HACK HACK HACK
		for (auto& e : modulelist) {
			((void(*)(void))e->entry)();
		}
	}

	void Process::RegisterModule(ModuleHandle handle)
	{
		// fire module load callbacks
		for (auto& cb : modulenotification) {
			cb(handle);
		}

		modulelist.emplace_back(handle);
	}

	void Process::RegisterModuleNotifaction(ModuleCallback cb)
	{
		modulenotification.push_back(cb);
	}
}