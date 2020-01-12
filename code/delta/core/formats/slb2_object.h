#pragma once

// Copyright (C) Force67 2019

#include <utl/File.h>

namespace formats {
struct slb2_header {
  uint32_t magic;
  uint32_t version;
  uint32_t flags;
  uint32_t fileCount;
  uint32_t blockCount;
  uint32_t unk[3];
};

struct slb2_entry {
  uint32_t offset;
  uint32_t fileSize;
  uint32_t unk[2];
  char fileName[32];
};

class slb2Object {
public:
  bool load(utl::File &);

private:
  slb2_header header{};
};
}