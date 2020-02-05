

// Copyright (C) Force67 2019

#include "kern/process.h"
#include "sys_generic.h"

namespace kern {
int PS4ABI sys_ioctl(uint32_t fd, uint32_t cmd, void* data) {
    auto* proc = process::getActive();
    if (!proc)
        return -1;

    auto* obj = proc->ids().get(fd);
    if (obj)
        return static_cast<device*>(obj)->ioctl(cmd, data);
    else {
        __debugbreak();
    }

    return 0;
}
} // namespace krnl