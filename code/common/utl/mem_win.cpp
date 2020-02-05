
/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "mem.h"

#include <Windows.h>

namespace utl {
/*TODO: bugcheck on default cases*/

DWORD protection_ToWin32(page_protection prot) {
    switch (prot) {
    case page_protection::page_private:
        return PAGE_NOACCESS;
    case page_protection::page_read:
        return PAGE_READONLY;
    case page_protection::page_read_write:
        return PAGE_READWRITE;
    case page_protection::page_execute_read_write:
        return PAGE_EXECUTE_READWRITE;
    default:
        __debugbreak();
        return PAGE_NOACCESS;
    }
}

DWORD allocType_ToWin32(allocType type) {
    switch (type) {
    case allocType::commit:
        return MEM_COMMIT;
    case allocType::reserve:
        return MEM_RESERVE;
    case allocType::reservecommit:
        return MEM_RESERVE | MEM_COMMIT;
    default:
        __debugbreak();
        return 0;
    }
}

void* allocMem(void* preferredAddr, size_t length, page_protection prot, allocType type) {
    return VirtualAlloc(preferredAddr, length, allocType_ToWin32(type), protection_ToWin32(prot));
}

void freeMem(void* Addr, bool free) {
    VirtualFree(Addr, 0, free ? MEM_RELEASE : MEM_DECOMMIT);
}

bool protectMem(void* addr, size_t len, page_protection prot) {
    DWORD old;
    return VirtualProtect(addr, len, protection_ToWin32(prot), &old);
}

size_t getAvailableMem() {
    MEMORYSTATUSEX statex{};
    statex.dwLength = sizeof(statex);

    if (GlobalMemoryStatusEx(&statex))
        return statex.ullTotalPhys;

    return -1;
}
}