
// Copyright (C) 2019 Force67

#include <cstdint>
#include <Windows.h>
#include <VersionHelpers.h>
#include <memory.h>

#include <utl/cpu_features.h>

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

wchar_t* mx_wcsstr(const wchar_t* s, const wchar_t* b) {
    auto* x = (wchar_t*)s;
    while (*x) {
        if (*x == *b) {
            auto* y = x;
            auto* c = (wchar_t*)b;
            while (*y && *c && *y == *c) {
                c++;
                y++;
            }
            if (!*c)
                return x;
        }
        x++;
    }
    return nullptr;
}

static memory::init_info g_mem{};

using dMain = int (*)(const memory::init_info&);

static uint8_t* reserve_block(memory::mem_area& info, uint8_t* desiredAddr,
                              size_t size = 0x100000000) {
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
    auto free_block = [](memory::mem_area& info) { VirtualFree(info.ptr, info.size, MEM_RELEASE); };

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
    // if (!reserveArea(0xFE0000000, 0x10000)) /*TODO: gnm chunk?*/
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
    if (!GlobalMemoryStatusEx(&statex) || statex.ullTotalPhys < 0x100000000 /*for now, 4 gib*/) {
        raise_err(L"Your system doesn't have enough physical memory");
        return false;
    }

    utl::cpu cpu;

#define CHECK_FEATURE(x, y)                                                                        \
    if (!cpu.has(utl::cpu::t##x)) {                                                                \
        raise_err(L"Your cpu is missing the " y L" instruction set.");                             \
        return false;                                                                              \
    }

    CHECK_FEATURE(SSE, "SSE");
    CHECK_FEATURE(SSE2, "SSE2");
    CHECK_FEATURE(SSE3, "SSE3");
    CHECK_FEATURE(SSSE3, "SSSE3");
    CHECK_FEATURE(SSE41, "SSE4.1");
    CHECK_FEATURE(SSE42, "SSE4.2");
    CHECK_FEATURE(AESNI, "AES");
    CHECK_FEATURE(AVX, "AVX");
    CHECK_FEATURE(PCLMULQDQ, "CLMUL");
    CHECK_FEATURE(F16C, "F16C");
    CHECK_FEATURE(BMI1, "BM1");

    return true;
}

// windows platform init
int start() {
    if (!validate_platform())
        return -1;

    int result = 0;
    if (!reserve_blocks())
        return -2;

    HINSTANCE hlib;
    if (mx_wcsstr(GetCommandLineW(), L"-qtcore"))
        hlib = LoadLibraryW(L"delta-qt.dll");
    else
        hlib = LoadLibraryW(L"delta.dll");
  
    if (!hlib) {
        raise_err(L"Unable to load core");
        return -3;
    }

    auto corMain = (dMain)GetProcAddress(hlib, "delta_main");
    if (corMain) {
        result = corMain(g_mem);
    } else
        raise_err(L"Unable to locate main export in core");

    release_blocks();
    return result;
}