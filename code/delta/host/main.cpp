
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
int wmain(int argc, wchar_t **argv)
{
	//C:\Users\vince\Desktop\AUTOMATEME\eboot.bin-decrypted

	auto* temp = LoadLibraryW(L"dcore.dll");
	if (temp) {
		auto boot = (int(*)(const wchar_t*))GetProcAddress(temp, "deltaBoot");
		if (boot)
			return boot(argv[0]);
	}

	return -1;
}
#endif