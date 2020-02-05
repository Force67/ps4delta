#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#if 0
#include <string>

#include <elf_types.h>
#include <sce_types.h>
#include "proc.h"

namespace utl {
class File;
}

namespace krnl {
struct moduleSeg {
    uint8_t* addr;
    uint32_t size;
};

struct moduleInfo {
    std::string name;
    uint32_t handle;
    uint8_t* base;
    uint8_t* entry;
    uint16_t tlsSlot;
    uint32_t codeSize;

    uint8_t* ripZone;
    size_t ripZoneSize;

    uint8_t* procParam;
    uint32_t procParamSize;

    uint8_t* initAddr;
    uint8_t* finiAddr;

    moduleSeg textSeg;
    moduleSeg dataSeg;

    uint8_t* tlsAddr;
    size_t tlsSizeMem;
    size_t tlsSizeFile;
    uint32_t tlsalign;

    uint8_t* ehFrameheaderAddr;
    uint8_t* ehFrameAddr;
    uint32_t ehFrameheaderSize;
    uint32_t ehFrameSize;

    uint8_t fingerprint[20];
};

class smodule {
    friend class proc;

public:
    explicit smodule(proc*);

    bool fromFile(const std::string&);
    bool fromMem(std::unique_ptr<uint8_t[]>);

    uintptr_t getSymbol(uint64_t);
    uintptr_t getSymbolFullName(const char* name);
    uintptr_t getSymbol2(const char* name);
    bool resolveObfSymbol(const char* name, uintptr_t& ptrOut);

    bool applyRelocations();
    bool resolveImports();

    bool unload();

    inline moduleInfo& getInfo() {
        return info;
    }

    inline bool isDynlib() {
        return elf->type == ET_SCE_DYNAMIC;
    }

    /*traits -> object_ref TODO: properly implement*/
    void release(){};
    void retain(){};

private:
    moduleInfo info{};

    void digestDynamic();
    void logDbgInfo();
    void installEHFrame();
    bool setupTLS();
    bool mapImage();

    template <typename Type, typename TAdd>
    Type* getOffset(const TAdd dist) {
        return (Type*)(data.get() + dist);
    }

    template <typename Type, typename TAdd>
    Type* getAddress(const TAdd dist) {
        return (Type*)(info.base + dist);
    }

    template <typename Type, typename TAdd>
    Type getAddressNPTR(const TAdd dist) {
        return (Type)(info.base + dist);
    }

    template <typename Type = ELFPgHeader>
    Type* getSegment(ElfSegType type) {
        for (uint16_t i = 0; i < elf->phnum; i++) {
            auto s = &segments[i];
            if (s->type == type)
                return reinterpret_cast<Type*>(s);
        }

        return nullptr;
    }

private:
    std::unique_ptr<uint8_t[]> data;

private:
    proc* process;
    ELFHeader* elf;
    ELFPgHeader* segments;

    struct libInfo {
        const char* name;
        int32_t id;
        uint16_t attr;
        bool exported;
    };

    struct modInfo {
        const char* name;
        int32_t id;
        uint16_t attr;
    };

    std::vector<modInfo> impModules;
    std::vector<libInfo> impLibs;
    std::vector<std::string> sharedObjects;

    ElfRel* jmpslots;
    ElfRel* rela;
    ElfSym* symbols;
    uint8_t* hashes;

    struct table {
        char* ptr;
        size_t size;
    };

    table strtab;
    table symtab;

    uint32_t numJmpSlots;
    uint32_t numSymbols;
    uint32_t numRela;
};
}
#endif