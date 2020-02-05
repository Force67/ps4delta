
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <utl/file.h>
#include <utl/mem.h>
#include <utl/path.h>
#include <xbyak.h>

#include "core.h"
#include "module.h"
#include "process.h"

#include <logger/logger.h>

namespace kern {
static process* current_proc{nullptr};

UniquePtr<process> process::create(core::System& sys, std::string name) {
    auto proc = std::make_unique<process>(sys);
    proc->name = std::move(name);
    proc->pid = 1;

    // set that process active
    current_proc = proc.get();

    LOG_INFO("process::create {} with pid {}", proc->name, proc->pid);
    return proc;
}

// get the currently executing process
process* process::getActive() {
    return current_proc;
}

process::process(core::System& s) : sys(s) {}

SharedPtr<prx_module> process::loadPrx(std::string_view name) {
    auto prx = getPrx(name);
    if (!prx) {
        auto& fs = sys.file_system();

        // this is temporary uglyness
        auto pathX = fs.get("/system/common/lib/" + std::string(name) + ".sprx");

        // TODO: this is a _bit_ ugly
        prx = prx_module::load(*this, pathX);
        if (!prx) {
            __debugbreak();
            return nullptr;
        }

        LOG_INFO("process::loadPrx {}", name);

        // add it to our collection
        prx->started = true;
        modules.push_back(prx);

        prx->loadNeededPrx();
    }
    return prx;
}

SharedPtr<prx_module> process::getPrx(std::string_view name) {
    auto iter = std::find_if(modules.begin(), modules.end(),
                             [&name](const auto& e) { return e->name == name; });

   if (iter != modules.end())
        return *iter;

   return nullptr;
}

SharedPtr<prx_module> process::getPrx(uint32_t handle) {
    auto iter = std::find_if(modules.begin(), modules.end(),
                             [&handle](const auto& e) { return e->handle() == handle; });

    if (iter != modules.end())
        return *iter;

    return nullptr;
}

bool process::load(std::string_view path) {
    if (!loadPrx("libkernel") || 
        !loadPrx("libSceLibcInternal")) {
        LOG_ERROR("Unable to preload system prx libs");
        return false;
    }

    main_module = exec_module::load(*this, path);
    if (!main_module) {
        LOG_ERROR("Failed to load eboot");
        return false;
    }

    return true;
}

static void PS4ABI shutdownHandler() {
    __debugbreak();
}

void process::run() {
    using exit_func_t = PS4ABI void (*)();
    using main_init_t = PS4ABI void (*)(void*, exit_func_t);

    init_modules(*this, false);

    union stack_entry {
        const void* ptr;
        uint64_t val;
    };

    auto stack = std::make_unique<stack_entry[]>(128);

    auto s = reinterpret_cast<stack_entry*>(&stack[1]);
    (*s++).ptr = main_module->name.c_str();

    stack[0].val = 1 + sys.argv.size(); // argc
    for (auto& it : sys.argv)
        (*s++).ptr = it.c_str();

    (*s++).ptr = nullptr; // arg null terminator
    (*s++).ptr = nullptr; // env null terminator
    (*s++).val = 9ull;    // entrypoint type
    (*s++).ptr = (const void*)(main_module->entry);
    (*s++).ptr = nullptr; // aux null type
    (*s++).ptr = nullptr;

    auto func = reinterpret_cast<main_init_t>(getPrx("libkernel")->entry);
    return func(stack.get(), shutdownHandler);
}
} // namespace kern