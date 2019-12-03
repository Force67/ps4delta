
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
	utl::createLogger();

	if (argc < 2) {
		LOG_ERROR("Please supply ELF path");
		return -2;
	}

	auto app = std::make_unique<deltaCore>(argc, argv);
	app->boot(argv[1]);

	getchar();
	return 0;
}