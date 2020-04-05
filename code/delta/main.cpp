
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <main.h>
#include <config.h>
#include <core.h>

#include <logger/logger.h>
#include <video_core.h>

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

static config::opt<i32> g_displayWidth{"display.sizeW", "Window width", 1080};
static config::opt<i32> g_displayHeight{"display.sizeH", "Window height", 1920};

// minimal frontend
class glfwFrontend final : public video_core::RenderWindow {
public:
    glfwFrontend(i32 &argc, char **argv);
    ~glfwFrontend();

    bool init();
    void run();

    // may be called again on switch of GFX api
    bool createViewport(video_core::renderBackend);

    // renderWindow
    void* getHandle() override;
    void getDimensions(i32& width, i32& height) override;

private:
    System system;   
    GLFWwindow* window = nullptr;
    bool needsSwap = false;
};

glfwFrontend::glfwFrontend(i32 &argc, char **argv) {
    // convert args
    if (argc > 1) {
        std::vector<std::string> xargv;

        for (int i = 1; i < argc; i++)
            xargv.emplace_back(argv[i]);

        system.argv = std::move(xargv);
    }
}

glfwFrontend::~glfwFrontend() {
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

bool glfwFrontend::init() {
    glfwSetErrorCallback([](int errc, const char* desc) { 
        LOG_ERROR("glfw error ({}) : {}", errc, desc); });

    if (!glfwInit()) {
        LOG_ERROR("Failed to initialize glfw");
        return false;
    }

    if (!system.setup(*this)) return false;

    return true;
}

void glfwFrontend::run() {
    // booting from command line
    if (system.argv.size()) {
        system.load(system.argv[0]);
    }

    while (!glfwWindowShouldClose(window)) {
        // when rendering with OGL we need to swap
        // front and back buffer
        if (needsSwap) glfwSwapBuffers(window);

        // update input, check for window resize, etc
        glfwPollEvents();
    }
}

bool glfwFrontend::createViewport(video_core::renderBackend api) {
    if (window)
        glfwDestroyWindow(window);

    switch (api) {
    case video_core::renderBackend::vulkan:
        if (!glfwVulkanSupported()) {
            LOG_ERROR("This machine doesn't support vulkan");
            return false;
        }

        // disable gl specifics
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        break;
    case video_core::renderBackend::opengl: {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        needsSwap = true;
        break;
    default: // unsupported
        LOG_ERROR("Unsupported graphics api requested ({})", static_cast<i32>(api));
        return false;
    }
    }

    window = glfwCreateWindow(g_displayWidth, g_displayHeight, PRJ_NAME " | " GIT_BRANCH "@" GIT_COMMIT, nullptr, nullptr);
    if (!window) {
        LOG_ERROR("Failed to create window");
        return false;
    }

    glfwSetWindowUserPointer(window, this);

    if (needsSwap)
        glfwMakeContextCurrent(window);

    return true;
}

void* glfwFrontend::getHandle() {
#ifdef _WIN32
    return glfwGetWin32Window(window);
#endif
}

void glfwFrontend::getDimensions(i32& width, i32& height) {
    glfwGetWindowSize(window, &width, &height);
}

inline int dmain(int argc, char** argv) {
    // create log sinks
    utl::createLogger(true);

    // load opts
    config::load();

    UniquePtr<glfwFrontend> appInstance(new glfwFrontend(argc, argv));
    if (!appInstance->init()) {
        return -1;
    }

    appInstance->run();
    return 0;
}

DELTA_MAIN(dmain);