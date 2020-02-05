#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <memory>
#include <mutex>
#include <logger/logger.h>
#include <utl/object_ref.h>

#include "object.h"

namespace kern {
class idManager {
public:
    idManager();
    ~idManager();

    void reset();
    void purge();
    bool add(s_object*, uint32_t&);
    bool remove(uint32_t);
    bool release(uint32_t);
    bool keep(uint32_t);
    s_object* get(uint32_t);

private:
    bool resize(uint32_t newCap);
    bool findSlot(uint32_t& out);

    std::mutex omutex;

    struct entry {
        int refCount = 0;
        s_object* obj = nullptr;
    };

    entry* findEntry(uint32_t);

    uint32_t tableCap = 0;
    uint32_t lastFreeEntry = 0;
    entry* table = nullptr;
};
}