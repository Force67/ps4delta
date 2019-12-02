
// Copyright (C) 2019 Force67

// temp shit
#ifdef _WIN32
#include <Windows.h>
#define EXP extern "C" __declspec(dllexport)
#endif

#include <cstdio>
#include <Delta.h>

EXP int deltaBoot()
{
	const std::wstring game(LR"(C:\Users\vince\Desktop\AUTOMATEME\eboot.bin-decrypted)");
	// create a delta instance
	auto& dt = Delta::Get();
	__debugbreak();
	// boot the title
	dt.Boot(game);

	getchar();
	return 0;
}