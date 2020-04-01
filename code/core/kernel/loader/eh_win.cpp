
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

// translate DWARF exception information into RTL for windows

#include "eh_win.h"

namespace kern {

enum {
DW_EH_PE_absptr, /* absolute value */
DW_EH_PE_udata2 = 2,
DW_EH_PE_udata4 = 3,
DW_EH_PE_udata8 = 4,
DW_EH_PE_sleb128 = 9,
DW_EH_PE_sdata2 = 0xA,
DW_EH_PE_sdata4 = 0xB,
DW_EH_PE_sdata8 = 0xC,
DW_EH_PE_pcrel = 0x10,
DW_EH_PE_textrel = 0x20,
DW_EH_PE_datarel = 0x30,
DW_EH_PE_funcrel = 0x40, /* start-of-procedure-relative */
DW_EH_PE_aligned = 0x50, /* aligned pointer */
DW_EH_PE_indirect = 0x80,
DW_EH_PE_omit = 0xFF,
};

enum { 
DW_EH_PE_APPL_MASK = 0x70, 
DW_EH_PE_FORMAT_MASK = 0x0F
};

static u8* dw_decode(u8 encoding, u8* base, const u8* ehBase, const u8*& buf) {
    u8* val = nullptr;

    switch (encoding) {
    case DW_EH_PE_omit:
        val = 0;
        break;
    case 0: // ptr
        __debugbreak();
    default:
        break;
    }

    switch (encoding & DW_EH_PE_APPL_MASK) {
    case DW_EH_PE_absptr: // no modification
        break;
    case DW_EH_PE_pcrel: // relative to PC
        val += reinterpret_cast<u64>(base);
        break;
    case DW_EH_PE_datarel: // relative to beginning of EH section
        val += reinterpret_cast<u64>(ehBase);
        break;
    default:
        __debugbreak();
        break;
    }

    switch (encoding & DW_EH_PE_FORMAT_MASK) {
    case DW_EH_PE_udata2:
        val += *reinterpret_cast<u16*>(val + 2);
        break;
    case DW_EH_PE_sdata4:
        val += *reinterpret_cast<i32*>(val + 4);
        break;
    case DW_EH_PE_udata4:
        val += *reinterpret_cast<u32*>(val + 4);
        break;
    default:
        __debugbreak();
    }

    if (encoding & DW_EH_PE_indirect)
        val = reinterpret_cast<u8*>(*(uintptr_t*)val);

    return val;
}

bool eh_install(u8* base, eh_frame_header* frame) {
    if (frame->version != 1) return false;

    const u8 *ptr = &frame->first;



    return true;
}

size_t os_get_eh_size() {

}
}