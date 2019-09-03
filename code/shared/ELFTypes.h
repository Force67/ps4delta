#pragma once

// Copyright (C) Force67 2019
// Elf spec with Sony additions

#include <cstdint>

enum ELFDynTag
{
	DT_NULL,
	DT_NEEDED,
	DT_PLTRELSZ,
	DT_PLTGOT,
	DT_HASH,
	DT_STRTAB,
	DT_SYMTAB,
	DT_RELA,
	DT_RELASZ,
	DT_RELAENT,
	DT_STRSZ,
	DT_SYMENT,
	DT_INIT,
	DT_FINI,
	DT_SONAME,
	DT_RPATH,
	DT_SYMBOLIC,
	DT_REL,
	DT_RELSZ,
	DT_RELENT,
	DT_PLTREL,
	DT_DEBUG,
	DT_TEXTREL,
	DT_JMPREL,
	DT_BIND_NOW,
	DT_INIT_ARRAY,
	DT_FINI_ARRAY,
	DT_INIT_ARRAYSZ,
	DT_FINI_ARRAYSZ,
	DT_RUNPATH,
	DT_FLAGS,
	DT_ENCODING,
	DT_PREINIT_ARRAY,
	DT_PREINIT_ARRAYSZ,

	DT_SCE_FINGERPRINT = 0x61000007,
	DT_SCE_ORIGFILENAME = 0x61000009,
	DT_SCE_MODULEINFO = 0x6100000d,
	DT_SCE_NEEDEDMODULE = 0x6100000f,
	DT_SCE_MODULEATTR = 0x61000011,
	DT_SCE_EXPLIB = 0x61000013,
	DT_SCE_IMPLIB = 0x61000015,

	DT_SCE_STRTAB = 0x61000035,
	DT_SCE_STRSIZE = 0x61000037,
};

enum ElfSegType
{
	PT_NULL,
	PT_LOAD,
	PT_DYNAMIC,
	PT_INTERP,
	PT_NOTE,
	PT_SHLIB,
	PT_PHDR,
	PT_TLS,
	PT_LOPROC = 0x70000000,
	PT_HIPROC = 0x7FFFFFFF,
	PT_SCE_DYNLIBDATA = 0x61000000,
	PT_SCE_PROCPARAM = 0x61000001,
	PT_SCE_MODULEPARAM = 0x61000002,
	PT_SCE_RELRO = 0x61000010,
	PT_SCE_COMMENT = 0x6FFFFF00,
	PT_SCE_LIBVERSION = 0x6FFFFF01,
	PT_GNU_EH_FRAME = 0x6474E550
};

enum ElfFlags
{
	PF_X = 1,
	PF_W = 2,
	PF_R = 4,
	PF_MASKPROC = 0xF0000000
};

enum ELFTypeSCE : uint16_t
{
	ET_SCE_EXEC = 0xfe00,  // PS4 Executable
	ET_SCE_DYNEXEC = 0xfe10,  // PS4 Main module
	ET_SCE_RELEXEC = 0xfe04,  // PS4 Relocatable PRX
	ET_SCE_STUBLIB = 0xfe0c,  // PS4 Stub library
	ET_SCE_DYNAMIC = 0xfe18,  // PS4 Dynamic PRX
};

static constexpr uint32_t ELF_MAGIC = 0x464C457F;
static constexpr uint16_t ELF_MACHINE_X86_64 = 0x3E;

struct ELFHeader
{
	uint32_t magic;
	uint8_t ident[12]; // < actually 16
	ELFTypeSCE type; //< sony custom
	uint16_t machine;
	uint32_t version;
	uint64_t entry;
	uint64_t phoff; //< program header offset
	uint64_t shoff; //< does sometimes not exist
	uint32_t flags;
	uint16_t ehsize;
	uint16_t phentsize; //< size in bytes of entries in pg table
	uint16_t phnum;
	uint16_t shentsize;
	uint16_t shnum;
	uint16_t shstrndx;
};

struct ELFPgHeader
{
	uint32_t type;  //< data info
	uint32_t flags; //< memory protection flags
	uint64_t offset;
	uint64_t vaddr;
	uint64_t paddr;
	uint64_t filesz;
	uint64_t memsz;
	uint64_t align;
};

struct ELFDyn
{
	int64_t tag;
	uint64_t value;
};

static_assert(sizeof(ELFPgHeader) == 56, "Elf program header size mismatch");
static_assert(sizeof(ELFHeader) == 64, "Elf header size mismatch");

inline const char* ElfTypeToString(uint32_t type)
{
	switch (type) {
	case ET_SCE_EXEC: return "Executable";
	case ET_SCE_DYNEXEC: return "Main module";
	case ET_SCE_RELEXEC: return "Relocatable PRX";
	case ET_SCE_STUBLIB: return "Stub library";
	case ET_SCE_DYNAMIC: return "Dynamic PRX";
	default: return "Unknown";
	}
}

inline const char* SceTypeToString(uint32_t type)
{
#define AS_STR(idx)      \
    if (type == idx)     \
        return #idx;

	AS_STR(PT_LOAD)
		AS_STR(PT_DYNAMIC)
		AS_STR(PT_INTERP)
		AS_STR(PT_NOTE)
		AS_STR(PT_SHLIB)
		AS_STR(PT_PHDR)
		AS_STR(PT_TLS)
		AS_STR(PT_LOPROC)
		AS_STR(PT_HIPROC)
		AS_STR(PT_SCE_DYNLIBDATA)
		AS_STR(PT_SCE_PROCPARAM)
		AS_STR(PT_SCE_MODULEPARAM)
		AS_STR(PT_SCE_RELRO)
		AS_STR(PT_SCE_COMMENT)
		AS_STR(PT_SCE_LIBVERSION)
		AS_STR(PT_GNU_EH_FRAME)

#undef AS_STR

		return "Unknown";
}