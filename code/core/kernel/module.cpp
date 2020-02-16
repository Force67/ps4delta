
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

#include "loader/elf.h"
#include "loader/elf_util.h"

#include "core.h"
#include "module.h"

#include <config.h>

namespace kern {
extern config::opt<bool> hleKernel;

static const static_module* staticModules[] = {
    &prx_static_modules::orbis_libkernel
};

using namespace formats;

static uintptr_t get_static_func(const char* lib, u64 hid) {
    const static_module* module = nullptr;

    // find the right table
    for (const auto& t : staticModules) {
        if (std::strcmp(lib, t->namePtr) == 0) {
            module = t;
            break;
        }
    }

    if (module) {
        __debugbreak();
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
        auto p = (const u8*)name;
        u32 h = 0;
        u32 g;
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

    auto* htab = reinterpret_cast<u32*>(hashTab.first);
    u32 nbucket = htab[0];
    u32 nchain = htab[1];
    u32* bucket = &htab[2];
    u32* chain = &bucket[nbucket];

    for (u32 i = bucket[hash % nbucket]; i; i = chain[i]) {
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

static bool decodeNid(const char* name, u64& lid, u64& mid) {
    /*nid's are always 16 bytes long so we should get away
     with hard coding the access offset*/

    bool decodeSuccess = true;
    if (!decode_nid(&name[12], 1, lid))
        decodeSuccess = false;
    if (!decode_nid(&name[14], 1, mid))
        decodeSuccess = false;
    if (!decodeSuccess) {
        LOG_ERROR("decodeNid: can't decode symbol");
        return false;
    }
    return decodeSuccess;
}

bool sce_module::resolveObfSymbol(const char* name, uintptr_t& ptrOut) {
    u64 libid = 0, modid = 0;
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

    u64 hid = 0;
    decode_nid(name, 11, hid);

    if (hleKernel) {
        ptrOut = get_static_func((*mod_it).name, hid);
        if (ptrOut)
            return true;
    }

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
    for (u32 i = 0; i < jumpTab.second; i++) {
        const auto* r = &jumpTab.first[i];

        i32 type = ELF64_R_TYPE(r->info);
        i32 isym = ELF64_R_SYM(r->info);

        ElfSym* sym = &symTab.first[isym];

        if (type != R_X86_64_JUMP_SLOT) {
            LOG_WARNING("resolveImports: bad jump slot {}", i);
            continue;
        }

        if ((u32)isym >= symTab.second || sym->st_name >= strTab.second) {
            LOG_WARNING("resolveImports: bad symbol index {} for relocation {}", isym, i);
            continue;
        }

        i32 binding = ELF64_ST_BIND(sym->st_info);
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

        u32 isym = ELF64_R_SYM(r->info);
        i32 type = ELF64_R_TYPE(r->info);

        ElfSym* sym = &symTab.first[isym];
        i32 bind = ELF64_ST_BIND(sym->st_info);

        if (isym >= symTab.second) {
            LOG_ERROR("Invalid symbol index {}", isym);
            continue;
        }

        u64 symVal = 0;

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
            *getAddress<u64*>(r->offset) = symVal + r->addend;
            break;
        case R_X86_64_RELATIVE:
            *getAddress<int64_t*>(r->offset) = getAddress<int64_t>(r->addend);
            break;
        case R_X86_64_GLOB_DAT:
            *getAddress<u64*>(r->offset) = symVal;
            break;
        case R_X86_64_PC32:
            *getAddress<u32*>(r->offset) =
                static_cast<u32>(symVal + r->addend - getAddress<u64>(r->offset));
            break;
        case R_X86_64_DTPMOD64:
            *getAddress<u64*>(r->offset) += 0xF00D; // BUG BUG BUG FIX FIX
            break;
        case R_X86_64_DTPOFF32:
            *getAddress<u32*>(r->offset) += static_cast<u32>(symVal + r->addend);
            break;
        case R_X86_64_DTPOFF64:
            *getAddress<u64*>(r->offset) += symVal + r->addend;
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

    ehFrameAddr = getAddress<u8*>(p->vaddr);
    ehFrameSize = p->memsz;

    // custom struct for eh_frame_hdr
    struct GnuExceptionInfo {
        u8 version;
        u8 encoding;
        u8 fdeCount;
        u8 encodingTable;
        u8 first;
    };

    auto* exinfo = elf.at<GnuExceptionInfo>(p->offset);

    if (exinfo->version != 1)
        return;

    u8* data_buffer = nullptr;
    u8* current = &exinfo->first;

    if (exinfo->encoding == 0x03) // relative to base address
    {
        auto offset = *reinterpret_cast<u32*>(current);
        current += 4;

        data_buffer = (u8*)&base[offset];
    } else if (exinfo->encoding == 0x1B) // relative to eh_frame
    {
        auto offset = *reinterpret_cast<i32*>(current);
        current += 4;
        data_buffer = &current[offset];
    } else {
        return;
    }

    if (!data_buffer) {
        return;
    }

    u8* data_buffer_end = data_buffer;
    while (true) {
        size_t size = *reinterpret_cast<i32*>(data_buffer_end);
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
        fde_count = *reinterpret_cast<u32*>(current);
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
    for (i32 i = 0; i < (dynSeg->filesz / sizeof(ELFDyn)); i++) {
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

#if 0
			case PT_SCE_LIBVERSION:
			{
				u8* sec = getOffset<u8>(s->offset);

				// count entries
				i32 index = 0;
				while (index <= s->filesz) {

					int8_t cb = sec[index];

					// skip control byte
					index++;

					for (int i = index; i < (index + cb); i++)
					{
						if (sec[i] == 0x3A) {

							size_t length = i - index;

							std::string name;
							name.resize(length);
							memcpy(name.data(), &sec[index], length);

							u32 version = *(u32*)& sec[i + 1];
							u8* vptr = (u8*)& version;

							std::printf("lib <%s>, version %x.%x.%x.%x\n", name.c_str(), vptr[0], vptr[1], vptr[2], vptr[3]);
							break;
						}
					}

					// skip forward
					index += cb;
				}
				break;
#endif

sce_module::sce_module(process& proc) : s_object(&proc, s_object::oType::dynlib) {}

SharedPtr<exec_module> exec_module::load(process& proc, std::string_view path) {
    // direct path
    auto exec = std::make_shared<exec_module>(proc);
    return loadElf(*exec, path) ? exec : nullptr;
}

SharedPtr<prx_module> prx_module::load(process& proc, std::string_view path) {
    auto& fs = core::System::get().file_system();

    // this is temporary uglyness
    // TODO: choose path
    // SHOULD BE MOVED INTO LDR
    auto pathX = fs.get("/system/common/lib/" + std::string(path) + ".sprx");

    auto prx = std::make_shared<prx_module>(proc);
    return loadElf(*prx, pathX) ? prx : nullptr;
}

// sys_dynlib_process_needed_and_relocate
bool init_modules(process& proc, bool phase2) {
    using mod_init_t = int PS4ABI (*)(size_t, const void*, void*);
    auto& main_mod = proc.main_exec();
    main_mod.loadNeededPrx();

    for (auto& mod : proc.prx_list()) {
        if (!mod->started)
            continue;

        if (!mod->resolveImports() || !mod->doRelocations()) {
            LOG_ERROR("failed to finalize module {}", mod->name);
            return false;
        }
    }

    if (!main_mod.resolveImports() || !main_mod.doRelocations()) {
        LOG_ERROR("failed to finalize main module!!! {}", main_mod.name);
        return false;
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