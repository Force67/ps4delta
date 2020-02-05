
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <cstdio>
#include <thread>

#include <logger/logger.h>
#include "core.h"

#include <utl/mem.h>
#include "arch/arch.h"

namespace core {
System System::s_instance;

System::System() {
    mem_mgr = std::make_unique<memory::vmManager>();
}

bool System::init() {
    if (!arch::validateCpu())
        return false;

    // init the system memory manager
    if (!mem_mgr->init()) {
        LOG_ERROR("Failed to map/initialize system memory");
        return false;
    }

    file_sys = fs::fileSystem::create();
    if (!file_sys) {
        LOG_ERROR("Failed to mount virtual file system paths");
        return false;
    }

    // null, till we implement a real one
    constexpr auto target = video_core::renderBackend::null;
    renderer = video_core::createRenderer(target);
    if (!renderer) {
        LOG_ERROR("Unable to create the renderer");
        return false;
    }

    LOG_INFO("System init OK");
    return true;
}

void System::load(std::string& dir) {
    main_proc = kern::process::create(*this, "main");
    if (!main_proc) return;

    if (!main_proc->load(dir)) {
        LOG_ERROR("System load FAILED");
        return;
    }
    
    LOG_INFO("System load OK");
    main_proc->run();
}

} // namespace core