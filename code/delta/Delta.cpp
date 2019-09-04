
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
	// create the process
	proc = std::make_unique<krnl::Process>();
	proc->RegisterModuleNotifaction([](krnl::ModuleHandle& mod)
	{
		std::printf("Delta: Module %s loaded at %p\n", mod->name.c_str(), mod->base);
	});

	// initialize with correct loader type
	auto loader = loaders::CreateLoader(fromdir);
	if (loader) {

		if (loader->Load(*proc)
			!= loaders::LoadErrorCode::SUCCESS) {

			// todo: cry and abort

			__debugbreak();
			return;
		}

		std::puts("Delta: starting Process!");
		proc->Start();
	}
	else {
		std::puts("[!] Unknown file type");
	}
}