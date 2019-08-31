
// Copyright (C) 2019 Force67

#include <Windows.h>
#include <loader/Loader.h>

int wmain()
{
	// and create a log console
	::AllocConsole();
	::AttachConsole(GetCurrentProcessId());
	::SetConsoleTitleW(L"Dev Console");

	FILE* file = nullptr;
	freopen_s(&file, "CON", "w", stdout);
	freopen_s(&file, "CONIN$", "r", stdin);

	auto loader = loaders::CreateLoader(LR"(C:\Users\vince\Desktop\.nomad\JOURNEY_HD\CUSA02172\eboot.bin)");
	if (loader) {
		loader->Load();
	}

	getchar();

	return 0;
}