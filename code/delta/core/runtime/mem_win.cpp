
#include <Windows.h>
#include <cstdint>

//https://github.com/wine-mirror/wine/blob/master/libs/wine/mmap.c
#if 0
static bool g_tlsInitguad = false;

static void reserveArea(void *addr, void *end) {
  size_t size = (uint8_t *)addr - (uint8_t *)end;

  if (!VirtualAlloc(addr, size, MEM_RESERVE, PAGE_READWRITE))
    __debugbreak();
}

static void initReservations() {
  /*block as much space as we can*/
  reserveArea((void *)0x000000010000, (void *)0x000068000000);
  reserveArea((void *)0x00007ff00000, (void *)0x00007fff0000);
  //reserveArea((void *)0x7ffffe000000, (void *)0x7fffffff0000);
}

/*In order to reserve memory during load we "abuse" tls mechanism,
 this should only be done from the main executable*/
void NTAPI initHook(PVOID, DWORD, PVOID) {
  if (!g_tlsInitguad) {
    initReservations();
    g_tlsInitguad = true;
  }
}

#pragma const_seg(".CRT$XLB") 

extern "C" __declspec(allocate(".CRT$XLB"))
    PIMAGE_TLS_CALLBACK _xl_y = initHook;

#pragma const_seg()
#endif