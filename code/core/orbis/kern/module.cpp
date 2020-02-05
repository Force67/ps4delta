
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#if 0
#include <algorithm>
#include <utl/file.h>
#include <utl/mem.h>

#include "arch/native/native_lift.h"
#include "runtime/vprx/vprx.h"

#include "module.h"
#include "proc.h"

namespace krnl {

bool smodule::fromMem(std::unique_ptr<uint8_t[]> data) {
    digestDynamic();

#ifdef _DEBUG
    LOG_TRACE("mapped {} at {}", info.name, fmt::ptr(info.base));
#endif
    setupTLS();

    installEHFrame();

    if (elf->entry == 0)
        info.entry = nullptr;
    else
        info.entry = getAddress<uint8_t>(elf->entry);

    for (auto& it : sharedObjects) {
        process->loadModule(it);
    }

    return true;
}

void smodule::digestDynamic() {
    const auto* dynS = getSegment(ElfSegType::PT_DYNAMIC);
    const auto* dyldS = getSegment(ElfSegType::PT_SCE_DYNLIBDATA);

    uint8_t* dynldPtr = getOffset<uint8_t>(dyldS->offset);
    uint8_t* dynldAddr = getAddress<uint8_t>(dyldS->vaddr);
    // std::printf("addr = %p\n", dynldAddr);
    ELFDyn* dynamics = getOffset<ELFDyn>(dynS->offset);
    for (int32_t i = 0; i < (dynS->filesz / sizeof(ELFDyn)); i++) {
        auto* d = &dynamics[i];

        switch (d->tag) {
        case DT_SCE_JMPREL:
            jmpslots = (ElfRel*)(dynldPtr + d->un.ptr);
            break;
        case DT_PLTRELSZ:
        case DT_SCE_PLTRELSZ:
            numJmpSlots = static_cast<uint32_t>(d->un.value / sizeof(ElfRel));
            break;
        case DT_STRSZ:
        case DT_SCE_STRSIZE:
            strtab.size = d->un.value;
            break;
        case DT_SCE_SYMTAB:
            symbols = reinterpret_cast<ElfSym*>(dynldPtr + d->un.ptr);
            break;
        case DT_SCE_SYMTABSZ:
            numSymbols = static_cast<uint32_t>(d->un.value / sizeof(ElfSym));
            break;
        case DT_SCE_RELA:
            rela = reinterpret_cast<ElfRel*>(dynldPtr + d->un.ptr);
            break;
        case DT_NEEDED: {
            auto name = (const char*)(strtab.ptr + (d->un.value & 0xFFFFFFFF));
            if (name) {
                std::string xname = name;
                /*quick but (valid?) hack for determining if an object is exported*/
                auto pos = xname.find(".prx");
                if (pos != -1) {
                    sharedObjects.push_back(xname.substr(0, pos));
                }
            }

            break;
        }
        case DT_RELASZ:
        case DT_SCE_RELASZ:
            numRela = static_cast<uint32_t>(d->un.value / sizeof(ElfRel));
            break;
        case DT_SCE_EXPLIB:
        case DT_SCE_IMPLIB: {
            /*for now, in the future we also want to store explibs*/
            auto& e = impLibs.emplace_back();
            e.id = d->un.value >> 48;
            e.exported = d->tag == DT_SCE_EXPLIB;
            e.name = (const char*)(strtab.ptr + (d->un.value & 0xFFFFFFFF));
            break;
        }
        case DT_SCE_EXPORT_LIB_ATTR:
        case DT_SCE_IMPORT_LIB_ATTR: {
            uint16_t id = d->un.value >> 48;
            uint16_t idx = d->un.value & 0xFFF;

            for (auto& mod : impLibs) {
                if (mod.id == id) {
                    mod.attr = idx;
                    break;
                }
            }
            break;
        }
        case DT_SCE_NEEDED_MODULE: {
            auto& e = impModules.emplace_back();
            e.id = d->un.value >> 48;
            e.name = (const char*)(strtab.ptr + (d->un.value & 0xFFFFFFFF));
            break;
        }
        case DT_SCE_MODULE_ATTR: {
            uint16_t id = d->un.value >> 48;
            uint16_t idx = d->un.value & 0xFFF;

            for (auto& mod : impModules) {
                if (mod.id == id) {
                    mod.attr = idx;
                    break;
                }
            }
            break;
        }
        }
    }
}



bool smodule::resolveObfSymbol(const char* name, uintptr_t& ptrOut) {
    uint64_t libid = 0, modid = 0;
    if (!decodeNid(name, libid, modid))
        __debugbreak();

    const char* libname = nullptr;

    // TODO: could be done nicer
    for (auto& mod : impLibs) {
        if (mod.id == static_cast<int32_t>(libid)) {
            libname = mod.name;
            break;
        }
    }

    if (!libname)
        return false;

    for (auto& mod : impModules) {
        if (mod.id == static_cast<int32_t>(modid)) {
            if (process->getEnv().enableHLE) {
                uint64_t hid = 0;
                runtime::decode_nid(name, 11, hid);

                ptrOut = runtime::vprx_get(mod.name, hid);
                if (ptrOut) {
                    return true;
                }
            }

            auto xmod = process->getModule(mod.name);
            if (!xmod) {
                LOG_ERROR("resolveObfSymbol: Unknown module {} ({}) requestd", mod.name, mod.id);
                return false;
            }

            char nameenc[12]{}; // name + null terminator
            std::strncpy(nameenc, name, 11);

            std::string longName = std::string(nameenc) + "#" + libname + "#" + mod.name;
            ptrOut = xmod->getSymbolFullName(longName.c_str());
            return true;
        }
    }

    return false;
}


uintptr_t smodule::getSymbol(uint64_t nid) {
    // are there any overrides for me?
    auto imp = runtime::vprx_get(info.name.c_str(), nid);
    if (imp != 0)
        return imp;

    for (uint32_t i = 0; i < numSymbols; i++) {
        const auto* s = &symbols[i];

        if (!s->st_value)
            continue;

        // if the symbol is exported
        // int32_t binding = ELF64_ST_BIND(s->st_info);

        const char* name = &strtab.ptr[s->st_name];

        uint64_t hid = 0;
        if (!runtime::decode_nid(name, 11, hid)) {
            LOG_ERROR("resolveExport: cant handle NID");
            return 0;
        }

        if (nid == hid) {
            return getAddressNPTR<uintptr_t>(s->st_value);
        }
    }

    return 0;
}

uintptr_t smodule::getSymbol2(const char* name) {
    for (uint32_t i = 0; i < numSymbols; i++) {
        const auto* s = &symbols[i];

        if (!s->st_value)
            continue;

        const char* sname = &strtab.ptr[s->st_name];

        if (std::strcmp(sname, name) == 0) {
            return getAddressNPTR<uintptr_t>(s->st_value);
        }
    }

    return 0;
}

// taken from idc's "uplift" project
void smodule::installEHFrame() {
    const auto* p = getSegment(PT_GNU_EH_FRAME);
    if (p->filesz > p->memsz)
        return;

    info.ehFrameAddr = getAddress<uint8_t>(p->vaddr);
    info.ehFrameSize = p->memsz;

    // custom struct for eh_frame_hdr
    struct GnuExceptionInfo {
        uint8_t version;
        uint8_t encoding;
        uint8_t fdeCount;
        uint8_t encodingTable;
        uint8_t first;
    };

    auto* exinfo = getOffset<GnuExceptionInfo>(p->offset);

    if (exinfo->version != 1)
        return;

    uint8_t* data_buffer = nullptr;
    uint8_t* current = &exinfo->first;

    if (exinfo->encoding == 0x03) // relative to base address
    {
        auto offset = *reinterpret_cast<uint32_t*>(current);
        current += 4;

        data_buffer = (uint8_t*)&info.base[offset];
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

    info.ehFrameheaderAddr = data_buffer;
    info.ehFrameheaderSize = data_buffer_end - data_buffer;
}

void smodule::logDbgInfo() {
    for (uint16_t i = 0; i < elf->phnum; i++) {
        auto s = &segments[i];

#if 0
			case PT_SCE_LIBVERSION:
			{
				uint8_t* sec = getOffset<uint8_t>(s->offset);

				// count entries
				int32_t index = 0;
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

							uint32_t version = *(uint32_t*)& sec[i + 1];
							uint8_t* vptr = (uint8_t*)& version;

							std::printf("lib <%s>, version %x.%x.%x.%x\n", name.c_str(), vptr[0], vptr[1], vptr[2], vptr[3]);
							break;
						}
					}

					// skip forward
					index += cb;
				}
				break;
			}
#endif
        }
    }
}
} // namespace krnl
#endif