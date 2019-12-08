
// Copyright (C) 2019 Force67

// temp shit
#ifdef _WIN32
#define EXP extern "C" __declspec(dllexport)
#endif

#include "dcore.h"
#include <logger/logger.h>

// temp exposed
EXP int dcoreMain(int argc, char** argv)
{
	utl::createLogger(true);

	/*if (argc < 2) {
		LOG_ERROR("Please supply ELF path");
		return -2;
	}*/

	auto app = std::make_unique<deltaCore>(argc, argv);
	if (app) {
		if (app->init()) {
			//app->boot(argv[1]);

			// enter event loop
			return app->exec();
		}
	}

	return 0;
}