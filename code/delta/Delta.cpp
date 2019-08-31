
// Copyright (C) Force67 2019

#include <Delta.h>

#include <loader/Loader.h>

Delta& Delta::Get() {
	static Delta s_instance;
	return s_instance;
}

Delta::Delta()
{
}

void Delta::Boot(const std::wstring& fromdir)
{
	using namespace loaders;

	// create the process
	proc = std::make_unique<krnl::Process>();

	// initialize with correct loader type
	auto loader = loaders::CreateLoader(fromdir);
	if (loader) {

		if (loader->Load(proc->GetVirtualMemory())
			!= LoadErrorCode::SUCCESS) {

			// todo: cry and abort

			__debugbreak();
			return;
		}
	}


}