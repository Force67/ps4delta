
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <config.h>
#include <logger/logger.h>

#include <utl/file.h>
#include <utl/path.h>

#include "patch_engine.h"

namespace tools {
static config::opt<bool> g_enablePatches{"tools.enablePatches", "Enable the quick patch tool (patch.yml)", true};

// we only allow little endian
static patch_type to_ptype(const std::string &type) {
    if (type == "le8" || type == "byte")
        return patch_type::le8;
    if (type == "le16")
        return patch_type::le16;
    if (type == "le32")
        return patch_type::le32;
    if (type == "le64")
        return patch_type::le64;
    if (type == "lef32")
        return patch_type::lef32;
    if (type == "lef64")
        return patch_type::lef64;

    // else assume a 64 bit value
    return patch_type::le64;
}

void PatchEngine::load() {
    auto path = utl::make_app_path(utl::data_dir, "patch.yml");

    if (!utl::exists(path) || !g_enablePatches)
        return;

    try {
        auto doc = YAML::LoadFile(path);
       
        for (auto pair : doc) {
            auto& name = pair.first.Scalar();
            auto& data = patches[name];

            for (auto patch : pair.second) {
                if (!patch[0].IsScalar()) continue;

                auto type = to_ptype(patch[0].Scalar());

                patch_entry entry{};
                entry.type = type;
                entry.offset = patch[1].as<u32>(0);

                // format the value type correctly
                switch (type) { 
                case patch_type::lef32:
                    entry.val = std::bit_cast<u32>(patch[2].as<f32>());
                    break;
                case patch_type::lef64:
                    entry.val = std::bit_cast<u64>(patch[2].as<f64>());
                    break;
                default:
                    entry.val = patch[2].as<u64>();
                    break;
                }

                data.emplace_back(entry);
            }
        }

    } catch (std::exception& ex) {
        LOG_ERROR("Failed to parse patch.yml ({})", ex.what());
        return;
    }
}

// we only allow little endian values
void PatchEngine::applyPatches(const std::string& name, u8* base, u32 filesz) {
    if (!g_enablePatches /*patch policy: disabled*/)
        return;

    const auto entry = patches.find(name);
    if (entry == patches.end())
        return;

    // apply the patches
    for (const auto& e : entry->second) {
        u8* absptr = base + e.offset;

        // range check (-> we only allow patches in code/data region
        // of the module, not in special region like recompiler segment)
        if (absptr >= base && absptr <= (base + filesz)) {
            switch (e.type) { 
            case patch_type::le8:
                *absptr = static_cast<u8>(e.val);
                break;
            case patch_type::le16:
                *reinterpret_cast<u16*>(absptr) = static_cast<u16>(e.val);
                break;
            case patch_type::le32:
            case patch_type::lef32:
                *reinterpret_cast<u32*>(absptr) = static_cast<u32>(e.val);
                break;
            case patch_type::le64:
            case patch_type::lef64:
                *reinterpret_cast<u64*>(absptr) = static_cast<u64>(e.val);
                break;
            }
        } else {
            LOG_WARNING("Ignoring patch at {}; forbidden range", fmt::ptr(absptr));
            continue;
        }
    }
}
} // namespace tools