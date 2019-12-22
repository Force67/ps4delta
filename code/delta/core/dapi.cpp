
// Copyright (C) 2019 Force67

#include <base.h>
#include <utl/mem.h>
#include <logger/logger.h>

#include "dcore.h"

static bool verifyViablity()
{
	constexpr size_t one_mb = 1024ull * 1024ull;
	constexpr size_t eight_gb = 8ull * 1024ull * one_mb;

	if (utl::getAvailableMem() < eight_gb) {
		LOG_ERROR("Your system doesn't have enough physical memory to run " FXNAME);
		return false;
	}

	return true;
}

EXPORT int dcoreMain(int argc, char** argv)
{
	utl::createLogger(true);

	if (!verifyViablity())
		return -1;

	/*if (argc < 2) {
		LOG_ERROR("Please supply ELF path");
		return -2;
	}*/

	auto app = std::make_unique<deltaCore>(argc, argv);
	if (app) {
		if (app->init()) {
			std::string temp(R"(C:\Users\vince\Desktop\Eboot_Give\Tetris_eboot.bin)");
			app->boot(temp);

			// enter event loop
			return app->exec();
		}
	}

	return 0;
}