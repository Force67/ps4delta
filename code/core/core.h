#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>

#include "memory.h"
#include "video_core.h"
#include "file_sys/file_sys.h"
#include "kernel/kernel_state.h"

#include "tools/patch_engine.h"

// System holds the master instances
// of all subsystems
class System {
public:
    System();
    ~System();

    bool setup(video_core::RenderWindow&);
    void load(const std::string&);

    inline memory::vmManager& mem() { return *mem_mgr; }
    inline fs::fileSystem& file_system() { return *file_sys; }
    inline tools::PatchEngine& patch_man() { return *patch_engine; }

public:
    using argvList = std::vector<std::string>;
    argvList argv;

private:
    UniquePtr<memory::vmManager> mem_mgr;
    UniquePtr<video_core::renderInterface> renderer;
    UniquePtr<fs::fileSystem> file_sys;
    UniquePtr<kern::KernelState> kernel_state;
    UniquePtr<tools::PatchEngine> patch_engine;
};

System* system_state();