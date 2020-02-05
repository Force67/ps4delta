
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <vector>
#include <base.h>
#include <crypto/sha1.h>
#include <utl/mem.h>

#include "formats/elf_object.h"
#include "sce_module.h"

#include "arch/native/native_lift.h"
#include "util/nid_util.h"

#include "core.h"
#include "memory.h"

static std::vector<const kern::static_module*> g_staticModules;

namespace kern {
enum class loadError {
    ok,
    nocode,
    alloc_failure,
    map_failure,
    cant_lift,
    proc_param,
    module_param
};

using namespace formats;

void reg_static_module(const static_module* info) {
    g_staticModules.push_back(info);
}

static void initialize_static_modules() {
    if (g_staticModules.empty()) {
        // run registration
        utl::init_function::init();
    }

    // TODO: context insertion
}

static uintptr_t get_static_func(const char* lib, uint64_t hid) {
    const static_module* module = nullptr;

    // find the right table
    for (const auto& t : g_staticModules) {
        if (std::strcmp(lib, t->namePtr) == 0) {
            module = t;
            break;
        }
    }

    if (module) {
        // search the table
        for (int i = 0; i < module->funcCount; i++) {
            auto* f = &module->funcNodes[i];
            if (f->hashId == hid) {
                return reinterpret_cast<uintptr_t>(f->address);
            }
        }
    }

    return 0;
}

uintptr_t sce_module::getSymbol(const char* fullName) {
    auto elfHash = [](const char* name) {
        auto p = (const uint8_t*)name;
        uint32_t h = 0;
        uint32_t g;
        while (*p != '\0') {
            h = (h << 4) + *p++;
            if ((g = h & 0xF0000000ull) != 0) {
                h ^= g >> 24;
            }
            h &= ~g;
        }
        return h;
    };

    auto hash = elfHash(fullName);

    auto* htab = reinterpret_cast<uint32_t*>(hashTab.first);
    uint32_t nbucket = htab[0];
    uint32_t nchain = htab[1];
    uint32_t* bucket = &htab[2];
    uint32_t* chain = &bucket[nbucket];

    for (uint32_t i = bucket[hash % nbucket]; i; i = chain[i]) {
        const auto* s = &symTab.first[i];

        if (i > nchain)
            return 0;

        if (!s->st_value)
            continue;

        const char* sname = &strTab.first[s->st_name];
        if (std::strncmp(sname, fullName, 11) == 0) {
            return getAddress<uintptr_t>(s->st_value);
        }
    }

    return 0;
}

static bool decodeNid(const char* name, uint64_t& lid, uint64_t& mid) {
    /*nid's are always 16 bytes long so we should get away
     with hard coding the access offset*/

    bool decodeSuccess = true;
    if (!util::decode_nid(&name[12], 1, lid))
        decodeSuccess = false;
    if (!util::decode_nid(&name[14], 1, mid))
        decodeSuccess = false;
    if (!decodeSuccess) {
        LOG_ERROR("decodeNid: can't decode symbol");
        return false;
    }
    return decodeSuccess;
}

bool sce_module::resolveObfSymbol(const char* name, uintptr_t& ptrOut) {
    uint64_t libid = 0, modid = 0;
    if (!decodeNid(name, libid, modid))
        __debugbreak();

    auto lib_it =
        std::find_if(libs.begin(), libs.end(), [&libid](const auto& e) { return e.id == libid; });

    if (lib_it == libs.end())
        return false;

    auto mod_it =
        std::find_if(mods.begin(), mods.end(), [&modid](const auto& e) { return e.id == modid; });

    if (mod_it == mods.end())
        return false;

    uint64_t hid = 0;
    util::decode_nid(name, 11, hid);

    ptrOut = get_static_func((*mod_it).name, hid);
    if (ptrOut)
        return true;

    auto prx = proc->getPrx((*mod_it).name);
    if (!prx) {
        LOG_ERROR("resolveObfSymbol: Unknown module {} ({}) requestd", (*mod_it).name,
                  (*mod_it).id);
        return false;
    }

    char nameenc[12]{}; // name + null terminator
    std::strncpy(nameenc, name, 11);

    std::string longName = std::string(nameenc) + "#" + (*lib_it).name + "#" + (*mod_it).name;
    ptrOut = prx->getSymbol(longName.c_str());
    return true;
}

static void report_unpatched() {
    __debugbreak();
}

bool sce_module::resolveImports() {
    for (uint32_t i = 0; i < jumpTab.second; i++) {
        const auto* r = &jumpTab.first[i];

        int32_t type = ELF64_R_TYPE(r->info);
        int32_t isym = ELF64_R_SYM(r->info);

        ElfSym* sym = &symTab.first[isym];

        if (type != R_X86_64_JUMP_SLOT) {
            LOG_WARNING("resolveImports: bad jump slot {}", i);
            continue;
        }

        if ((uint32_t)isym >= symTab.second || sym->st_name >= strTab.second) {
            LOG_WARNING("resolveImports: bad symbol index {} for relocation {}", isym, i);
            continue;
        }

        int32_t binding = ELF64_ST_BIND(sym->st_info);
        if (binding == STB_LOCAL) {
            *getAddress<uintptr_t*>(r->offset) = getAddress<uintptr_t>(sym->st_value);
            continue;
        }

        uintptr_t addr = 0;
        const char* name = &strTab.first[sym->st_name];

        if (!resolveObfSymbol(name, addr)) {
            return false;
        }

        if (!addr)
            addr = reinterpret_cast<uintptr_t>(&report_unpatched);

        *getAddress<uintptr_t*>(r->offset) = addr;
    }

    return true;
}

bool sce_module::doRelocations() {
    for (size_t i = 0; i < relaTab.second; i++) {
        auto* r = &relaTab.first[i];

        uint32_t isym = ELF64_R_SYM(r->info);
        int32_t type = ELF64_R_TYPE(r->info);

        ElfSym* sym = &symTab.first[isym];
        int32_t bind = ELF64_ST_BIND(sym->st_info);

        if (isym >= symTab.second) {
            LOG_ERROR("Invalid symbol index {}", isym);
            continue;
        }

        uint64_t symVal = 0;

        if (bind == STB_LOCAL)
            symVal = sym->st_value;
        else if (bind == STB_GLOBAL || bind == STB_WEAK) {
            /*relative offset*/ // TODO (force): should we check MID here?
            if (sym->st_value)
                symVal = getAddress<uintptr_t>(sym->st_value);
            else {
                const char* name = &strTab.first[sym->st_name];

                if (!resolveObfSymbol(name, symVal)) {
                    return false;
                }

                if (!symVal)
                    __debugbreak();
            }
        }

        switch (type) {
        case R_X86_64_64:
            *getAddress<uint64_t*>(r->offset) = symVal + r->addend;
            break;
        case R_X86_64_RELATIVE:
            *getAddress<int64_t*>(r->offset) = getAddress<int64_t>(r->addend);
            break;
        case R_X86_64_GLOB_DAT:
            *getAddress<uint64_t*>(r->offset) = symVal;
            break;
        case R_X86_64_PC32:
            *getAddress<uint32_t*>(r->offset) =
                static_cast<uint32_t>(symVal + r->addend - getAddress<uint64_t>(r->offset));
            break;
        case R_X86_64_DTPMOD64:
            *getAddress<uint64_t*>(r->offset) += 0xF00D; // BUG BUG BUG FIX FIX
            break;
        case R_X86_64_DTPOFF32:
            *getAddress<uint32_t*>(r->offset) += static_cast<uint32_t>(symVal + r->addend);
            break;
        case R_X86_64_DTPOFF64:
            *getAddress<uint64_t*>(r->offset) += symVal + r->addend;
            break;
        case R_X86_64_NONE:
            break;
        default:
            continue;
        }
    }

    return true;
}

void sce_module::installExceptionhandler(formats::elfObject& elf) {
    auto* p = elf.get_seg(ElfSegType::PT_GNU_EH_FRAME);

    ehFrameAddr = getAddress<uint8_t*>(p->vaddr);
    ehFrameSize = p->memsz;

    // custom struct for eh_frame_hdr
    struct GnuExceptionInfo {
        uint8_t version;
        uint8_t encoding;
        uint8_t fdeCount;
        uint8_t encodingTable;
        uint8_t first;
    };

    auto* exinfo = elf.at<GnuExceptionInfo>(p->offset);

    if (exinfo->version != 1)
        return;

    uint8_t* data_buffer = nullptr;
    uint8_t* current = &exinfo->first;

    if (exinfo->encoding == 0x03) // relative to base address
    {
        auto offset = *reinterpret_cast<uint32_t*>(current);
        current += 4;

        data_buffer = (uint8_t*)&base[offset];
    } else if (exinfo->encoding == 0x1B) // relative to eh_frame
    {
        auto offset = *reinterpret_cast<int32_t*>(current);
        current += 4;
        data_buffer = &current[offset];
    } else {
        return;
    }

    if (!data_buffer) {
        return;
    }

    uint8_t* data_buffer_end = data_buffer;
    while (true) {
        size_t size = *reinterpret_cast<int32_t*>(data_buffer_end);
        if (size == 0) {
            data_buffer_end = &data_buffer_end[4];
            break;
        }
        if (size == -1) {
            size = 12 + *reinterpret_cast<size_t*>(&data_buffer_end[4]);
        } else {
            size = 4 + size;
        }
        data_buffer_end = &data_buffer_end[size];
    }

    size_t fde_count;
    if (exinfo->fdeCount == 0x03) // absolute
    {
        fde_count = *reinterpret_cast<uint32_t*>(current);
        current += 4;
    } else {
        return;
    }

    if (exinfo->encodingTable != 0x3B) // relative to eh_frame
    {
        return;
    }

    ehFrameheaderAddr = data_buffer;
    ehFrameheaderSize = data_buffer_end - data_buffer;
}

bool sce_module::digestDynamic(formats::elfObject& elf) {
    auto* dynSeg = elf.get_seg(ElfSegType::PT_DYNAMIC);
    auto* dynDat = elf.get_seg(ElfSegType::PT_SCE_DYNLIBDATA);

    if (!dynSeg || !dynDat)
        return false;

    // copy the data segment
    dynData = memory::alloc(dynDat->bin.size(), memory::exec);
    if (!dynData)
        return false;

    std::memcpy(dynData, dynDat->bin.data(), dynDat->bin.size());

    auto* dynamics = elf.at<ELFDyn>(dynSeg->offset);

    // a long boring list of assignments
    for (int32_t i = 0; i < (dynSeg->filesz / sizeof(ELFDyn)); i++) {
        const auto* d = &dynamics[i];
        switch (d->tag) {
        case DT_SCE_FINGERPRINT:
            std::memcpy(fingerprint, dynData + d->un.value, 20);
            break;
        case DT_SCE_MODULE_INFO:
            this->name = reinterpret_cast<const char*>((strTab.first + (d->un.value & 0xFFFFFFFF)));
            break;
        case DT_STRTAB:
        case DT_SCE_STRTAB:
            strTab.first = getDynPtr<char>(d->un.ptr);
            break;
        case DT_STRSZ:
        case DT_SCE_STRSZ:
            strTab.second = d->un.value;
            break;
        case DT_HASH:
        case DT_SCE_HASH:
            hashTab.first = dynData + d->un.ptr;
            break;
        case DT_SCE_HASHSZ:
            hashTab.second = d->un.value;
            break;
        case DT_SYMTAB:
        case DT_SCE_SYMTAB:
            symTab.first = getDynPtr<ElfSym>(d->un.ptr);
            break;
        case DT_SCE_SYMTABSZ:
            symTab.second = d->un.value / sizeof(ElfSym);
            break;
        case DT_JMPREL:
        case DT_SCE_JMPREL:
            jumpTab.first = getDynPtr<ElfRel>(d->un.ptr);
            break;
        case DT_PLTRELSZ:
        case DT_SCE_PLTRELSZ:
            jumpTab.second = d->un.value / sizeof(ElfRel);
            break;
        case DT_SCE_RELA:
            relaTab.first = getDynPtr<ElfRel>(d->un.ptr);
            break;
        case DT_RELASZ:
        case DT_SCE_RELASZ:
            relaTab.second = d->un.value / sizeof(ElfRel);
            break;
        case DT_INIT:
            initAddr = base + d->un.ptr;
            break;
        case DT_FINI:
            finiAddr = base + d->un.ptr;
            break;
        case DT_NEEDED: {
            auto name = reinterpret_cast<const char*>(strTab.first + (d->un.value & 0xFFFFFFFF));
            if (name) {

                // this is how sony do it too
                std::string xname = name;
                auto pos = xname.find_last_of('.');
                if (pos != -1) {
                    if ((pos = xname.find(".prx", pos)) || (pos = xname.find(".sprx", pos))) {
                        sharedObjects.push_back(xname.substr(0, pos));
                    }
                }
            }
            break;
        }
        case DT_SCE_EXPORT_LIB:
        case DT_SCE_IMPORT_LIB: {
            auto& e = libs.emplace_back();
            e.id = d->un.value >> 48;
            e.exported = d->tag == DT_SCE_EXPORT_LIB;
            e.name = reinterpret_cast<const char*>(strTab.first + (d->un.value & 0xFFFFFFFF));
            break;
        }
        case DT_SCE_NEEDED_MODULE: {
            auto& e = mods.emplace_back();
            e.id = d->un.value >> 48;
            e.name = reinterpret_cast<const char*>(strTab.first + (d->un.value & 0xFFFFFFFF));
            break;
        }
        }
    }

    return true;
}

bool sce_module::loadNeededPrx() {
    for (auto& it : sharedObjects) {
        proc->loadPrx(it);
    }

    return true;
}

constexpr uint32_t default_rip_size = 10 * 1024;

static loadError patch_module(elfObject& elf, sce_module& mod) {
    // allocate additional rip memory for relative addressing
    uint8_t* rip = memory::alloc(default_rip_size, memory::exec);
    std::memset(rip, 0xCC, default_rip_size);

    // recompile unsupported instructions
    for (auto& p : elf.programs) {
        if (p.type == PT_LOAD && (p.flags & PF_X)) {
            codeLift lift(rip);
            LOG_ASSERT(lift.init());

            if (!lift.transform(mod.base + p.vaddr, p.memsz))
                return loadError::cant_lift;
        }
    }

#if 0
    // and lastly, apply page protections
    for (auto& s : elf.programs) {
        uint32_t perm = s.flags & (PF_R | PF_W | PF_X);
        auto trans_perm = [](uint32_t op) {
            switch (op) {
            case (PF_R | PF_X):
                return utl::page_execute_read_write;
            case (PF_R | PF_W):
                return utl::page_read_write;
            case (PF_R):
                return utl::page_read;
            default:
                return utl::page_private;
                /*todo: invalid parameter bugcheck*/
            }
        };

        utl::protectMem(mod.base + s.vaddr, s.filesz, trans_perm(perm));
    }
#endif
    return loadError::ok;
}

static void set_module_info(elfObject& elf, sce_module& mod) {
    std::memcpy(&mod.header, &elf.header, sizeof(ELFHeader));

    for (auto& p : elf.programs)
        mod.programs.push_back(p);

    for (auto& p : elf.programs) {
        auto set_info = [&](formats::elf_pg& pg, int idx) {
            auto& s = mod.segments[idx];
            s.addr = reinterpret_cast<uintptr_t>(mod.base + p.vaddr);
            s.flags = p.flags;
            s.size = ::align_up(p.memsz, p.align);
        };

        switch (p.type) {
        case PT_LOAD:
            set_info(p, 0);
            break;
        case PT_SCE_RELRO:
            set_info(p, 1);
            break;
        case PT_TLS: {
            auto t = std::make_unique<tls_info>();
            t->vaddr = p.vaddr;
            t->align = p.align;
            t->filesz = p.filesz;
            t->memsz = p.memsz;

            mod.tlsInfo = std::move(t);
            break;
        }
        default:
            continue;
        }
    }

    mod.installExceptionhandler(elf);
}

static loadError load_exec(elfObject& elf, exec_module& exec) {
    // fingerprint
    sha1_context sha;
    sha1_starts(&sha);

    uint32_t loadSize = 0;
    for (auto& p : elf.programs) {
        if (p.type == PT_LOAD || p.type == PT_SCE_RELRO)
            loadSize += ::align_up(p.memsz, p.align);
    }

    // allocate an aligned chunk
    exec.base = memory::alloc(loadSize, memory::exec);
    LOG_ASSERT(exec.base);

    for (const auto& p : elf.programs) {
        switch (p.type) {
        case PT_LOAD:
        case PT_SCE_RELRO: {
            if (p.memsz) {

                // create a fingerprint of each segment
                sha1_update(&sha, reinterpret_cast<const uint8_t*>(&p.vaddr), sizeof(p.vaddr));
                sha1_update(&sha, reinterpret_cast<const uint8_t*>(&p.memsz), sizeof(p.memsz));
                sha1_update(&sha, p.bin.data(), p.bin.size());

                std::memcpy(exec.base + p.vaddr, p.bin.data(), p.bin.size());
            }
            break;
        }
        case PT_SCE_COMMENT: {
            struct sce_comment {
                uint32_t type;
                uint32_t unk;
                uint32_t data_len;
            };

            const auto& comment = reinterpret_cast<const sce_comment&>(*p.bin.data());
            if (comment.type == 0x48544150 /*"PATH"*/) {

                // path is null terminated
                exec.comment = std::string(elf.at<const char>(p.offset + sizeof(sce_comment)));
            }
            break;
        }
        case PT_SCE_PROCPARAM: {

            auto* info = reinterpret_cast<process_param*>(exec.base + p.vaddr);
            if (info->size < 64)
                LOG_WARNING("Bad process_param size {},{}", info->size, sizeof(proc_param));

            if (info->magic != 0x4942524F /*'ORBI'*/)
                return loadError::proc_param;

            LOG_INFO("** sdk version: {0:x}", info->sdk_version);

            exec.param = info;
            break;
        }
        }
    }

    sha1_finish(&sha, exec.sha1);

    // inspired by rpcs3 :)
    std::string hash("EXC-0000000000000000000000000000000000000000");
    for (u32 i = 0; i < 20; i++) {
        constexpr auto pal = "0123456789abcdef";
        hash[4 + i * 2] = pal[exec.sha1[i] >> 4];
        hash[5 + i * 2] = pal[exec.sha1[i] & 15];
    }

    LOG_INFO("EXEC module hash {}", hash);

    const auto patch_result = patch_module(elf, exec);
    if (patch_result != loadError::ok)
        return patch_result;

    set_module_info(elf, exec);
    return loadError::ok;
}

static loadError load_prx(elfObject& elf, prx_module& prx) {

    // prx fingerprint
    sha1_context sha;
    sha1_starts(&sha);

    // calculate code + data size, so we can allocate 'em in one block
    uint32_t loadSize = 0;
    for (auto& p : elf.programs) {
        if (p.type == PT_LOAD || p.type == PT_SCE_RELRO)
            loadSize += ::align_up(p.memsz, p.align);
    }

    // allocate an aligned chunk
    prx.base = memory::alloc(loadSize, memory::exec);
    LOG_ASSERT(prx.base);

    for (const auto& p : elf.programs) {
        switch (p.type) {
        case PT_LOAD:
        case PT_SCE_RELRO: {
            if (p.memsz) {

                // create a fingerprint of each segment
                sha1_update(&sha, reinterpret_cast<const uint8_t*>(&p.vaddr), sizeof(p.vaddr));
                sha1_update(&sha, reinterpret_cast<const uint8_t*>(&p.memsz), sizeof(p.memsz));
                sha1_update(&sha, p.bin.data(), p.bin.size());

                std::memcpy(prx.base + p.vaddr, p.bin.data(), p.bin.size());
            }
            break;
        }
        case PT_SCE_MODULEPARAM: {
            auto* info = reinterpret_cast<module_param*>(prx.base + p.vaddr);
            if (info->size < 24)
                LOG_WARNING("Bad module_param size {},{}", info->size, sizeof(module_param));

            if (info->magic != 0x3c13f4bf)
                return loadError::module_param;

            LOG_INFO("** sdk version: {0:x}", info->sdk_version);

            prx.param = info;
            break;
        }
        }
    }

    sha1_finish(&sha, prx.sha1);

    // inspired by rpcs3 :)
    std::string hash("PRX-0000000000000000000000000000000000000000");
    for (u32 i = 0; i < 20; i++) {
        constexpr auto pal = "0123456789abcdef";
        hash[4 + i * 2] = pal[prx.sha1[i] >> 4];
        hash[5 + i * 2] = pal[prx.sha1[i] & 15];
    }

    LOG_INFO("PRX module hash {}, loaded at {}", hash, fmt::ptr(prx.base));

    const auto patch_result = patch_module(elf, prx);
    if (patch_result != loadError::ok)
        return patch_result;

    set_module_info(elf, prx);
    return loadError::ok;
}

static bool handle_load_error(loadError err) {
    switch (err) {
    case loadError::map_failure:
        LOG_ERROR("Failed to load obj: unable to map file");
        break;
    case loadError::cant_lift:
        LOG_ERROR("Failed to load: unable to recompile code");
        break;
    case loadError::proc_param:
        LOG_ERROR("Invalid proc_param magic (not ORBI)");
        break;
    case loadError::module_param:
        LOG_ERROR("Invalid module_param magic");
        break;
    default:
        return true;
    }

    return false;
}

static bool handle_elf_err(elfError err) {
    switch (err) {
    case elfError::encrypted:
        LOG_ERROR("Failed to load elf: file was encrypted");
        break;
    case elfError::badformat:
        LOG_ERROR("Failed to load elf: file is not an elf");
        break;
    case elfError::badmachine:
        LOG_ERROR("Failed to load elf: elf wasn't built for amd64");
        break;
    case elfError::stream_pgheaders:
        LOG_ERROR("Failed to load elf: unable to load program headers");
        break;
    case elfError::stream_data:
        LOG_ERROR("Failed to laod elf: unable to load program data");
        break;
    default:
        return true;
    }

    return false;
}

sce_module::sce_module(process& proc) : s_object(&proc, s_object::oType::dynlib) {}

SharedPtr<exec_module> exec_module::load(process& proc, std::string_view path) {
    initialize_static_modules();

    utl::File file(path);
    if (!file.IsOpen()) {
        LOG_ERROR("Failed to open exec_module {}", path);
        return nullptr;
    }

    formats::elfObject elf;
    const auto elf_result = elf.load(file);
    if (!handle_elf_err(elf_result))
        return nullptr;

    auto exec = std::make_shared<exec_module>(proc);

    const auto result = load_exec(elf, *exec);
    if (!handle_load_error(result))
        return nullptr;

    if (!exec->digestDynamic(elf)) {
        LOG_ERROR("Unable to digest exec_module {}", path);
        return nullptr;
    }

    LOG_ASSERT(elf.header.entry != 0);
    exec->entry = exec->base + elf.header.entry;
    return exec;
}

SharedPtr<prx_module> prx_module::load(process& proc, std::string_view path) {
    initialize_static_modules();

    utl::File file(path);
    if (!file.IsOpen()) {
        LOG_ERROR("Failed to open prx_module {}", path);
        return nullptr;
    }

    formats::elfObject elf;
    const auto elf_result = elf.load(file);
    if (!handle_elf_err(elf_result))
        return nullptr;

    auto prx = std::make_shared<prx_module>(proc);

    const auto result = load_prx(elf, *prx);
    if (!handle_load_error(result))
        return nullptr;

    if (!prx->digestDynamic(elf)) {
        LOG_ERROR("Unable to digest prx_module {}", path);
        return nullptr;
    }

    if (elf.header.entry == 0)
        prx->entry = nullptr;
    else
        prx->entry = prx->getAddress<uint8_t*>(elf.header.entry);

    return prx;
}

// sys_dynlib_process_needed_and_relocate
bool init_modules(process& proc, bool phase2) {
    using mod_init_t = int PS4ABI (*)(size_t, const void*, void*);

    for (auto& mod : proc.prx_list()) {
        if (!mod->started)
            continue;

        if (!mod->resolveImports() || !mod->doRelocations()) {
            LOG_ERROR("failed to finalize module {}", mod->name);
            return false;
        }
    }

    if (!phase2)
        return true;

    for (auto& mod : proc.prx_list()) {
        if (!mod->started)
            continue;

        std::string longName = "BaOKcng8g88#" + mod->name + "#" + mod->name;

        uintptr_t start_address = 0;
        if (!mod->resolveObfSymbol(longName.c_str(), start_address)) {
            start_address = reinterpret_cast<uintptr_t>(mod->entry);
        }

        if (start_address) {
            auto module_init = reinterpret_cast<mod_init_t>(start_address);
            module_init(0, nullptr, nullptr); /*argc, argv, retaddr*/
        }
    }

    return true;
}
} // namespace kern