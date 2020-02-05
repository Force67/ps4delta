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
#include "sce_module.h"
#include "object.h"
#include "id_manager.h"

namespace core {
class System;
}

namespace kern {
struct procInfo {
    uint32_t ripZoneSize = 10 * 1024;
    uint8_t* userStack = nullptr;
    size_t userStackSize = 20 * 1024 * 1024;
    void* fsBase = nullptr;
};

enum process_type {
    process_user
};

class process {
public:
    using prx_list_t = std::vector<SharedPtr<prx_module>>;

    static UniquePtr<process> create(core::System&, std::string name);
    static process* getActive();

    process(core::System&);

    // load executable and prx files
    bool load(std::string_view);

    // run the process indefinitely
    void run();

    // load a new module in the target process
    // returns a previous module handle if it already exists
    SharedPtr<prx_module> loadPrx(std::string_view name);

    // get's the module
    SharedPtr<prx_module> getPrx(std::string_view name);
    SharedPtr<prx_module> getPrx(uint32_t handle);

    inline idManager& ids() { return id_mgr; }
    inline prx_list_t& prx_list() { return modules; }
    inline exec_module& main_exec() { return *main_module; }

private:
    SharedPtr<exec_module> main_module;

    // object manager
    idManager id_mgr;

    // desc
    std::string name;

    // the id of this process
    uint32_t pid;

    // our prx module collection
    prx_list_t modules;

    // core ref
    core::System& sys;
};
}