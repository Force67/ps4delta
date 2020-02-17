
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
u8* PS4ABI sys_mmap(void* addr, size_t size, u32 prot, u32 flags, u32 fd, size_t offset) {
    // stop
    if (!addr) {
        __debugbreak();
        addr = reinterpret_cast<void*>(0x200000000);
    }

    // FOR NOW we only select user block
    auto block = memory::manager()->getBlock(static_cast<u8*>(addr), memory::user);
    if (!block) {
        LOG_ERROR("SYS: Unable to select proper memory block");
        return reinterpret_cast<u8*>(-1); // TODO: some sort of autocastable macro?
    }

    // FIXME, use real ppt
    const u32 hack = memory::page_executable | memory::page_writable | memory::page_readable;

    // do the actual allocation
    void* ptr =
        block->xalloc(static_cast<u8*>(addr), size, flags, (memory::page_flags)hack, 0x1000);

    if (!ptr) {
        LOG_ERROR("SYS: Unable to alloc at address {} in user pool", ptr);
        return reinterpret_cast<u8*>(-1); // TODO: some sort of autocastable macro?
    }

    if (fd != -1) {
        if (auto* dev = utl::fxm<idManager>::get().get(fd)) {

            // notify device of memory allocation
            static_cast<device*>(dev)->map(addr, size, prot, flags, offset);
        }
    }

    std::printf("mmap request %p -> action %p %x, %p\n", addr, ptr, size, _ReturnAddress());

    if (flags & mFlags::stack)
        return &static_cast<u8*>(ptr)[size];

    return static_cast<u8*>(ptr);
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