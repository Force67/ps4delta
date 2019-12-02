
// Copyright (C) 2019 Force67

#ifdef _WIN32
#include <Windows.h>
#endif

// todo: check if cpu supports same hardware ext
static bool ValidateCPU()
{
	return true;
}

#ifdef _WIN32

// windows platform init
int wmain()
{
	auto* temp = LoadLibraryW(L"dcore.dll");
	if (temp) {
		auto boot = (int(*)())GetProcAddress(temp, "deltaBoot");
		if (boot)
			return boot();
	}

	return -1;
}
#endif