
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "kernel_state.h"

namespace kern {
// kernel instance
static KernelState* g_ks{nullptr};

// currently executed process
// has to remain a global to optimize TLS access
process* current_proc{nullptr};

process* active_proc() { return current_proc; }
KernelState* kernel_state() { return g_ks; }

KernelState::KernelState(System& s) : system(s) {
    g_ks = this;
}

KernelState::~KernelState() {
    g_ks = nullptr;
}

bool KernelState::LoadAndExecuteProc(const std::string& path) {
    proc = kern::process::create(system /*TODO: make it deal on KSTATE level only*/, "main");
    if (!proc) return false;

    if (!proc->load(path)) {
        LOG_ERROR("process load FAILED");
        return false;
    }

    // set it active
    current_proc = proc.get();

    LOG_INFO("System load OK");
    proc->run();
    return true;
}
}