
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <cstdint>
#include <cstdio>
#include <intrin.h>
#include <xbyak.h>

#include "sys_debug.h"
#include "sys_dynlib.h"
#include "sys_generic.h"
#include "sys_info.h"
#include "sys_mem.h"
#include "sys_net.h"
#include "sys_thread.h"
#include "sys_vfs.h"

#include "kern/process.h"

namespace kern {
int sys_write(uint32_t fd, const void* buf, size_t nbytes);
int sys_sigprocmask(int, const int*, int*);
int sys_sigaction(int how, void (*cb)(void*, void*, void*));
int sys_regmgr_call(uint32_t op, uint32_t id, void* result, void* value, uint64_t type);

uint32_t sys_namedobj_create(const char* name, void* arg2, uint32_t arg3);
int sys_namedobj_delete(uint32_t, uint32_t op);

int sys_budget_get_ptype();

int sys_getpid();
int sys_exit();
int sys_rfork();
int sys_execve();

int sys_sysarch(int num, void* args);

#if 0
moduleInfo* reportCallAddress(void* retAddr) {
    uintptr_t addrsafe = (uintptr_t)retAddr;

    for (auto& mod : process::getActive()->prx_list()) {
        auto& info = mod->getInfo();

        if (addrsafe <= (uintptr_t)(info.base + info.codeSize) &&
            (addrsafe >= (uintptr_t)info.base)) {
            std::printf(">>>> reportCallAddress: %p called in %s = %p\n", retAddr,
                        info.name.c_str(), (void*)((uint8_t*)addrsafe - info.base));
            return &info;
        }
    }
    return nullptr;
}
#endif

static int PS4ABI sys_nosys() {
    void* retAddr = _ReturnAddress();
    __debugbreak();
    //reportCallAddress(retAddr);
    return 0;
}

struct syscall_Reg {
    const void* ptr;
    const char* name;
};

static const syscall_Reg syscall_dpt[] = {
#include "syscalls.inl"
};

static void PS4ABI trace_syscall(const char* name, int index, void* addr) {
    std::printf("trace_syscall: %s (%d), %p\n", name, index, addr);
}

static uintptr_t emitCallReporter(const char* name, uint32_t sid, const void* dest) {
    struct callTrace : Xbyak::CodeGenerator {
        callTrace(uintptr_t name, uint32_t sid, uintptr_t dest) {
            push(rdi);
            push(rsi);
            push(rdx);
            push(rcx);
            push(r8);
            push(r9);
            push(r10);
            push(r11);

            sub(rsp, 0x28);
            mov(rdi, name);
            mov(esi, sid);
            mov(rdx, rsp);
            mov(rcx, reinterpret_cast<uintptr_t>(&trace_syscall));
            call(rcx);
            add(rsp, 0x28);

            pop(r11);
            pop(r10);
            pop(r9);
            pop(r8);
            pop(rcx);
            pop(rdx);
            pop(rsi);
            pop(rdi);

            mov(rax, dest);
            jmp(rax);
        }
    };

    callTrace* gen =
        new callTrace(reinterpret_cast<uintptr_t>(name), sid, reinterpret_cast<uintptr_t>(dest));

    return reinterpret_cast<uintptr_t>(gen->getCode());
}

uintptr_t lv1_get(uint32_t sid) {
    constexpr auto memberCount = sizeof(syscall_dpt) / sizeof(syscall_Reg);

    if (sid > memberCount)
        __debugbreak();

    return reinterpret_cast<uintptr_t>(
        emitCallReporter(syscall_dpt[sid].name, sid, syscall_dpt[sid].ptr));

    // return reinterpret_cast<uintptr_t>(syscall_dpt[sid].ptr);
}
} // namespace kern