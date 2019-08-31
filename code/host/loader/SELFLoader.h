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

	struct SELFHeader
	{
		uint32_t magic;
		uint32_t unk;
		uint8_t contentType;
		SELFProductType productType;
		uint16_t pad;
		uint16_t headerSize;
		uint16_t signatureSize;
		uint32_t sizeSELF;
		uint32_t pad1;
		uint16_t numSegments;
		uint16_t unk1; //< always 0x22
		uint32_t pad2;
	};

	static constexpr uint32_t SELF_MAGIC = 0x4F153D1D;

	struct SELFSegmentTable
	{
		uint64_t flags;
		uint64_t offset;
		uint64_t encCompressedSize;
		uint64_t decCompressedSize;

		uint32_t Id() { return flags >> 20; }

		bool IsOrdered() { return (flags & 1) != 0; }
		bool IsEncrypted() { return (flags & 2) != 0; }
		bool IsSigned() { return (flags & 4) != 0; }
		bool IsCompressed() { return (flags & 8) != 0; }
		bool IsBlocked() { return (flags & 0x800) != 0; }
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

	struct ELFHeader
	{
		uint8_t ident[16];
		uint16_t type;
		uint16_t machine;
		uint32_t version;
		uint64_t entry;
		uint64_t phoff;
		uint64_t shoff;
		uint32_t flags;
		uint16_t ehsize;
		uint16_t phentsize;
		uint16_t phnum;
		uint16_t shentsize;
		uint16_t shnum;
		uint16_t shstrndx;
	};

	struct ELFPgHeader
	{
		uint32_t type;
		uint32_t flags;
		uint64_t offset;
		uint64_t vaddr;
		uint64_t paddr;
		uint64_t filesz;
		uint64_t memsz;
		uint32_t flags1;
		uint16_t align;
	};

	static_assert(sizeof(SELFHeader) == 32, "header size mismatch");
	static_assert(sizeof(ELFPgHeader) == 0x38, "Elf program header size mismatch");
	static_assert(sizeof(SELFSegmentTable) == 32, "segment table size mismatch");

	class SELF_Loader final : public AppLoader
	{
	public:

		SELF_Loader() :
			AppLoader()
		{}

		static FileType IdentifyType(const uint8_t*);

		LoadErrorCode Load() override;
		LoadErrorCode Unload() override;
	};
}