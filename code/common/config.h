#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <type_traits>
#include <yaml-cpp/yaml.h>

namespace config {
struct optBase {
    inline explicit optBase()
        : next(root()) {
        root() = this;
    }

    virtual void parseNode(const YAML::Node&) = 0;
    virtual void dumpNode(YAML::Node&) = 0;

    static inline optBase*& root() noexcept {
        static optBase* s_root{nullptr};
        return s_root;
    }

    optBase* next = nullptr;
    const char* name;
    const char* desc;
};

template <typename DataType>
class opt : public optBase {
public:
    // non copyable
    opt(const opt&) = delete;
    opt& operator=(const opt&) = delete;

    constexpr inline explicit opt(const char* name, const char* desc, const DataType v)
        : optBase(), data(v) {
        optBase::name = name;
        optBase::desc = desc; 
    }

    DataType* operator->() const noexcept {
        return std::pointer_traits<DataType*>::pointer_to(*data);
    }

    DataType& operator&() const noexcept {
        return data;
    }

    // TODO: check if type bool?
    operator DataType() const noexcept {
        return data;
    }

    // convert to
    void parseNode(const YAML::Node& v) override {
         data = v.as<DataType>(data);
    }

    // convert at
    void dumpNode(YAML::Node& v) override {
         v = YAML::Node(data);
    }

private:
    DataType data;
};

bool load();
bool save();
} // namespace config