#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <vector>
#include <unordered_map>

namespace tools {

// idea based on rpcs3's patch engine :)
enum class patch_type {
    le8,
    le16,
    le32,
    le64,

    // floating point
    lef32,
    lef64,
};

class PatchEngine 
{
public:
    void load();
    void applyPatches(const std::string &name, u8* base, u32 filesz);

private:
    struct patch_entry {
        patch_type type;
        u32 offset;
        u64 val;
    };

    std::unordered_map<std::string, std::vector<patch_entry>> patches;
};
}