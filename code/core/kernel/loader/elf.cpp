
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "elf.h"
#include "memory.h"

#include "arch/native/native_lift.h"
#include "formats/elf_object.h"

#include <crypto/sha1.h>

namespace kern {
using namespace formats;

static loadStatus patch_module(elfObject& elf, sce_module& mod) {
    // allocate additional rip memory for relative addressing
    uint8_t* rip = memory::alloc(default_rip_size, memory::exec);
    std::memset(rip, 0xCC, default_rip_size);

    // recompile unsupported instructions
    for (auto& p : elf.programs) {
        if (p.type == PT_LOAD && (p.flags & PF_X)) {
            codeLift lift(rip);
            LOG_ASSERT(lift.init());

            if (!lift.transform(mod.base + p.vaddr, p.memsz))
                return loadStatus::ErrorRecompiler;
        }
    }

#if 0
    // and lastly, apply page protections
    for (auto& s : elf.programs) {
        u32 perm = s.flags & (PF_R | PF_W | PF_X);
        auto trans_perm = [](u32 op) {
            switch (op) {
            case (PF_R | PF_X):
                return utl::page_execute_read_write;
            case (PF_R | PF_W):
                return utl::page_read_write;
            case (PF_R):
                return utl::page_read;
            default:
                return utl::page_private;
                /*todo: invalid parameter bugcheck*/
            }
        };

        utl::protectMem(mod.base + s.vaddr, s.filesz, trans_perm(perm));
    }
#endif

    // builtin hack: enable debug messages on 5.05
    if (mod.moduleHash == "PRX-f1d3ebb39f0e011286a43ceb1ef87d462b87b86f") {
        *(uint32_t*)(mod.base + 0x68264) = UINT32_MAX;
    } else if (mod.moduleHash == "PRX-dfb5aa182bee65859d19d16792940fd282489384") {
        *(uint8_t*)(mod.base + 0x23A20) = 0xCC;
    }

    return loadStatus::Success;
}

static loadStatus load_exec(elfObject& elf, exec_module& exec) {
    // fingerprint
    sha1_context sha;
    sha1_starts(&sha);

    u32 loadSize = 0;
    for (auto& p : elf.programs) {
        if (p.type == PT_LOAD || p.type == PT_SCE_RELRO)
            loadSize += ::align_up(p.memsz, p.align);
    }

    // allocate an aligned chunk
    exec.base = memory::alloc(loadSize, memory::exec);
    LOG_ASSERT(exec.base);

    for (const auto& p : elf.programs) {
        switch (p.type) {
        case PT_LOAD:
        case PT_SCE_RELRO: {
            if (p.memsz) {

                // create a fingerprint of each segment
                sha1_update(&sha, reinterpret_cast<const uint8_t*>(&p.vaddr), sizeof(p.vaddr));
                sha1_update(&sha, reinterpret_cast<const uint8_t*>(&p.memsz), sizeof(p.memsz));
                sha1_update(&sha, p.bin.data(), p.bin.size());

                std::memcpy(exec.base + p.vaddr, p.bin.data(), p.bin.size());
            }
            break;
        }
        case PT_SCE_COMMENT: {
            struct sce_comment {
                u32 type;
                u32 unk;
                u32 data_len;
            };

            const auto& comment = reinterpret_cast<const sce_comment&>(*p.bin.data());
            if (comment.type == 0x48544150 /*"PATH"*/) {

                // path is null terminated
                exec.comment = std::string(elf.at<const char>(p.offset + sizeof(sce_comment)));
            }
            break;
        }
        case PT_SCE_PROCPARAM: {

            auto* info = reinterpret_cast<process_param*>(exec.base + p.vaddr);
            if (info->size < 64)
                LOG_WARNING("Bad process_param size {},{}", info->size, sizeof(proc_param));

            if (info->magic != 0x4942524F /*'ORBI'*/)
                return loadStatus::ErrorProcInfo;

            LOG_INFO("** sdk version: {0:x}", info->sdk_version);

            exec.param = info;
            break;
        }
        }
    }

    sha1_finish(&sha, exec.sha1);

    // inspired by rpcs3 :)
    std::string hash("EXC-0000000000000000000000000000000000000000");
    for (u32 i = 0; i < 20; i++) {
        constexpr auto pal = "0123456789abcdef";
        hash[4 + i * 2] = pal[exec.sha1[i] >> 4];
        hash[5 + i * 2] = pal[exec.sha1[i] & 15];
    }

    LOG_INFO("EXEC module hash {}", hash);

    exec.moduleHash = std::move(hash);

    const auto patch_result = patch_module(elf, exec);
    if (patch_result != loadStatus::Success)
        return patch_result;

    return loadStatus::Success;
}

static loadStatus load_prx(elfObject& elf, prx_module& prx) {

    // prx fingerprint
    sha1_context sha;
    sha1_starts(&sha);

    // calculate code + data size, so we can allocate 'em in one block
    u32 loadSize = 0;
    for (auto& p : elf.programs) {
        if (p.type == PT_LOAD || p.type == PT_SCE_RELRO)
            loadSize += ::align_up(p.memsz, p.align);
    }

    // allocate an aligned chunk
    prx.base = memory::alloc(loadSize, memory::exec);
    LOG_ASSERT(prx.base);

    for (const auto& p : elf.programs) {
        switch (p.type) {
        case PT_LOAD:
        case PT_SCE_RELRO: {
            if (p.memsz) {

                // create a fingerprint of each segment
                sha1_update(&sha, reinterpret_cast<const uint8_t*>(&p.vaddr), sizeof(p.vaddr));
                sha1_update(&sha, reinterpret_cast<const uint8_t*>(&p.memsz), sizeof(p.memsz));
                sha1_update(&sha, p.bin.data(), p.bin.size());

                std::memcpy(prx.base + p.vaddr, p.bin.data(), p.bin.size());
            }
            break;
        }
        case PT_SCE_MODULEPARAM: {
            auto* info = reinterpret_cast<module_param*>(prx.base + p.vaddr);
            if (info->size < 24)
                LOG_WARNING("Bad module_param size {},{}", info->size, sizeof(module_param));

            if (info->magic != 0x3c13f4bf)
                return loadStatus::ErrorModuleInfo;

            LOG_INFO("** sdk version: {0:x}", info->sdk_version);

            prx.param = info;
            break;
        }
        }
    }

    sha1_finish(&sha, prx.sha1);

    // inspired by rpcs3 :)
    std::string hash("PRX-0000000000000000000000000000000000000000");
    for (u32 i = 0; i < 20; i++) {
        constexpr auto pal = "0123456789abcdef";
        hash[4 + i * 2] = pal[prx.sha1[i] >> 4];
        hash[5 + i * 2] = pal[prx.sha1[i] & 15];
    }

    LOG_INFO("PRX module hash {}, loaded at {}", hash, fmt::ptr(prx.base));

    prx.moduleHash = std::move(hash);

    const auto patch_result = patch_module(elf, prx);
    if (patch_result != loadStatus::Success)
        return patch_result;

    return loadStatus::Success;
}

static void set_module_info(elfObject& elf, sce_module& mod) {
    std::memcpy(&mod.header, &elf.header, sizeof(ELFHeader));

    for (auto& p : elf.programs)
        mod.programs.push_back(p);

    for (auto& p : elf.programs) {
        auto set_info = [&](formats::elf_pg& pg, int idx) {
            auto& s = mod.segments[idx];
            s.addr = reinterpret_cast<uintptr_t>(mod.base + p.vaddr);
            s.flags = p.flags;
            s.size = ::align_up(p.memsz, p.align);
        };

        switch (p.type) {
        case PT_LOAD:
            set_info(p, 0);
            break;
        case PT_SCE_RELRO:
            set_info(p, 1);
            break;
        case PT_TLS: {
            auto t = std::make_unique<tls_info>();
            t->vaddr = p.vaddr;
            t->align = p.align;
            t->filesz = p.filesz;
            t->memsz = p.memsz;

            mod.tlsInfo = std::move(t);
            break;
        }
        default:
            continue;
        }
    }

    mod.installExceptionhandler(elf);
}

inline const char* elf_to_string(u32 type) {
    switch (type) {
    case ET_SCE_EXEC:
        return "Executable";
    case ET_SCE_DYNEXEC:
        return "Main module";
    case ET_SCE_RELEXEC:
        return "Relocatable PRX";
    case ET_SCE_STUBLIB:
        return "Stub library";
    case ET_SCE_DYNAMIC:
        return "Dynamic PRX";
    default:
        return "Unknown";
    }
}

bool loadElf(sce_module& elf, std::string_view path) {
    utl::File file(path);
    if (!file.IsOpen()) {
        __debugbreak();
        LOG_ERROR("Failed to open module {}", path);
        return false;
    }

    // load common elf object
    formats::elfObject elfObj;
    const auto elf_result = elfObj.load(file);

    switch (elf_result) {
    case elfError::encrypted:
        LOG_ERROR("Failed to load elf: file was encrypted");
        return false;
    case elfError::badformat:
        LOG_ERROR("Failed to load elf: file is not an elf");
        return false;
    case elfError::badmachine:
        LOG_ERROR("Failed to load elf: elf wasn't built for amd64");
        return false;
    case elfError::stream_pgheaders:
        LOG_ERROR("Failed to load elf: unable to load program headers");
        return false;
    case elfError::stream_data:
        LOG_ERROR("Failed to laod elf: unable to load program data");
        return false;
    default:
        break;
    }

    // validate header type
    bool isPrx = false;

    switch (elfObj.header.type) {
    case ET_SCE_EXEC:
    case ET_SCE_DYNEXEC:
        break;
    case ET_SCE_RELEXEC:
    case ET_SCE_DYNAMIC:
        isPrx = true;
        break;
    default:
        LOG_WARNING("Unknown ET_SCE type ({})", elfObj.header.type);
        return false;
    }

    // load specific module data
    loadStatus err = isPrx ? load_prx(elfObj, reinterpret_cast<prx_module&>(elf))
                           : load_exec(elfObj, reinterpret_cast<exec_module&>(elf));

    switch (err) {
    case loadStatus::ErrorBadMap:
        LOG_ERROR("Failed to load obj: unable to map file");
        return false;
    case loadStatus::ErrorRecompiler:
        LOG_ERROR("Failed to load: unable to recompile code");
        return false;
    case loadStatus::ErrorProcInfo:
        LOG_ERROR("Invalid proc_param magic (not ORBI)");
        return false;
    case loadStatus::ErrorModuleInfo:
        LOG_ERROR("Invalid module_param magic");
        return false;
    default:
        break;
    }

    // digest file info
    if (!elf.digestDynamic(elfObj)) {
        LOG_ERROR("Unable to digest elf {}", path);
        return false;
    }

    // set elf information
    set_module_info(elfObj, elf);

    if (elf.header.entry == 0) {
        if (!isPrx) {
            LOG_ERROR("No entry point in exec");
            return false;
        }

        elf.entry = nullptr;
    } else
        elf.entry = elf.getAddress<u8*>(elf.header.entry);

    LOG_INFO("Loaded {} as {} at {}", elf.name, elf_to_string(elfObj.header.type),
             fmt::ptr(elf.base));
    return true;
}
} // namespace kern