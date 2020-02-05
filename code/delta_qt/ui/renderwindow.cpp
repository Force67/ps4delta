
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "logger/logger.h"
#include "renderwindow.h"

#if 0
renderWindow::renderWindow() : QWindow() {
    // require 720p
    setMinimumWidth(1280);
    setMinimumHeight(720);
    setTitle("PS4Delta - GameWindow - [Vulkan]");
}

renderWindow::~renderWindow() {}

void renderWindow::toggleFullscreen() {
    /*if (inFullscreen)
            //setVisibility(Windowed);
    else
            //setVisibility(FullScreen);*/

    inFullscreen = !inFullscreen;
}

void renderWindow::takeScreenshot(std::vector<uint8_t>& data, uint32_t sizeX, uint32_t sizeY) {
    LOG_UNIMPLEMENTED;
}
#endif