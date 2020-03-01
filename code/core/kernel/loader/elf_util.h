#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <cstdint>

namespace kern {
class process;

// symbol util
void encode_nid(const char* symName, uint8_t* out);
bool decode_nid(const char* subset, size_t len, uint64_t&);

// process util
bool relocate_modules(process&);
void start_modules(process&);
} // namespace kern