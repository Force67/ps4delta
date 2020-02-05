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
    uint64_t hashId;
    const void* address;
};

struct static_module {
    static_func* funcNodes;
    size_t funcCount;
    const char* namePtr;
};

void reg_static_module(const static_module* info);

struct process_param {
    size_t size;
    uint32_t magic;
    uint32_t version;
    uint32_t sdk_version;
    char pad[60];
};

struct module_param {
    size_t size;
    uint32_t magic;
    uint32_t version;
    uint32_t sdk_version;
    uint32_t unk;
};

struct lib_info {
    const char* name;
    int32_t id;
    bool exported;
};

struct mod_info {
    const char* name;
    int32_t id;
};

// hacky hack
struct tls_info {
    uint32_t vaddr;
    uint32_t memsz;
    uint32_t filesz;
    uint32_t align;
};

static_assert(sizeof(process_param) == 80);
static_assert(sizeof(module_param) == 24);

class sce_module : public s_object {
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

    // module base address
    uint8_t* base = nullptr;

    // module entry point
    uint8_t* entry = 0;
    uint32_t mappedSize = 0;

    // elf data
    ELFHeader header;
    std::vector<ELFPgHeader> programs;

    // unique module identifier
    uint8_t sha1[20]{};

    // sce module fingerprint
    uint8_t fingerprint[20]{};

    // sce module name
    std::string name;

    // sce segments
    segment_info segments[4]{};

    std::unique_ptr<tls_info> tlsInfo;

    uint8_t* initAddr = nullptr;
    uint8_t* finiAddr = nullptr;

    uint8_t* ehFrameheaderAddr;
    uint8_t* ehFrameAddr;
    uint32_t ehFrameheaderSize;
    uint32_t ehFrameSize;

    bool loadNeededPrx();
    void installExceptionhandler(formats::elfObject&);

protected:
    bool digestDynamic(formats::elfObject&);

    uint8_t* dynData = nullptr;

    template<typename T>
    T* getDynPtr(uint32_t disp) {
        return reinterpret_cast<T*>(dynData + disp);
    }

    std::pair<ElfSym*, size_t> symTab;
    std::pair<ElfRel*, size_t> jumpTab;
    std::pair<ElfRel*, size_t> relaTab;
    std::pair<uint8_t*, size_t> hashTab;
    std::pair<char*, size_t> strTab;

    std::vector<std::string> sharedObjects;
    std::vector<lib_info> libs;
    std::vector<mod_info> mods;
};

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

bool init_modules(process&, bool phase2 = true);
} // namespace kern

#define MODULE_INIT(tname)                                                                         \
                                                                                                   \
    static const hle::static_module info_##tname{                                                  \
        (hle::static_func*)&functions, (sizeof(functions) / sizeof(hle::static_func)), #tname};    \
                                                                                                   \
    static utl::init_function init_##tname([]() { hle::reg_static_module(&info_##tname); })