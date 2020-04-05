
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
#include <utl/path.h>

#include "arch/arch.h"

#include <config.h>

static System* g_system = nullptr;

System* system_state() {
    return g_system;
}

System::System() {
    mem_mgr = std::make_unique<memory::vmManager>();
    file_sys = std::make_unique<fs::fileSystem>();
    patch_engine = std::make_unique<tools::PatchEngine>();

    // the kernel core
    kernel_state = std::make_unique<kern::KernelState>(*this);

    // global instance
    g_system = this;
}

System::~System() {
    g_system = nullptr;
}

bool System::setup(video_core::RenderWindow& videoOut) {
    // create memory manager, everything depends on it
    if (!mem_mgr->init()) {
        LOG_ERROR("Failed to map/initialize system memory");
        return false;
    }

    // patch engine, for in memory patches (maybe belong in "memory" folder)?
    patch_engine->load();

    // temp thing.. should be kept in a folder like "firmware" or so
    file_sys->mount("/system/common/lib", utl::make_app_path(utl::data_dir, "lib"));

    // create the gpu
    constexpr auto target = video_core::renderBackend::null;
    renderer = video_core::createRenderer(target);
    if (!renderer) {
        LOG_ERROR("Unable to create the renderer");
        return false;
    }

    LOG_INFO("System init OK");
    return true;
}

void System::load(const std::string& dir) {
    // TODO: content manager check; SFO etc

    if (!kernel_state->LoadAndExecuteProc(dir)) {
        LOG_ERROR("Failed to load and start process");
    }
}