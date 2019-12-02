
// Copyright (C) 2019 Force67

// temp shit
#ifdef _WIN32
#define EXP extern "C" __declspec(dllexport)
#endif

#include <delta.h>

// temp exposed
EXP int deltaBoot(const wchar_t *gameDir)
{
	auto& dt = delta::get();
	dt.boot(gameDir);

	getchar();
	return 0;
}