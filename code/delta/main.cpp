
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

#include <main.h>

#include <config.h>
#include <core.h>
#include <logger/logger.h>

#include <GLFW/glfw3.h>

class glfwApp {

};

static int dmain(int argc, char** argv) {
    // create log sinks
    utl::createLogger(true);

    // load opts
    config::load();

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
        // not blocking
        sys.load(sys.argv[0]);
    }

    return 0;
}

DELTA_MAIN(dmain);