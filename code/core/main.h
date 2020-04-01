#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <memory.h>
#include <utl/string_util.h>

// OS specific entry wrapper
#ifdef _WIN32
#define DELTA_MAIN(x)                               \
EXPORT int delta_main(memory::init_info& info) {    \
    memory::preinit(info);                          \
                                                    \
    i32 argc = 0;                                   \
    char** argv = nullptr;                          \
    utl::init_utf8_args(argc, &argv);               \
                                                    \
    return x(argc, argv);                           \
}                                                   
#else
#define DELTA_MAIN(x)                               \
EXPORT int delta_main(int argc, char** argv) {      \
    return x(argc, argv);                           \
}                                                   
#endif                                          