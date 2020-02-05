
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <string>
#include <base.h>

#include "sys_thread.h"

namespace kern {
int PS4ABI sys_thr_self(uint32_t* tid) {
    /*todo: kthread*/
    *tid = 0xF00D;
    return 135;
}

int PS4ABI sys_rtprio_thread(int a1, uint64_t a2, thread_prio* rtp) {
    rtp->type = 3; /*normal time sharing process*/
    rtp->prio = 1; /*almost highest prio*/
    return 0;
}

int PS4ABI sys_umtx_op(void* ptr, int, uint32_t op, void*, void*) {

    /*umtx_op_nwake_private*/
    if (op == 21)
        __debugbreak();

    __debugbreak();

    std::printf("locking kmutex! %p\n", ptr);
    __debugbreak();
    // TODO:
    return 0;
}
} // namespace krnl