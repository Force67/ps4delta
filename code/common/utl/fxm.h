#pragma once

/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <memory>

namespace utl {
template<typename Type>
class fxm {
public:
    using ptr_t = std::unique_ptr<Type>;

    fxm() = delete;
    ~fxm() = delete;

    static inline void set(ptr_t set) {
        inst = std::move(set);
    }

    template<typename Imp = Type, typename... Args>
    static inline Imp& make(Args && ...args) {
        inst = std::make_unique<Imp>(std::forward<Args>(args)...);
        return *inst;
    }

    static bool empty() noexcept {
        return !static_cast<bool>(inst);
    }

    static inline Type& get() noexcept {
        return *inst; 
    }

    static void reset() {
        inst.reset();
    }

private:
    static inline ptr_t inst;
};
}