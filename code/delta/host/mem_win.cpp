
// Copyright (C) 2019 Force67

#ifdef _WIN32
#include <Windows.h>
#include <cstdint>
#endif

#ifdef _WIN32

// preallocating a block of memory with the smallest dependency count possible
// idea based on wine pre-loader
// (https://github.com/wine-mirror/wine/blob/master/libs/wine/mmap.c)

using dMain = int(*)();

static bool reserveArea(uintptr_t addr, size_t size, size_t attempts = 4) {
  bool result = false;

  auto cursor = addr;
  for (int i = 0; i < attempts; i++) {
    if (VirtualAlloc(reinterpret_cast<void *>(cursor), size, MEM_RESERVE,
                     PAGE_READWRITE)) {
      result = true;
      break;
    }
    cursor += 1024;
  }

  return result;
}

static void showErr(const wchar_t *msg) {
  static decltype(&::MessageBoxW) msgBox = nullptr;

  if (!msgBox) {
    auto *huser = LoadLibraryW(L"User32.dll");
    if (huser) {
      msgBox = (decltype(&::MessageBoxW))GetProcAddress(huser, "MessageBoxW");
    }
  }

  msgBox(nullptr, msg, FXNAME_WIDE, MB_ICONERROR);
}

static bool initReservations() {
  const wchar_t *errMsg = nullptr;

  /*reserve required mem areas*/
  if (!reserveArea(0xFE0000000, 0x10000)) /*gnm chunk*/
    errMsg = L"Failed to reserve GNM area";
  if (!reserveArea(0x200000000, 0x1000000)) /*thread zone*/
    errMsg = L"Failed to reserve Thread zone";

  if (errMsg) {
    showErr(errMsg);
  }

  return errMsg == nullptr;
}

// windows platform init
int hostEntry() {
  if (!initReservations()) {
    return -1;
  }

  auto *hlib = LoadLibraryW(L"dcore.dll");
  if (!hlib) {
    showErr(L"Unable to load core");
    return -1;
  }

  auto corMain = (dMain)GetProcAddress(hlib, "coreMain");
  if (corMain) {
    return corMain();
  }

  return 0;
}
#endif