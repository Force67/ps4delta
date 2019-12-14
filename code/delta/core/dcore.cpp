
// Copyright (C) Force67 2019

#include <cstdio>
#include <thread>

#include "dcore.h"
#include <logger/logger.h>
#include <utl/File.h>

#include "vfs/pup_reader.h"

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

void deltaCore::boot(std::string& dir)
{
	vfs::pupReader read(R"(C:\Users\vince\Desktop\PUP\PS4UPDATE1.PUP.dec)");
	read.load();

	return;

	// sanitize path
	std::replace(dir.begin(), dir.end(), '/', '\\');

	std::thread ctx([&]() {
		auto proc = std::make_unique<krnl::proc>();
		if (!proc->create(dir))
			return;

		proc->start();
	});

	ctx.detach();
}