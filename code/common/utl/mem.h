#pragma once

/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <cstdint>
#include <type_traits>

namespace utl {
enum class allocType { reserve, commit, reservecommit };

enum page_protection : uint32_t {
    page_private = 0,
    page_read = 1 << 0,
    page_read_write = 1 << 1,
    page_execute = 1 << 2,
    page_execute_read_write = page_read_write | page_execute,
};

inline bool operator&(page_protection lhs, page_protection rhs) {
    return (static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs));
}

inline page_protection operator|(page_protection lhs, page_protection rhs) {
    return static_cast<page_protection>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
}

inline page_protection& operator|=(page_protection& lhs, page_protection rhs) {
    lhs = lhs | rhs;
    return lhs;
}

using page_protection = page_protection;

void* allocMem(void* preferredAddr, size_t len, page_protection, allocType);
void freeMem(void* addr, bool free = true);
bool protectMem(void* addr, size_t len, page_protection);

size_t getAvailableMem();
}