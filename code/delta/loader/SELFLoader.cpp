
// Copyright (C) Force67 2019

#include <cstdio>
#include <loader/SELFLoader.h>

#define SHT_NULL 0
#define SHT_PROGBITS 1
#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4
#define SHT_HASH 5
#define SHT_DYNAMIC 6
#define SHT_NOTE 7
#define SHT_NOBITS 8
#define SHT_REL 9
#define SHT_SHLIB 10
#define SHT_DYNSYM 11
#define SHT_LOPROC 0x70000000
#define SHT_HIPROC 0x7FFFFFFF
#define SHT_LOUSER 0x80000000
#define SHT_HIUSER 0xFFFFFFFF

// Segment Types
#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_SHLIB 5
#define PT_PHDR 6
#define PT_LOPROC 0x70000000
#define PT_HIPROC 0x7FFFFFFF

// Segment Memory Protection Flags
#define PF_X 0x1
#define PF_W 0x2
#define PF_R 0x4
#define PF_MASKPROC 0xF0000000

enum ElfType {
	ET_NONE = 0,
	ET_REL = 1,
	ET_EXEC = 2,
	ET_DYN = 3,
	ET_CORE = 4,
	ET_LOPROC = 0xFF00,
	ET_HIPROC = 0xFFFF,
};

namespace loaders
{
	SELF_Loader::SELF_Loader(uint8_t *d) :
		AppLoader(d)
	{}

	FileType SELF_Loader::IdentifyType(const uint8_t* data)
	{
		SELFHeader* hdr = (SELFHeader*)data;
		if (hdr->magic == SELF_MAGIC && hdr->contentType == SELFContentType::SELF)
			return FileType::SELF;

		return FileType::UNKNOWN;
	}

	LoadErrorCode SELF_Loader::Load(krnl::VMAccessMgr& vma)
	{
		auto* hdr = GetOffset<SELFHeader>(0);

		auto* tables = GetOffset<SELFSegmentTable>(sizeof(SELFHeader));
		for (int i = 0; i < hdr->numSegments; i++) {

			auto* p = &tables[i];

			auto flag_set = [&](SegFlags flag) -> bool {
				return p->flags & flag != 0;
			};

			printf("== Segment%d ==\n"
				"Ordered %d\n"
				"Encrypted %d\n"
				"Deflated %d\n"
				"Block Segment %d\n"
				"Signed %d\n"
				"Size compressed %llu\n"
				"Size decompressed %llu\n"
				"Offset %llu, (%llx)\n"
				"Segment Id %d\n",
				i,
				flag_set(SF_ORDR), flag_set(SF_ENCR), flag_set(SF_DFLG),
				flag_set(SF_BFLG), flag_set(SF_SIGN),
				p->encCompressedSize,
				p->decCompressedSize,
				p->offset, p->offset,
				p->Id());
		}

		// TODO: find a better way :D
		elf = GetOffset<ELFHeader>(sizeof(SELFHeader) + (sizeof(SELFSegmentTable) * hdr->numSegments));

		if (*GetElfOfs<uint32_t>(0) != 0x464C457F)
			return LoadErrorCode::BADMAGIC;

		std::printf("ELF TYPE %s\n", TypeToString());

		segments = GetOffset<ELFPgHeader>(elf->phoff); // phoff should be 0x40

		if (elf->shoff == 0)
			std::puts("section header table missing!");

		SetLoaded();
		if (MapSegments(vma))
			return LoadErrorCode::SUCCESS;

		return LoadErrorCode::UNKNOWN;
	}

	bool SELF_Loader::MapSegments(krnl::VMAccessMgr& vma)
	{
		std::printf(__FUNCTION__ " %d segments, %d sections\n", elf->phnum, elf->shnum);

		// count total segment size
		uint64_t total_size = 0;
		for (uint16_t i = 0; i < elf->phnum; ++i) {
			const auto* p = &segments[i];
			if (p->type == PT_LOAD) {
				total_size += (p->memsz + 0xFFF) & ~0xFFF;
			}
		}

		for (uint16_t i = 0; i < elf->phnum; ++i) {
			const auto* p = &segments[i];
			//if (p->type == PT_LOAD) {
				std::printf("ELF SEG:%d, %lld (%llx), ELF TYPE %x\n", i, p->offset, p->offset, p->type);
			//}
		}

		return true;
	}

	const char* SELF_Loader::TypeToString()
	{
#define AS_STR(idx)                             \
    if (elf->type == idx)                   \
        return #idx;

		AS_STR(ET_SCE_EXEC)
		AS_STR(ET_SCE_DYNEXEC)
		AS_STR(ET_SCE_RELEXEC)
		AS_STR(ET_SCE_STUBLIB)
		AS_STR(ET_SCE_DYNAMIC)

#undef AS_STR

		return "Unknown";
	}

	LoadErrorCode SELF_Loader::Unload()
	{
		return LoadErrorCode::UNKNOWN;
	}
}