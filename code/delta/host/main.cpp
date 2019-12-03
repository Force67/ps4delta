
// Copyright (C) 2019 Force67

#ifdef _WIN32
#include <Windows.h>
#endif

#ifdef _WIN32

// windows platform init
int main(int argc, char **argv)
{
	auto *hlib = LoadLibraryW(L"dcore.dll");
	if (!hlib) {
		MessageBoxW(0, L"Unable to load core", FXNAME_WIDE, 0);
		return -1;
	}

	auto corMain = (int(*)(int, char**))GetProcAddress(hlib, "dcoreMain");
	if (corMain)
		return corMain(argc, argv);

	return -1;
}
#endif