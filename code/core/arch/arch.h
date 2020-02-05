#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <xbyak_util.h>
#include <logger/logger.h>

namespace arch {
inline bool validateCpu() {
    std::string missingFeatures;
    Xbyak::util::Cpu cpu;

#define CHECK_FEATURE(x, y)                                                                        \
    if (!cpu.has(Xbyak::util::Cpu::t##x)) {                                                        \
        missingFeatures += ";" + std::string(y);                                                   \
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