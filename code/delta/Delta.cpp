
// Copyright (C) Force67 2019

#include <Delta.h>
#include <loader/Loader.h>
#include <modules/Module.h>

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
		std::printf("Delta: Module %s loaded at 0x%llx\n", mod->name.c_str(), reinterpret_cast<uintptr_t>(mod->base));
	});

	// initialize with correct loader type
	auto loader = loaders::CreateLoader(fromdir);
	if (loader) {

		// init module store
		modules::init_modules();

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