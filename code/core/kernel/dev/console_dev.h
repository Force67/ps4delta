#pragma once

// Copyright (C) Force67 2019

#include "device.h"

namespace kern {
class consoleDevice : public device {
public:
    consoleDevice(process*);

    bool init(const char*, uint32_t, uint32_t) override;
};
}