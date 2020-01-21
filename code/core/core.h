#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <memory>
#include <string>
#include <kern/proc.h>

namespace core {
bool canRunSystem();

class System {
public:
    using argvList = std::vector<std::string>;

    static System& get() {
        return s_instance;
    }

    void boot(std::string& fromdir);

public:
    argvList argv;

private:
    static System s_instance;
    std::unique_ptr<krnl::proc> process;
};
}