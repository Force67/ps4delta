
/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <algorithm>
#include <cstdio>
#include "file.h"

namespace utl {
namespace {
// file on disk impl
class PhysFile final : public fileBase {
    size_t sizeTracker;
    std::FILE* fptr;

public:
    PhysFile(std::string_view path, fileMode mode) : fptr(nullptr) {
        Open(path, mode);
    }

    ~PhysFile() {
        Close();
    }

    bool Open(std::string_view path, fileMode mode) override {
        // convert access mode
        const char* modeStr = "a+";
        if (mode == fileMode::read)
            modeStr = "rb";
        else if (mode == fileMode::write)
            modeStr = "wb";

        fopen_s(&fptr, std::string(path).data(), modeStr);
        if (!fptr) return false;

        // we can cache the size now
        if (mode == fileMode::read) {

            // determine initial size
            std::fseek(fptr, 0, SEEK_END);
            sizeTracker = static_cast<size_t>(std::ftell(fptr));
        } else {
            sizeTracker = 0;
        }

        // ensure that we can always start from offset 0
        std::fseek(fptr, 0, SEEK_SET);

        return true;
    }

    void Close() override {
        if (fptr) {
            std::fclose(fptr);
            sizeTracker = 0;
        }
    }

    uint64_t Read(void* buf, size_t size) override {
        uint64_t x = std::fread(buf, size, 1, fptr);
#if DELTA_DBG
        if (x != 1) {
            __debugbreak();
        }
#endif

        if (x == 1)
            return size;

        return x;
    }

    uint64_t Write(const void* buf, size_t size) override {
        sizeTracker += size;
        return std::fwrite(buf, size, 1, fptr);
    }

    uint64_t Seek(int64_t ofs, seekMode mode) override {
        // translate mode
        int32_t origin = 0;
        switch (mode) {
        case seekMode::seek_cur:
            origin = SEEK_CUR;
            break;
        case seekMode::seek_end:
            origin = SEEK_END;
            break;
        case seekMode::seek_set:
            origin = SEEK_SET;
            break;
        default:
            return 0;
        }

        uint64_t x = std::fseek(fptr, static_cast<uint32_t>(ofs), origin);
        if (x == 0)
            return static_cast<uint64_t>(std::ftell(fptr));

        return x;
    }

    uint64_t Tell() override {
        return std::ftell(fptr);
    }

    uint64_t GetSize() override {
        /*if (sizeTracker == 0) {
                std::FILE* fptr = static_cast<std::FILE*>(handle);

                std::fseek(fptr, 0, SEEK_END);
                sizeTracker = static_cast<size_t>(std::ftell(fptr));
                std::fseek(fptr, 0, SEEK_SET);
        }*/

        return sizeTracker;
    }

    native_handle GetNativeHandle() override {
        return static_cast<native_handle>(fptr);
    }

    bool IsOpen() override {
        return fptr != nullptr;
    }
};

// from memory
class MemStream final : public fileBase {
    uint64_t pos;
    const char* const ptr;
    const uint64_t size;

public:
    MemStream(const void* ptr, uint64_t size) : ptr(static_cast<const char*>(ptr)), size(size) {}

    uint64_t Read(void* buf, size_t count) override {
        if (pos < size) {
            // get readable size
            if (const uint64_t result = std::min<uint64_t>(count, size - pos)) {
                std::memcpy(buf, ptr + pos, result);
                pos += result;
                return result;
            }
        }

        return 0;
    }

    // TODO
    uint64_t Write(const void*, size_t) override {
        return 0;
    }

    uint64_t Seek(int64_t ofs, seekMode mode) override {
        switch (mode) {
        case seekMode::seek_cur:
            pos += ofs;
            break;
        case seekMode::seek_end:
            pos = size;
            break;
        case seekMode::seek_set:
            pos = ofs;
            break;
        default:
            return 0;
        }

        return 0;
    }

    uint64_t Tell() override {
        return pos;
    }

    uint64_t GetSize() override {
        return size;
    }

    native_handle GetNativeHandle() override {
        return nullptr;
    }
};
}

File::File(std::string_view path, fileMode mode /* = fileMode::read */)
    : file(std::make_unique<PhysFile>(path, mode)) {}

File::File(const void* ptr, size_t size) : file(std::make_unique<MemStream>(ptr, size)) {}

File::File(std::unique_ptr<fileBase>&& base) : file(std::move(base)) {}

File::~File() {
    Close();
}
}