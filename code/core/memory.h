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
    uint8_t* ptr;
    size_t size;
};

struct init_info {
    mem_area proc;
    mem_area procMirror;
    mem_area exec;
    mem_area debug;
};

enum memory_location : int {
    any,
    user,
    app,
    exec,
    debug
};

// co relate to free-bsd
enum memory_flags : uint32_t {
    fixed = 0x10,
    stack = 0x400,
    noextend = 0x100,
    anon = 0x1000,
};

enum page_flags : uint8_t {
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

    uint32_t memUsed() const {
        return 0;
    }

    uint8_t* alloc(size_t size, uint32_t, page_flags, uint32_t align);
    uint8_t* xalloc(uint8_t*, size_t size, uint32_t, page_flags, uint32_t);

    void free(uint8_t*);

    inline bool check(uint8_t* ptr) {
        return ptr >= base && ptr <= base + size;
    }

public:
    const uint8_t* base;
    const size_t size;

private:
    std::mutex writer_lock;
    std::vector<std::atomic<page_flags>> pages;
    std::unordered_map<uint8_t*, uint8_t> allocations;
};

class vmManager {
public:
    vmManager();
    bool init();

    static vmManager* get();
    
    uint8_t* alloc(uint8_t *ptr, size_t size, memory_location, uint32_t align);
    SharedPtr<block> getBlock(uint8_t* loc, memory_location);

private:

   // uint8_t* alloc(uint8_t* base, size_t size, utl::ppt);

private:
    std::vector<SharedPtr<block>> blocks;
};

uint8_t* alloc(size_t size, memory_location = memory_location::any, uint32_t align = 0x1000);
uint8_t* falloc(uint8_t* base, size_t size, memory_location = memory_location::any, uint32_t align = 0x1000);

void preinit(const init_info&);

} // namespace vm