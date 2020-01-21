
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

#include <xbyak_util.h>

#ifdef _WIN32
#include <VersionHelpers.h>
#endif

namespace core {
System System::s_instance;

void System::boot(std::string& xdir) {
    static std::string dir = xdir;

    // sanitize path
    std::replace(dir.begin(), dir.end(), '/', '\\');

    process = std::make_unique<krnl::proc>();

    std::thread ctx([&]() {
        if (!process->create(dir))
            return;

        process->start(argv);
    });

    ctx.detach();
}

bool canRunSystem() {
#ifdef _WIN32
    if (!IsWindows8OrGreater()) {
        LOG_ERROR("Your operating system is outdated. Please update to windows 8 "
                  "or newer.");
        return false;
    }
#endif

    constexpr size_t one_mb = 1024ull * 1024ull;
    constexpr size_t eight_gb = 8ull * 1024ull * one_mb;

    if (utl::getAvailableMem() < eight_gb) {
        LOG_ERROR("Your system doesn't have enough physical memory to run " FXNAME);
        return false;
    }

    std::string missingFeatures;
    Xbyak::util::Cpu cpu;

#define CHECK_FEATURE(x, y)                                                                        \
    if (!cpu.has(Xbyak::util::Cpu::t##x)) {                                                        \
        missingFeatures += std::string(y) + ";";                                                   \
    }

    CHECK_FEATURE(SSE, "SSE");
    CHECK_FEATURE(SSE2, "SSE2");
    CHECK_FEATURE(SSE3, "SSE3");
    CHECK_FEATURE(SSSE3, "SSSE3");
    CHECK_FEATURE(SSE41, "SSE4.1");
    CHECK_FEATURE(SSE42, "SSE4.2");
    CHECK_FEATURE(AESNI, "AES");
    CHECK_FEATURE(AVX, "AVX");
    CHECK_FEATURE(PCLMULQDQ, "CLMUL");
    CHECK_FEATURE(F16C, "F16C");
    CHECK_FEATURE(BMI1, "BM1");

    if (!missingFeatures.empty()) {
        LOG_ERROR("Your cpu is missing the following instructions: {}", missingFeatures);
        return false;
    }

    return true;
}
}