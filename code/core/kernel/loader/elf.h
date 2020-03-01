#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "kernel/module.h"

namespace kern {
enum class loadStatus {
    Success,
    ErrorNotFound,
    ErrorElfObj,
    ErrorNoCode,
    ErrorBadAlloc,
    ErrorBadMap,
    ErrorRecompiler,
    ErrorProcInfo,
    ErrorModuleInfo
};

enum class libLoadType {
    hleAll,
    hleSys,
    hleNone,
};

constexpr u32 default_rip_size = 2 * 1024;

bool loadElf(sce_module& elf, const std::string&);
}