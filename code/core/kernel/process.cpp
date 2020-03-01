
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
#include <utl/fxm.h>
#include <config.h>

#include "loader/elf_util.h"

#include <xbyak.h>

#include "core.h"
#include "process.h"

#include <logger/logger.h>

namespace kern {
config::opt<bool> hleKernel{"kern.useHLE", "HLE the kernel module", true};

// currently executed process
static process* current_proc{nullptr};

UniquePtr<process> process::create(core::System& sys, std::string name) {
    auto proc = std::make_unique<process>(sys);
    proc->name = std::move(name);
    proc->pid = 1;

    // ensure IDM exists
    if (utl::fxm<idManager>::empty())
        utl::fxm<idManager>::make();

    // set that process active
    current_proc = proc.get();

    // and create the user stack
    proc->userStack = memory::alloc(user_stack_size, memory::app);

    LOG_INFO("process::create {} with pid {}, userstack @ {}", proc->name, proc->pid, fmt::ptr(proc->userStack));
    return proc;
}

process::process(core::System& s) : sys(s) {}

SharedPtr<prx_module> process::loadPrx(std::string_view name) {
    auto prx = getPrx(name);
    if (prx) return prx;

    prx = prx_module::load(*this, name);
    if (!prx) {
        __debugbreak();
        return nullptr;
    }

    // add it to our collection
    prx->started = true;
    modules.push_back(prx);

    // load sub dependencies
    prx->loadNeededPrx();
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

    __debugbreak();

    return nullptr;
}

bool process::load(const std::string &path) {
    if (!hleKernel) {
        // in non HLE kernel mode we need to pre-load
        // the system libs
        if (!loadPrx("libkernel") || 
            !loadPrx("libSceLibcInternal")) {
            LOG_ERROR("Unable to preload system prx libs");
            return false;
        }
    }

    // load the eboot
    if ((main_module = exec_module::load(*this, path))) {
        // load linked dependencies of eboot
        main_module->started = true;
        main_module->loadNeededPrx();
        return true;
    }

    return false;
}

static void PS4ABI shutdownHandler() {
    __debugbreak();
}

void process::run() {
    using exit_func_t = PS4ABI void (*)();
    using main_init_t = PS4ABI void (*)(void*, exit_func_t);

    relocate_modules(*this);

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

    main_init_t start = nullptr;

    if (!hleKernel)
        start = reinterpret_cast<main_init_t>(getPrx("libkernel")->entry);
    else
        start = reinterpret_cast<main_init_t>(main_module->entry);

    return start(stack.get(), shutdownHandler);
}

process* activeProc() {
    return current_proc;
}
} // namespace kern