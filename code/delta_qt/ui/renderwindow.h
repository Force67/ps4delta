#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <QWidget>
#include <QWindow>

#if 0
#include "gfx/gfx_base.h"

class renderWindow : public QWindow, public gfx::frameBase {
    Q_OBJECT

public:
    renderWindow();
    ~renderWindow();

    void toggleFullscreen() override;
    void takeScreenshot(std::vector<uint8_t>& data, uint32_t sizeX, uint32_t sizeY) override;

private:
    bool inFullscreen{false};
    bool showFps{false};
};
#endif