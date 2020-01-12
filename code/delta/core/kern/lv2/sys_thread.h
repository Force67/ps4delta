#pragma once

// Copyright (C) Force67 2019

#include <base.h>

namespace krnl {
struct thread_prio {
  uint16_t type;
  uint16_t prio;
};

int PS4ABI sys_thr_self(uint32_t *tid);
int PS4ABI sys_rtprio_thread(int, uint64_t, thread_prio *);
int PS4ABI sys_umtx_op(void *, int, uint32_t, void *, void *);
}