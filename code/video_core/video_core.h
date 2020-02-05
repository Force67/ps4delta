#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <memory>

namespace video_core {
enum class renderBackend {
    null,
    vulkan,
    opengl
};

class renderInterface {
public:
    virtual ~renderInterface() = default;
    virtual bool init() = 0;
    virtual void shutdown() = 0;
    virtual void requestScreenshot() = 0;
};

class nullRenderer : public renderInterface {
public:
    bool init() override { return true; }
    void shutdown() override{};
    void requestScreenshot() override{};
};

class renderInterface;

std::unique_ptr<renderInterface> createRenderer(renderBackend);
}