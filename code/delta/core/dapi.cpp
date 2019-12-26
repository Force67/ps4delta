
// Copyright (C) 2019 Force67

#include <base.h>
#include <xbyak_util.h>
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

	bool missing_set = false;
	Xbyak::util::Cpu cpu;

#define CHECK_FEATURE(x,y) \
	if (!cpu.has(Xbyak::util::Cpu::t ## x)) \
	{ \
		LOG_ERROR("your cpu does not feature " y " instructions"); \
		missing_set = true; \
	}

	CHECK_FEATURE(SSE, "SSE");
	CHECK_FEATURE(SSE2, "SSE2");
	CHECK_FEATURE(SSE3, "SSE3");
	CHECK_FEATURE(SSSE3, "SSSE3");
	CHECK_FEATURE(SSE41, "SSE4.1");
	CHECK_FEATURE(SSE42, "SSE4.2");
	CHECK_FEATURE(AESNI, "AES");
	CHECK_FEATURE(AVX, "AVX");
	CHECK_FEATURE(PCLMULQDQ, "CLMUL");
	CHECK_FEATURE(F16C, "F16C");
	CHECK_FEATURE(BMI1, "BM1");

	if (missing_set)
		return false;

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
			std::string temp(R"(C:\Users\vince\Desktop\ISHALLRISE\eboot.bin-decrypted)");
			app->boot(temp);

			// enter event loop
			return app->exec();
		}
	}

	return 0;
}