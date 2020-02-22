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
#include <vector>

#include "dev/device.h"
#include "module.h"
#include "object.h"
#include "id_manager.h"

namespace core {
class System;
}

namespace kern {
enum process_type {
    process_user,
    process_worker
};

class process {
public:
    using prx_list_t = std::vector<SharedPtr<prx_module>>;

    static UniquePtr<process> create(core::System&, std::string name);

    process(core::System&);

    // load executable and prx files
    bool load(const std::string&);

    // run the process indefinitely
    void run();

    // load a new module in the target process
    // returns a previous module handle if it already exists
    SharedPtr<prx_module> loadPrx(std::string_view name);

    // gets the module
    SharedPtr<prx_module> getPrx(std::string_view name);
    SharedPtr<prx_module> getPrx(uint32_t handle);

    inline prx_list_t& prx_list() { return modules; }
    inline exec_module& main_exec() { return *main_module; }

private:
    SharedPtr<exec_module> main_module;

    // desc
    std::string name;

    // the id of this process
    uint32_t pid;

    // our prx module collection
    prx_list_t modules;

    // core ref
    core::System& sys;
};

// currently executed process
process* activeProc();
}