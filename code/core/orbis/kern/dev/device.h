#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "kern/sys/error_table.h"
#include "../object.h"

namespace kern {
class proc;

class device : public s_object {
public:
    inline device(process* p) : s_object(p, s_object::oType::device) {}

    virtual bool init(const char*, uint32_t, uint32_t) {
        return true;
    }
    virtual uint8_t* map(void*, size_t, uint32_t, uint32_t, size_t) {
        __debugbreak();
        return nullptr;
    }
    virtual int32_t ioctl(uint32_t command, void* args) {
        __debugbreak();
        return -1;
    }
};
}