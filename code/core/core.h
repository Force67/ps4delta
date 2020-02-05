#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>

#include <string>
#include <vector>

#include "memory.h"
#include "video_core.h"
#include "file_sys/file_sys.h"

#include "kern/process.h"

namespace core {
using argvList = std::vector<std::string>;

enum class backend {
    native,
};

class System {
public:
    static System& get() { return s_instance; }

    System();

    bool init();
    void load(std::string& fromdir);

    inline memory::vmManager& mem() { return *mem_mgr; }
    inline fs::fileSystem& file_system() { return *file_sys; }

public:
    argvList argv;

private:
    static System s_instance;

    UniquePtr<memory::vmManager> mem_mgr;
    UniquePtr<video_core::renderInterface> renderer;
    UniquePtr<fs::fileSystem> file_sys;
    UniquePtr<kern::process> main_proc;
};
}