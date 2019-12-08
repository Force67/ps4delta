
// Copyright (C) Force67 2019

#include <cstdio>

#include "dcore.h"

#include "modules/elfLoader.h"

#include <logger/logger.h>
#include <utl/File.h>

#include <modules/vmodLinker.h>

deltaCore::deltaCore(int& argc, char** argv) :
	QApplication(argc, argv)
{
	setApplicationName(rsc_productname);
	setApplicationVersion(rsc_productversion);
}

bool deltaCore::init()
{
	LOG_INFO("Initializing deltaCore " rsc_copyright);

	if (!headless)
		window = std::make_unique<mainWindow>(*this);

	if (window)
		window->init();

	return true;
}

#include <thread>

void deltaCore::boot(const std::string& fromdir)
{
	std::thread ctx([&]() {
		//auto t = std::time(nullptr);
//LOG_INFO("Starting " FXNAME " on {:%Y-%m-%d}", *std::localtime(&t));
		utl::File inFile(R"(C:\Users\vince\Desktop\LIMBO\CUSA01369\eboot.bin-decrypted)");

		auto data = std::make_unique<uint8_t[]>(inFile.GetSize());
		inFile.Read(data.get(), inFile.GetSize());
		modules::initVMods();
		// create the process
		proc = std::make_unique<krnl::Process>();
		proc->RegisterModuleNotifaction([](krnl::ModuleHandle& mod)
			{
				LOG_INFO("Delta: Module {} loaded at {}\n", mod->name, reinterpret_cast<uintptr_t>(mod->base));
			});

		modules::elfLoader ldr(std::move(data));
		ldr.loadinProc(*proc);

		proc->Start({});
	});

	ctx.detach();
}