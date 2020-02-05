#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <utl/file.h>

#include <elf_types.h>
#include <sce_types.h>

namespace formats {
enum elfError { success, encrypted, badformat, badmachine, stream_pgheaders, stream_data };

struct elf_pg : ELFPgHeader {
    using base = ELFPgHeader;

    elf_pg() = default;

    elf_pg(uint32_t type, uint32_t flags, size_t vaddr, size_t memsz, size_t align,
           std::vector<uint8_t>&& bin)
        : bin(std::move(bin)) {

        base::type = type;
        base::flags = flags;
        base::vaddr = vaddr;
        base::memsz = memsz;
        base::align = align;
        base::filesz = static_cast<size_t>(bin.size());
        base::paddr = 0;
        base::offset = -1;
    }

    template <typename T>
    T *at(uint32_t offset) {
        if (offset >= base::offset && offset <= base::offset + base::filesz)
            return reinterpret_cast<T*>(&bin[offset - base::offset]);
        return nullptr;
    }

    std::vector<uint8_t> bin;
};

class elfObject {
public:
    elfError load(utl::File& file, uint64_t offset = 0) {
        file.Seek(offset);
        file.Read(header);

        // encrypted, should have been decrypted by firmware installer earlier
        if (header.magic == 0x1D3D154F)
            return elfError::encrypted;

        if (header.magic != ELF_MAGIC)
            return elfError::badformat;

        if (header.machine != ELF_MACHINE_X86_64)
            return elfError::badmachine;

        std::vector<ELFPgHeader> progs(header.phnum);
        file.Seek(offset + header.phoff);
        if (!file.Read(progs))
            return elfError::stream_pgheaders;

        programs.resize(progs.size());
        for (auto& hdr : progs) {

            // cheat: skip empty segments
            if (!hdr.filesz)
                continue;

            auto& p = programs.emplace_back();
            std::memcpy(&p, &hdr, sizeof(ELFPgHeader));

            file.Seek(offset + hdr.offset);
            p.bin.resize(hdr.filesz);

            if (!file.Read(p.bin))
                return elfError::stream_data;
        }

        programs.shrink_to_fit();
        return elfError::success;
    }

    template<typename T>
    T* at(uint32_t offset) {
        for (auto& p : programs) {
            if (offset >= p.offset && offset <= p.offset + p.filesz) {
                return reinterpret_cast<T*>(&p.bin[offset - p.offset]);
            }
        }
        return nullptr;
    }

    elf_pg* get_seg(uint32_t type) {
        for (auto& p : programs) {
            if (p.type == type)
                return &p;
        }

        return nullptr;
    }

public:
    ELFHeader header{};
    std::vector<elf_pg> programs;

private:
};
} // namespace formats