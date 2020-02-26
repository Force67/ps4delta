
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <logger/logger.h>

#include <utl/mem.h>
#include <utl/fxm.h>

#include "error_table.h"
#include "sys_mem.h"

#include "kernel/id_manager.h"
#include "kernel/dev/device.h"

#include "memory.h"

namespace kern {
u8* PS4ABI sys_mmap(u8* addr, size_t size, u32 prot, u32 flags, u32 fd, size_t offset) {

    //if (!addr && !(flags & mFlags::fixed))
    //    addr = reinterpret_cast<u8*>(0x2'0000'0000);

    // bad behaviour caused by return address check in
    // sceKernelMapNamedSystemFlexibleMemory
    if (addr == reinterpret_cast<u8*>(0x8'8000'0000))
        __debugbreak();

    // select our memory block by address
    auto block = memory::manager()->getBlock(addr);
    if (!block) {
        __debugbreak();
        LOG_ERROR("SYS: Unable to select proper memory block");
        return reinterpret_cast<u8*>(-1); // TODO: some sort of autocastable macro?
    }

    // FIXME, use real ppt
    const u32 hack = memory::page_executable | memory::page_writable | memory::page_readable;

    // do the actual allocation
    u8* ptr = block->xalloc(addr, size, flags, (memory::page_flags)hack, 0x1000 * 4 /*PS4 PAGE is 16 kib*/);

    //FIXME; FIXED CHECKS
    if (!memory::manager()->getBlock(nullptr, memory::user)->check(addr) && !ptr) {
        ptr = block->alloc(size, flags, (memory::page_flags)hack, 0x1000 * 4);
    }

    if (!ptr) {
        __debugbreak();
        LOG_ERROR("SYS: Unable to alloc at address {} in pool {}", fmt::ptr(ptr), fmt::ptr(block->base));
        return reinterpret_cast<u8*>(-1); // TODO: some sort of autocastable macro?
    }

    // stack grows from top to bottom
    if (flags & mFlags::stack)
        ptr = &ptr[size];

    if (fd != -1) {
        if (auto* dev = utl::fxm<idManager>::get().get(fd)) {

            // notify device of memory allocation
            static_cast<device*>(dev)->map(ptr, size, prot, flags, offset);
        }
    }

    std::printf("mmap req %p -> dest %p|%x|%p\n", addr, ptr, size, _ReturnAddress());
    return ptr;
}

int PS4ABI sys_mprotect(u8*, size_t len, int prot) {
    // TODO
    return 0;
}

int PS4ABI sys_mname(u8* ptr, size_t len, const char* name, void*) {
    //__debugbreak();
    #if 0
    auto* info = proc->getVma().get(ptr);
    if (!info) {
        LOG_WARNING("attempted to tag unknown memory ({}, {})", fmt::ptr(ptr), name);
        return -1;
    }

    LOG_WARNING("tagged {} with name {}", fmt::ptr(ptr), name);
    info->name = name;
    #endif
    return 0;
}

struct mdbg_property {
    int32_t unk;
    int32_t unk2;
    void* addr;
    size_t areaSize;
    int64_t unk3;
    int64_t unk4;
    char name[32];
};

static_assert(sizeof(mdbg_property) == 72);

int PS4ABI sys_mdbg_service(u32 op, void* arg1, void* arg2, void* a3) {
    switch (op) {
    case 1: {
        auto* info = static_cast<mdbg_property*>(arg1);
        LOG_WARNING("set property {} for addr {} with size {}", info->name, info->addr,
                    info->areaSize);
        /*TODO: create named object*/

        break;
    }
    }

    return 0;
}

int PS4ABI sys_dmem_container(u32 op) {
    if (op == -1)
        return 0;

    __debugbreak();
    return -1;
}
} // namespace krnl