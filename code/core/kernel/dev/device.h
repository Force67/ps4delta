#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "kernel/sys/error_table.h"
#include "../object.h"

#include <base.h>

namespace kern {
class proc;

class device : public object {
public:
    inline device(process* p) :
        object(object::kind::device), parentProc(p) {}

    virtual bool init(const char*, uint32_t, uint32_t) {
        return true;
    }
    virtual uint8_t* map(u8*, size_t, uint32_t, uint32_t, size_t) {
        __debugbreak();
        return nullptr;
    }
    virtual int32_t ioctl(uint32_t command, void* args) {
        __debugbreak();
        return -1;
    }

protected:
    process* parentProc = nullptr;
};
}