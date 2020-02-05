#pragma once

// Copyright (C) Force67 2019

#include <base.h>

namespace kern {
int PS4ABI sys_ioctl(uint32_t fd, uint32_t cmd, void* data);
}