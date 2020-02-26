#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>

namespace kern {
struct proc_param {
    u64 length;
    u32 magic;
    u32 unk;
    u32 kvers;
};

struct segment_info {
    uintptr_t addr;
    u32 size;
    u32 flags;
};

struct dynlib_info_ex {
    size_t size;
    char name[256];
    i32 handle;
    u16 tls_index;
    u16 pad0;
    u8* tls_init_addr;
    u32 tls_init_size;
    u32 tls_size;
    u32 tls_offset;
    u32 tls_align;
    u8* init_proc_addr;
    u8* fini_proc_addr;
    u64 reserved1;
    u64 reserved2;
    u8* eh_frame_hdr_addr;
    u8* eh_frame_addr;
    u32 eh_frame_hdr_size;
    u32 eh_frame_size;
    segment_info segs[4];
    u32 seg_count;
    u32 ref_count;
};

struct dynlib_info {
    size_t size;
    char name[256];
    segment_info segs[4];
    u32 seg_count;
    uint8_t fingerprint[20];
};

static_assert(sizeof(dynlib_info_ex) == 424);
static_assert(sizeof(dynlib_info) == 352);

int PS4ABI sys_dynlib_dlopen(const char*);
int PS4ABI sys_dynlib_get_info(u32 handle, dynlib_info*);
int PS4ABI sys_dynlib_get_info_ex(u32 handle, i32 ukn /*always 1*/,
                                  dynlib_info_ex* dyn_info);
int PS4ABI sys_dynlib_get_proc_param(void** data, size_t* size);
int PS4ABI sys_dynlib_get_list(u32* handles, size_t maxCount, size_t* count);
int PS4ABI sys_dynlib_dlsym(u32 handle, const char* cname, void** sym);
int PS4ABI sys_dynlib_get_obj_member(u32 handle, uint8_t index, void** value);
int PS4ABI sys_dynlib_process_needed_and_relocate();
}