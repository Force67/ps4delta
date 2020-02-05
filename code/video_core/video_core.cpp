
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "video_core.h"

namespace video_core {
std::unique_ptr<renderInterface> createRenderer(renderBackend type) {
    std::unique_ptr<renderInterface> ptr;

    switch (type) { 
    case renderBackend::null:
        ptr = std::make_unique<nullRenderer>();
        break;
    case renderBackend::vulkan:
    case renderBackend::opengl:
    default:
        return nullptr;
    }

    if (!ptr->init())
        return nullptr;

    return ptr;
}
} // namespace video_core