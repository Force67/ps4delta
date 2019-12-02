
// Copyright (C) Force67 2019

#include <Delta.h>
#include <loader/Loader.h>
#include <modules/ModuleLinker.h>

#include <logger/logger.h>

delta& delta::get() {
	static delta s_instance;
	return s_instance;
}

void delta::boot(const std::wstring& fromdir)
{
	utl::createLogger(L"delta.log");
	LOG_INFO("Booting {}", "TEST");

	// create the process
	proc = std::make_unique<krnl::Process>();
	proc->RegisterModuleNotifaction([](krnl::ModuleHandle& mod)
	{
		LOG_INFO("Delta: Module %s loaded at 0x%llx\n", mod->name.c_str(), reinterpret_cast<uintptr_t>(mod->base));
	});

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
}