
// Copyright (C) Force67 2019

#include "mem.h"

#include <Windows.h>

namespace utl
{
	/*TODO: bugcheck on default cases*/

	DWORD protection_ToWin32(pageProtection prot)
	{
		switch (prot) {
		case pageProtection::noaccess:
			return PAGE_NOACCESS;
		case pageProtection::read:
			return PAGE_READONLY;
		case pageProtection::write:
			return PAGE_READWRITE;
		case pageProtection::execute:
			return PAGE_EXECUTE_READ;
		case pageProtection::rwx:
			return PAGE_EXECUTE_READWRITE;
		default:
			return PAGE_READWRITE;
		}
	}

	DWORD allocType_ToWin32(allocationType type)
	{
		switch (type) {
		case allocationType::commit:
			return MEM_COMMIT;
		case allocationType::reserve:
			return MEM_RESERVE;
		case allocationType::reservecommit:
			return MEM_RESERVE | MEM_COMMIT;
		default:
			return MEM_COMMIT;
		}
	}

	void* allocMem(void* preferredAddr, size_t length, pageProtection prot, allocationType type) {
		return VirtualAlloc(preferredAddr, length, allocType_ToWin32(type), protection_ToWin32(prot));
	}

	bool protectMem(void* addr, size_t len, pageProtection prot) {
		DWORD old;
		return VirtualProtect(addr, len, protection_ToWin32(prot), &old);
	}

	size_t getAvailableMem()
	{
		MEMORYSTATUSEX statex{};
		statex.dwLength = sizeof(statex);

		if (GlobalMemoryStatusEx(&statex))
			return statex.ullTotalPhys;

		return -1;
	}
}