#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "id_manager.h"
#include "process.h"

namespace delta {
class System;
}

namespace kern {
class KernelState {
public:
    KernelState(System&);
    ~KernelState();

    inline idManager& idm() { return id_mgr; }
    inline process* currentProcess() { return nullptr; }

    bool LoadAndExecuteProc(const std::string&);
private:
    System& system;

    idManager id_mgr;
    UniquePtr<process> proc;
};

KernelState* kernel_state();

// currently executed process
process* active_proc();
}