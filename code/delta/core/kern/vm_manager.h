#pragma once

// Copyright (C) Force67 2019

#include <cstdint>
#include <vector>

#include <utl/mem.h>

namespace krnl {
struct procInfo;

using mprot = utl::pageProtection;
using alloct = utl::allocationType;

struct pageInfo {
  uint8_t *ptr;
  size_t size;
  mprot prot;
  const char *name = nullptr;

  pageInfo(uint8_t *p, size_t s, mprot mp) : ptr(p), size(s), prot(mp) {}
};

class vmManager {
public:
  vmManager(procInfo &);
  ~vmManager();

  bool init();
  void add(uint8_t *ptr, size_t size, mprot);
  pageInfo *get(uint8_t *ptr);

  uint8_t *mapMemory(uint8_t *preference, size_t size, utl::pageProtection);
  void unmapRtMemory(uint8_t *);

private:
  procInfo &pinfo;

  size_t codeMemTotal{0};
  size_t rtMemTotal{0};

  std::vector<pageInfo> codePages;
  std::vector<pageInfo> rtPages;
};
}