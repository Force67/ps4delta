
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "object.h"
#include "kernel_state.h"

namespace kern {
object::object(kind type) : 
    type(type) 
{
    uint32_t temp = 0;
    kernel_state()->idm().add(this, temp);
}

void object::release() {
    if (--refCount == 0)
        delete this;
}

void object::retain() {
    refCount++;
}

void object::retainHandle() {
    kernel_state()->idm().keep(handleCollection[0]);
}

void object::releaseHandle() {
    kernel_state()->idm().release(handleCollection[0]);
}
} // namespace krnl