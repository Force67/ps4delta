
// Copyright (C) Force67 2019

#include "dcore.h"
#include <loader/Loader.h>
#include <modules/ModuleLinker.h>

#include <logger/logger.h>

deltaCore::deltaCore(int& argc, char** argv) :
	QApplication(argc, argv)
{
	QApplication::setApplicationName(FXNAME);
}

void deltaCore::boot(const std::string& fromdir)
{
	LOG_INFO("Booting {}", "TEST");

	// create the process
	proc = std::make_unique<krnl::Process>();
	proc->RegisterModuleNotifaction([](krnl::ModuleHandle& mod)
	{
		LOG_INFO("Delta: Module %s loaded at 0x%llx\n", mod->name.c_str(), reinterpret_cast<uintptr_t>(mod->base));
	});

#if 0
	// initialize with correct loader type
	auto loader = loaders::CreateLoader(fromdir);
	if (loader) {

		// init superhle module linker
		mlink::init_modules();

		auto ec = loader->Load(*proc);
		if (ec != loaders::LoadErrorCode::SUCCESS) {

			// todo: cry and abort
			LOG_ERROR("Error code %d\n", (int)ec);

			__debugbreak();
			return;
		}

		LOG_INFO("Delta: starting Process!");

		std::vector<std::string> args;
		args.push_back("");
		args.push_back("");
		args.push_back("");
		args.push_back("");
		proc->Start(args);
	}
	else {
		LOG_ERROR("[!] Unknown file type");
	}
#endif
}