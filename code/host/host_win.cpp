
// Copyright (C) 2019 Force67

#include <cstdint>
#include <Windows.h>
#include <VersionHelpers.h>
#include <memory.h>

// request to be run on the dedicated gpu
extern "C" {
__declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

// preallocating a block of memory with the smallest dependency count possible
// idea based on wine pre-loader
// this is done to ensure the likelihood of being able to allocate at the desired
// address bases required by modules like libkernel for thread management etc.
// (https://github.com/wine-mirror/wine/blob/master/libs/wine/mmap.c)

// so we can use modern c++ initializers
void* memset(void* dest, int val, size_t len) {
    unsigned char* ptr = (unsigned char*)dest;
    while (len-- > 0)
        *ptr++ = val;
    return dest;
}

static memory::init_info g_mem{};

using dMain = int (*)(const memory::init_info&);

static uint8_t* reserve_block(memory::mem_area &info, uint8_t* desiredAddr, size_t size = 0x100000000) {
    for (uintptr_t cursor = reinterpret_cast<uintptr_t>(desiredAddr);; cursor += 0x100000000) {
        if (VirtualAlloc(reinterpret_cast<void*>(cursor), size, MEM_RESERVE, PAGE_NOACCESS)) {
            uint8_t* ptr = reinterpret_cast<uint8_t*>(cursor);

            info.ptr = ptr;
            info.size = size;
            return ptr;
        }
    }

    return nullptr;
}

static void release_blocks() {
    auto free_block = [](memory::mem_area& info) { 
        VirtualFree(info.ptr, info.size, MEM_RELEASE);
    };

    free_block(g_mem.proc);
    free_block(g_mem.procMirror);
    free_block(g_mem.exec);
    free_block(g_mem.debug);
}

static void raise_err(const wchar_t* msg) {
    static decltype(&::MessageBoxW) msgBox = nullptr;

    if (!msgBox) {
        auto* huser = LoadLibraryW(L"User32.dll");
        if (huser) {
            msgBox = (decltype(&::MessageBoxW))GetProcAddress(huser, "MessageBoxW");
        }
    }

    msgBox(nullptr, msg, FXNAME_WIDE, MB_ICONERROR);
}

static bool reserve_blocks() {
    const wchar_t* errMsg = nullptr;
    //if (!reserveArea(0xFE0000000, 0x10000)) /*TODO: gnm chunk?*/
    //                 0xC0000000

    /*virtual memory for the process*/
    uint8_t* ptr = nullptr;

    if (!(ptr = reserve_block(g_mem.proc, reinterpret_cast<uint8_t*>(0x2'0000'0000))))
        errMsg = L"Failed to reserve user area";
    if (!(ptr = reserve_block(g_mem.procMirror, ptr)))
        errMsg = L"Failed to mirror user area";
    if (!(ptr = reserve_block(g_mem.exec, ptr, 0x200000000)))
        errMsg = L"Failed to reserve exec area";
    if (!(ptr = reserve_block(g_mem.debug, ptr)))
        errMsg = L"Failed to reserve debug area";

    if (errMsg)
        raise_err(errMsg);

    return errMsg == nullptr;
}

static bool validate_platform() {

    if (!IsWindows8OrGreater()) {
        raise_err(L"Your operating system is no longer supported. Please update to windows 10");
        return false;
    }

    MEMORYSTATUSEX statex{};
    statex.dwLength = sizeof(statex);
    if (!GlobalMemoryStatusEx(&statex) || statex.ullTotalPhys < 0x200000000 /*eight gb*/) {
        raise_err(L"Your system doesn't have enough physical memory");
        return false;
    }

    return true;
}

// windows platform init
int start() {
    if (!validate_platform())
        return -1;

    int result = 0;
    if (!reserve_blocks())
        return -2;

    // TODO: handle different cores
    auto* hlib = LoadLibraryW(L"delta-qt.dll");
    if (!hlib) {
        raise_err(L"Unable to load core");
        return -3;
    }

    auto corMain = (dMain)GetProcAddress(hlib, "delta_main");
    if (corMain) {
        result = corMain(g_mem);
    }

    release_blocks();
    return result;
}