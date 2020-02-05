
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <cstdint>
#include <string>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#include <utl/mcrt_win.h>
#include <memory.h>

#define EXPORT __declspec(dllexport)
#endif

#include <core.h>
#include <logger/logger.h>

#include <GLFW/glfw3.h>

int deltaMain(int argc, char** argv) {
    utl::createLogger(true);

    glfwSetErrorCallback(
        [](int errc, const char* desc) { LOG_ERROR("glfw error ({}) : {}", errc, desc); });

    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize glfw");
        return -1;
    }

    auto& sys = core::System::get();
    if (!sys.init())
        return -1;

    // propagate command line arguments
    if (argc > 1) {
        std::vector<std::string> xargv;

        for (int i = 1; i < argc; i++)
            xargv.emplace_back(argv[i]);

        sys.argv = std::move(xargv);
        sys.load(sys.argv[0]);
    }

    return 0;
}

#ifdef _WIN32
EXPORT int delta_main(memory::init_info& info) {
    // initialize the virtual memory manager
    memory::preinit(info);

    // convert command line to c style command line
    int32_t nArgs = 0;
    auto** args = utl::cmdl_to_argv(GetCommandLineA(), nArgs);

    return deltaMain(nArgs, args);
}
#else
EXPORT int delta_main(int argc, char** argv) {
    return deltaMain(argc, argv);
}
#endif