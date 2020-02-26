#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <atomic>

#include <logger/logger.h>

#include <utl/file.h>
#include <utl/init_func.h>
#include <utl/object_ref.h>

#include "formats/elf_object.h"
#include "sys/sys_dynlib.h"

#include "object.h"

namespace kern {
class process;

struct static_func {
    u64 hashId;
    const void* address;
};

struct static_module {
    static_module(const char* name, static_func* funcs, size_t count)
        : funcNodes(funcs), funcCount(count), namePtr(name)
    {}

    static_func* funcNodes;
    size_t funcCount;
    const char* namePtr;
};

struct process_param {
    size_t size;
    u32 magic;
    u32 version;
    u32 sdk_version;
    char pad[60];
};

struct module_param {
    size_t size;
    u32 magic;
    u32 version;
    u32 sdk_version;
    u32 unk;
};

struct lib_info {
    const char* name;
    i32 id;
    bool exported;
};

struct mod_info {
    const char* name;
    i32 id;
};

// hacky hack
struct tls_info {
    u32 vaddr;
    u32 memsz;
    u32 filesz;
    u32 align;
};

static_assert(sizeof(process_param) == 80);
static_assert(sizeof(module_param) == 24);

class sce_module : public object {
public:
    sce_module(process&);

    template <typename Type, typename TAdd>
    Type getAddress(const TAdd disp) {
        return reinterpret_cast<Type>(base + disp);
    }

    uintptr_t getSymbol(const char* fullName);
    bool resolveObfSymbol(const char* name, uintptr_t& ptrOut);

    bool resolveImports();
    bool doRelocations();

public:
    // is the module being executed?
    std::atomic<bool> started = false;

    // module load bse
    u8* base = nullptr;

    // module entry point
    u8* entry = nullptr;
    u32 mappedSize = 0;

    // loader fingerprint
    u8 sha1[20]{};

    // sce fingerprint
    u8 fingerprint[20]{};

    // delta module fingerprint
    std::string moduleHash;

    // sce module name
    std::string name;

    // sce segments
    segment_info segments[4]{};

    u8* initAddr = nullptr;
    u8* finiAddr = nullptr;

    u8* ehFrameheaderAddr;
    u8* ehFrameAddr;
    u32 ehFrameheaderSize;
    u32 ehFrameSize;

    u8* tlsAddr = nullptr;
    u32 tlsSize;
    u32 tlsfSize;
    u32 tlsAlign;
    u16 tlsSlot;

    bool loadNeededPrx();
    void installExceptionhandler(formats::elfObject&);
    bool digestDynamic(formats::elfObject&);

    inline process* parent() const { return parentProc; }

protected:
    // process owning the module
    process* parentProc = nullptr;

    u8* dynData = nullptr;

    template<typename T>
    T* getDynPtr(u32 disp) {
        return reinterpret_cast<T*>(dynData + disp);
    }

    std::pair<ElfSym*, u32> symTab;
    std::pair<ElfRel*, u32> jumpTab;
    std::pair<ElfRel*, u32> relaTab;
    std::pair<u8*, u32> hashTab;
    std::pair<char*, u32> strTab;

    std::vector<std::string> sharedObjects;
    std::vector<lib_info> libs;
    std::vector<mod_info> mods;
};

constexpr auto y = sizeof(std::pair<u8*, u32>);

constexpr auto x = sizeof(sce_module);

class exec_module final : public sce_module {
public:
    exec_module(process& p) : sce_module(p) {}

    static SharedPtr<exec_module> load(process&, std::string_view path);

public:
    process_param* param = nullptr;
    std::string comment;
};

class prx_module final : public sce_module {
public:
    prx_module(process& p) : sce_module(p) {}

    static SharedPtr<prx_module> load(process&, std::string_view path);

public:
    module_param* param = nullptr;
    dynlib_info_ex info;
};

struct prx_static_modules {
    static static_module prx_libkernel;
};
} // namespace kern

#define BIND_FUNC(nid, func) \
{nid, (const void*)&ptr_##func},

#define IMP_FUNC(x) const void* ptr_##x = (const void*)&x;
#define DECL_FUNC(x) extern const void* ptr_##x;

#define MODULE_INIT(x) kern::static_module kern::prx_static_modules::prx_##x( \
    #x, (kern::static_func*)&functions,\
    (sizeof(functions) / sizeof(kern::static_func)));