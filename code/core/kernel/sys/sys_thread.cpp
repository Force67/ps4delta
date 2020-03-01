
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <string>
#include <base.h>
#include <logger/logger.h>

#include "sys_thread.h"

namespace kern {
// get current thread id
int PS4ABI sys_thr_self(uint32_t* tid) {
    *tid = 1;
    return 0;
}

int PS4ABI sys_rtprio_thread(int a1, uint64_t tid, thread_prio* rtp) {
    // FIXME: we hardcode the thread id for now
    LOG_ASSERT(tid == 1);

    //rtp->type = 3; /*normal time sharing process*/
    //rtp->prio = 1; /*almost highest prio*/
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