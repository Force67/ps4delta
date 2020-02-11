

// Copyright (C) Force67 2019

#include "kernel/dev/device.h"
#include "kernel/id_manager.h"

#include "sys_generic.h"

#include <utl/fxm.h>

namespace kern {
int PS4ABI sys_ioctl(uint32_t fd, uint32_t cmd, void* data) {
    auto* obj = utl::fxm<idManager>::get().get(fd);
    if (obj)
        return static_cast<device*>(obj)->ioctl(cmd, data);
    else {
        __debugbreak();
    }

    return 0;
}
} // namespace krnl