#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <mutex>
#include <atomic>
#include <vector>
#include <unordered_map>
#include <utl/mem.h>

namespace memory {
using addr = uintptr_t;

struct mem_area {
    u8* ptr;
    size_t size;
};

struct init_info {
    mem_area proc;
    mem_area procMirror;
    mem_area exec;
    mem_area debug;
};

enum memory_location : int {
    user, //< starts at 0x200000000
    app,
    exec,
    debug,
    any = 0xFFFFFF
};

// co relate to free-bsd
enum memory_flags : u32 {
    fixed = 0x10,
    stack = 0x400,
    noextend = 0x100,
    anon = 0x1000,
};

enum page_flags : u8 {
    page_free,
    page_readable = (1 << 0),
    page_writable = (1 << 1),
    page_executable = (1 << 2),
    page_allocated = (1 << 7),
};

class block {
public:
    block(const mem_area& area);
    ~block();

    u32 memUsed() const {
        return 0;
    }

    u8* alloc(size_t size, u32, u8, u32 align);
    u8* xalloc(u8*, size_t size, u32, u8, u32);

    void free(u8*);

    inline bool check(u8* ptr) {
        return ptr >= base && ptr <= base + size;
    }

public:
    const u8* base;
    const size_t size;

private:
    std::mutex writer_lock;
    std::vector<std::atomic<u8>> pages;
    std::unordered_map<u8*, u8> allocations;
};

class vmManager {
public:
    vmManager();
    bool init();

    static vmManager* get();
    
    u8* alloc(u8 *ptr, size_t size, memory_location, u32 align);
    SharedPtr<block> getBlock(u8* loc, memory_location);
    SharedPtr<block> getBlock(u8* loc);

private:

   // u8* alloc(u8* base, size_t size, utl::ppt);

private:
    std::vector<SharedPtr<block>> blocks;
};

// easy access wrappers
u8* alloc(size_t size, memory_location = memory_location::any, u32 align = 0x1000);
u8* falloc(u8* base, size_t size, memory_location = memory_location::any, u32 align = 0x1000);

vmManager* manager();

void preinit(const init_info&);
} // namespace vm