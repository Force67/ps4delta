
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>
#include <logger/logger.h>

#include "memory.h"

namespace memory {
using namespace utl;

// constants
constexpr size_t AMD64_PAGE_SIZE = 4096;
constexpr size_t AMD64_PAGE_MASK = (AMD64_PAGE_SIZE - 1);

constexpr size_t PS4_PAGE_SIZE = 1024 * 16;
constexpr size_t PS4_PAGE_MASK = (PS4_PAGE_SIZE - 1);

static const init_info* g_areas = nullptr;

// pre-alloc hook
void preinit(const init_info& info) {
    g_areas = &info;
}

static vmManager* s_instance = nullptr;

inline static void commit(u8* addr, size_t size) {
    utl::allocMem(static_cast<void*>(addr), size, utl::page_read_write, utl::allocType::commit);
}

inline static void protect(u8* addr, size_t len, utl::page_protection pt) {
    utl::protectMem(static_cast<void*>(addr), len, pt);
}

inline static void decommit(u8* addr) {
    utl::freeMem(static_cast<void*>(addr), false);
}

block::block(const mem_area& area)
    : base(area.ptr), size(area.size), pages(area.size / AMD64_PAGE_SIZE /*PS4_PAGE_SIZE*/) {}

block::~block() {
    for (auto& a : allocations)
        decommit(a.first);
}

u8* block::xalloc(u8* targetAddress, size_t size, u32 flags, u8 pflags, u32 align) {

    const uintptr_t addr = reinterpret_cast<uintptr_t>(targetAddress);
    if (!size || pflags & page_allocated || (size | addr) % PS4_PAGE_SIZE)
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

    const u32 disp = targetAddress - base;

    for (size_t i = disp / AMD64_PAGE_SIZE;
         i < disp / AMD64_PAGE_SIZE + block_size / AMD64_PAGE_SIZE; i++) {
        auto& pinfo = pages[i];

        size_t hits = 0;
        // check if we get enough slots
        for (size_t k = i; k < (i + page_count); k++) {
            if (!pages[k] || pages[k] & page_flags::page_allocated)
                hits++;
        }

        // free space is too small
        if (hits != page_count) {
            __debugbreak();
            return nullptr;
        }

        // mark pages as used
        for (size_t k = i; k < (i + page_count); k++)
            pages[k].exchange(pflags | page_flags::page_allocated);

        commit(targetAddress, block_size);
        break;
    }

    // clear out private memory
    if (flags & anon)
        std::memset(targetAddress, 0, block_size);

    page_protection tp = utl::page_read;
    if (pflags & page_writable)
        tp = utl::page_read_write;
    if (pflags & page_executable)
        tp |= utl::page_execute;

    // apply native page protection
    protect(targetAddress, block_size, tp);

    // register with alloc store
    allocations.insert({targetAddress, page_count});
    return targetAddress;
}

u8* block::alloc(size_t size, u32 flags, u8 pflags, u32 align) {
    if (!size || pflags & page_allocated)
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

    u8* allocation = nullptr;

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
            pages[k].exchange(pflags | page_flags::page_allocated);

        // and reserve the real memory
        allocation = (u8*)(this->base + (i * AMD64_PAGE_SIZE));
        commit(allocation, block_size);
        break;
    }

    if (!allocation)
        return nullptr;

    // clear out private memory
    if (flags & anon)
        std::memset(allocation, 0, block_size);

    page_protection tp = utl::page_read;
    if (pflags & page_writable)
        tp = utl::page_read_write;
    if (pflags & page_executable)
        tp |= utl::page_execute;

    // apply native page protection
    protect(allocation, block_size, tp);

    // register with alloc store
    allocations.insert({allocation, page_count});

    return allocation;
}

void block::free(u8* ptr) {
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

    LOG_INFO("ProcArea {} ({})\nApp {} ({})\nExecArea {} ({})\nDebugArea {} ({})",
             fmt::ptr(g_areas->proc.ptr), g_areas->proc.size, 
             fmt::ptr(g_areas->procMirror.ptr), g_areas->procMirror.size, 
             fmt::ptr(g_areas->exec.ptr), g_areas->exec.size, 
             fmt::ptr(g_areas->debug.ptr),g_areas->debug.size);

    /*auto* memBase = reserve(0x2'0000'0000);
    if (!memBase)
        return false;

    userStack = reserve(userStackSize);

    */
    return true;
}

// ref counted so the block can't get dealloc'd while referenced
SharedPtr<block> vmManager::getBlock(u8* addr, memory_location loc) {

    // by pool index
    if (loc != memory_location::any && loc < blocks.size()) {
        auto block = blocks[loc];
        if (!block)
            __debugbreak();

        return block;
    }

    // by addr
    return addr ? getBlock(addr) : nullptr;
}

SharedPtr<block> vmManager::getBlock(u8* addr) {
    auto iter = std::find_if(blocks.begin(), blocks.end(),
                             [&addr](const auto& b) { return b->check(addr); });

    if (iter != blocks.end())
        return (*iter);

    return nullptr;
}

u8* vmManager::alloc(u8* desired, size_t size, memory_location location, u32 align) {
    auto block = getBlock(desired, location);

    if (!block)
        __debugbreak();

    const u32 hack = page_executable | page_writable | page_readable;

    if (!desired /*TODO: check for fixed*/)
        return block->alloc(size, 0, (page_flags)hack, align);
    else
        return block->xalloc(desired, size, 0, (page_flags)hack, align);
}

u8* alloc(size_t size, memory_location where, u32 align) {
    return s_instance->alloc(nullptr, size, where, align);
}

u8* falloc(u8* base, size_t size, memory_location where, u32 align) {
    return s_instance->alloc(base, size, where, align);
}

vmManager* manager() {
    return s_instance;
}

} // namespace memory