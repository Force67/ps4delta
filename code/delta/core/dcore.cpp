
// Copyright (C) Force67 2019

#include <cstdio>

#include "dcore.h"

#include <loader/Loader.h>
#include <modules/ModuleLinker.h>

#include <logger/logger.h>

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
		window = std::make_unique<mainWindow>(nullptr);

	if (window)
		window->init();

	return true;
}

void deltaCore::boot(const std::string& fromdir)
{
	return;
	//auto t = std::time(nullptr);
	//LOG_INFO("Starting " FXNAME " on {:%Y-%m-%d}", *std::localtime(&t));

	// create the process
	proc = std::make_unique<krnl::Process>();
	proc->RegisterModuleNotifaction([](krnl::ModuleHandle& mod)
	{
		LOG_INFO("Delta: Module %s loaded at 0x%llx\n", mod->name.c_str(), reinterpret_cast<uintptr_t>(mod->base));
	});
}