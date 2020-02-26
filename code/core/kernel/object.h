#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <atomic>
#include <mutex>
#include <string>
#include <vector>
#include <utl/object_ref.h>

namespace kern {
class process;

class object {
public:
    using handleList = std::vector<uint32_t>;

    enum class kind {
        file,
        device,
        namedobj,
        dynlib
    };

    explicit object(kind);

    void retain();
    void release();
    void retainHandle();
    void releaseHandle();

    handleList& handles() {
        return handleCollection;
    }

    uint32_t handle() const {
        return handleCollection[0];
    }

public:
    kind type;
    std::string name;

private:
    handleList handleCollection;
    std::atomic<int32_t> refCount;
};

template <typename T>
utl::object_ref<T> retain_object(T* ptr) {
    if (ptr)
        ptr->retain();
    return utl::object_ref<T>(ptr);
}
}