
// Copyright (C) 2019 Force67

#include <Windows.h>
#include <cstdio>

#include <Delta.h>

int wmain()
{
	// and create a log console
	::AllocConsole();
	::AttachConsole(GetCurrentProcessId());
	::SetConsoleTitleW(L"Dev Console");

	FILE* file = nullptr;
	freopen_s(&file, "CON", "w", stdout);
	freopen_s(&file, "CONIN$", "r", stdin);

	//void* m = VirtualAlloc(nullptr, 0x7fffffff, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	//std::printf("test %p\n", m);

	auto& dt = Delta::Get();
	//dt.Boot(LR"(C:\Users\vince\Desktop\.nomad\JOURNEY_HD\CUSA02172\Journey\Journey.elf)");
	//dt.Boot(LR"(C:\Users\vince\Desktop\pup\PS4UPDATE1.PUP.dec)");
	dt.Boot(LR"(C:\Users\vince\Desktop\.nomad\JOURNEY_HD\CUSA02172\eboot.bin)");
	//dt.Boot(LR"(C:\Users\vince\Desktop\.nomad\Angry_Birds_Star_Wars_1_01_PS4-LFC\CUSA00184\eboot.bin)");

	getchar();

	return 0;
}