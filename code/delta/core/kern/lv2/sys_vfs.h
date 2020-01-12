#pragma once

// Copyright (C) Force67 2019

#include <base.h>

namespace krnl {
enum fcFlags {
  /*open only*/
  O_RDONLY,
  O_WRONLY,
  O_RDWR,
  O_ACCMODE,

  O_EXEC = 0x00040000,
};

int PS4ABI sys_open(const char *path, uint32_t flags, uint32_t mode);
}