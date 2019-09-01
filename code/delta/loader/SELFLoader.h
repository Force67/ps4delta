#pragma once

// Copyright (C) Force67 2019

#include "Loader.h"

namespace loaders
{
	// increments by 0x10 for each new
	// revision
	enum class SELFProductType : uint8_t
	{
		PUP,
		K = 0xC,
		SL = 0xF,
		SM = 0xE,
		SELF = 0x8, //< applies to EBOOT, ELF and SELF
		SPRX = 0x9  //< applies to SPRX, SDLL and SEXE
	};

	enum class SELFContentType : uint8_t
	{
		SELF = 1,
		PUP = 4
	};

	enum SegFlags
	{
		SF_ORDR = 0x1,    // ordered?
		SF_ENCR = 0x2,    // encrypted
		SF_SIGN = 0x4,    // signed
		SF_DFLG = 0x8,    // deflated
		SF_BFLG = 0x800,  // block segment
	};

	struct SELFHeader
	{
		uint32_t magic;
		uint32_t unk;
		SELFContentType contentType;
		SELFProductType productType;
		uint16_t pad;
		uint16_t headerSize;
		uint16_t signatureSize;
		uint32_t sizeSELF; // < unrounded img size
		uint32_t pad1;
		uint16_t numSegments;
		uint16_t unk1; //< always 0x22
		uint32_t pad2;
	};

	static constexpr uint32_t SELF_MAGIC = 0x1D3D154F;

	struct SELFSegmentTable
	{
		uint64_t flags;
		uint64_t offset;
		uint64_t encCompressedSize;
		uint64_t decCompressedSize;

		uint32_t Id() {
			return static_cast<uint32_t>((uint64_t)flags >> 20); }
	};

	struct SCEContentId
	{
		char pad[0x20];
	};

	struct SCESpecial
	{
		uint64_t authId;
		uint64_t productType;
		uint64_t version1;
		uint64_t version2;
		SCEContentId contentId;
		char shaSum[0x20];
	};

	enum ELFTypeSCE : uint16_t
	{
		ET_SCE_EXEC = 0xfe00,  // PS4 Executable
		ET_SCE_DYNEXEC = 0xfe10,  // PS4 Main module
		ET_SCE_RELEXEC = 0xfe04,  // PS4 Relocatable PRX
		ET_SCE_STUBLIB = 0xfe0c,  // PS4 Stub library
		ET_SCE_DYNAMIC = 0xfe18,  // PS4 Dynamic PRX
	};

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

	struct ELFHeader
	{
		uint8_t ident[16];
		ELFTypeSCE type;
		uint16_t machine;
		uint32_t version;
		uint64_t entry;
		uint64_t phoff;
		uint64_t shoff; //< null
		uint32_t flags;
		uint16_t ehsize;
		uint16_t phentsize;
		uint16_t phnum;
		uint16_t shentsize;
		uint16_t shnum;
		uint16_t shstrndx;
	};

	static constexpr uint64_t ELF_MACHINE_X86_64 = 0x3E;

	struct ELFPgHeader
	{
		uint32_t type;
		uint32_t flags;
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

	static_assert(sizeof(SELFHeader) == 32, "header size mismatch");
	static_assert(sizeof(ELFPgHeader) == 56, "Elf program header size mismatch");
	static_assert(sizeof(SELFSegmentTable) == 32, "segment table size mismatch");

	class SELF_Loader final : public AppLoader
	{
		ELFHeader* elf;
		ELFPgHeader* segments;

		// based on our elf
		template<typename Type>
		Type* GetElfOfs(uint32_t dist) {
			return (Type*)((uint8_t*)elf + dist);
		}

		template<typename Type>
		Type* GetOffset(uint32_t dist) {
			return (Type*)(data + dist);
		}

		void SetupDynamics(const ELFPgHeader*);
		void SetupTLS(const ELFPgHeader*);
		bool MapSegments(krnl::VMAccessMgr&);

		// SELF_DEBUG
		const char* TypeToString();
		static const char* SecTypeToStr(uint32_t);

	public:

		SELF_Loader(uint8_t*);

		static FileType IdentifyType(const uint8_t*);

		LoadErrorCode Load(krnl::VMAccessMgr&) override;
		LoadErrorCode Unload() override;
	};
}