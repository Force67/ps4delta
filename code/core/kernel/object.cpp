
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "process.h"
#include "object.h"
#include "id_manager.h"

#include <logger/logger.h>

namespace kern {
s_object::s_object(process* proc, oType type) : otype(type), proc(proc) {
    uint32_t temp = 0;
    utl::fxm<idManager>::get().add(this, temp);
}

void s_object::release() {
    if (--refCount == 0)
        delete this;
}

void s_object::retain() {
    refCount++;
}

void s_object::retainHandle() {
    utl::fxm<idManager>::get().keep(handleCollection[0]);
}

void s_object::releaseHandle() {
    utl::fxm<idManager>::get().release(handleCollection[0]);
}
} // namespace krnl