
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include "memory.h"

namespace memory {
using namespace utl;

// constants
constexpr size_t AMD64_PAGE_SIZE = 4096;
constexpr size_t AMD64_PAGE_MASK = (AMD64_PAGE_SIZE - 1);

constexpr size_t PS4_PAGE_SIZE = 1024 * 16;
constexpr size_t PS4_PAGE_MASK = (PS4_PAGE_SIZE - 1);

constexpr size_t USER_STACK = 20 * 1024 * 1024;

static const init_info* g_areas = nullptr;

// pre-alloc hook
void preinit(const init_info& info) {
    g_areas = &info;
}

static vmManager* s_instance = nullptr;

inline static void commit(uint8_t* addr, size_t size) {
    utl::allocMem(static_cast<void*>(addr), size, utl::page_read_write, utl::allocType::commit);
}

inline static void protect(uint8_t* addr, size_t len, utl::page_protection pt) {
    utl::protectMem(static_cast<void*>(addr), len, pt);
}

inline static void decommit(uint8_t* addr) {
    utl::freeMem(static_cast<void*>(addr), false);
}

block::block(const mem_area& area)
    : base(area.ptr), size(area.size), pages(area.size / AMD64_PAGE_SIZE /*PS4_PAGE_SIZE*/) {}

block::~block() {
    for (auto& a : allocations)
        decommit(a.first);
}

uint8_t* block::xalloc(uint8_t* desired_addr, size_t size, uint32_t flags, page_flags prot,
                       uint32_t align) {

    const uintptr_t addr = reinterpret_cast<uintptr_t>(desired_addr);
    if (!size || prot & page_allocated || (size | addr) % PS4_PAGE_SIZE)
        return nullptr;

    if (flags & stack || flags & noextend)
        flags |= anon;

    // optimization...
    const size_t page_mask = flags & noextend ? AMD64_PAGE_MASK : PS4_PAGE_MASK;

    // adjust size for rounding
    const size_t block_size = ::align_up(size, page_mask);

    size_t page_count = 1;

    // map small to big ps4 pages
    if (block_size > AMD64_PAGE_SIZE)
        page_count = block_size / AMD64_PAGE_SIZE;

    for (size_t i = addr / AMD64_PAGE_SIZE; i < addr / AMD64_PAGE_SIZE + block_size; i++) {
        auto& pinfo = pages[i];

        if (!pinfo) {
            /*attempts to map at fixed address failed*/
            __debugbreak();
            return nullptr;
        }

        size_t hits = 0;
        // check if we get enough slots
        for (size_t k = i; k < (i + page_count); k++) {
            if (!pages[k])
                hits++;
        }

        // free space is too small
        if (hits != page_count) {
            __debugbreak();
            return nullptr;
        }

        // mark pages as used
        for (size_t k = i; k < (i + page_count); k++)
            pages[k].store(prot);

        commit(desired_addr, block_size);
        break;
    }

    // clear out private memory
    if (flags & anon)
        std::memset(desired_addr, 0, block_size);

    page_protection tp = utl::page_read;
    if (prot & page_writable)
        tp = utl::page_read_write;
    if (prot & page_executable)
        tp |= utl::page_execute;

    // apply native page protection
    protect(desired_addr, block_size, tp);

    // register with alloc store
    allocations.insert({desired_addr, page_count});
    return desired_addr;
}

uint8_t* block::alloc(size_t size, uint32_t flags, page_flags prot, uint32_t align) {
    if (!size || prot & page_allocated)
        return nullptr;

    if (flags & stack || flags & noextend)
        flags |= anon;

    // optimization...
    const size_t page_mask = flags & noextend ? AMD64_PAGE_MASK : PS4_PAGE_MASK;

    // adjust size for rounding
    const size_t block_size = ::align_up(size, page_mask);

    size_t page_count = 1;

    // map small to big ps4 pages
    if (block_size > AMD64_PAGE_SIZE)
        page_count = block_size / AMD64_PAGE_SIZE;

    uint8_t* allocation = nullptr;

    // now allocate it
    for (size_t i = 0; i < pages.size(); i++) {
        auto& pinfo = pages[i];

        if (pinfo)
            continue;

        size_t hits = 0;

        // check if we get enough slots
        for (size_t k = i; k < (i + page_count); k++) {
            if (!pages[k])
                hits++;
        }

        if (hits != page_count) {
            // is this smart?
            // i guess not...
            i += page_count;
            continue;
        }

        // mark pages as used
        for (size_t k = i; k < (i + page_count); k++)
            pages[k].store(prot);

        // and reserve the real memory
        allocation = (uint8_t*)(this->base + (i * AMD64_PAGE_SIZE));
        commit(allocation, block_size);
        break;
    }

    if (!allocation)
        return nullptr;

    // clear out private memory
    if (flags & anon)
        std::memset(allocation, 0, block_size);

    page_protection tp = utl::page_read;
    if (prot & page_writable)
        tp = utl::page_read_write;
    if (prot & page_executable)
        tp |= utl::page_execute;

    // apply native page protection
    protect(allocation, block_size, tp);

    // register with alloc store
    allocations.insert({allocation, page_count});

    return allocation;
}

void block::free(uint8_t* ptr) {
    std::lock_guard lock(writer_lock);

    auto it = allocations.find(ptr);
    if (it != allocations.end()) {

        // free small pages
        const size_t i = (it->first - this->base) / AMD64_PAGE_SIZE;
        for (size_t k = i; k < (i + it->second); k++)
            pages[k].store(page_free);

        decommit(it->first);
        allocations.erase(it);
    }
}

vmManager::vmManager() {
    s_instance = this;
}

vmManager* vmManager::get() {
    return s_instance;
}

bool vmManager::init() {
    // register the 'default' memory areas available for every
    // ps4 user mode process
    blocks.push_back(std::make_shared<block>(g_areas->proc));
    blocks.push_back(std::make_shared<block>(g_areas->procMirror));
    blocks.push_back(std::make_shared<block>(g_areas->exec));
    blocks.push_back(std::make_shared<block>(g_areas->debug));

    /*auto* memBase = reserve(0x2'0000'0000);
    if (!memBase)
        return false;

    userStack = reserve(userStackSize);

    */
    return true;
}

// ref counted so the block can't get dealloc'd while referenced
SharedPtr<block> vmManager::getBlock(uint8_t* addr, memory_location loc) {

    // by pool index
    if (loc != memory_location::any && loc < blocks.size()) {
        auto block = blocks[loc];
        if (!block)
            __debugbreak();

        return block;
    }

    if (!addr)
        return nullptr;

    // by address
    for (auto& b : blocks) {
        if (b->check(addr))
            return b;
    }

    return nullptr;
}

uint8_t* vmManager::alloc(uint8_t* desired, size_t size, memory_location location, uint32_t align) {
    auto block = getBlock(desired, location);

    if (!block)
        __debugbreak();

    const uint32_t hack = page_executable | page_writable | page_readable;

    if (!desired /*TODO: check for fixed*/)
        return block->alloc(size, 0, (page_flags)hack, align);
    else
        return block->xalloc(desired, size, 0, (page_flags)hack, align);
}

uint8_t* alloc(size_t size, memory_location where, uint32_t align) {
    return s_instance->alloc(nullptr, size, where, align);
}

uint8_t* falloc(uint8_t* base, size_t size, memory_location where, uint32_t align) {
    return s_instance->alloc(base, size, where, align);
}

} // namespace memory