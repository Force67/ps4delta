/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>

namespace kern {
struct eh_frame_header {
    u8 version;
    u8 encoding;
    u8 fdeCount;
    u8 encodingTable;
    u8 first;
};

bool eh_install(u8* base, eh_frame_header* frame);
size_t os_get_eh_size();
}