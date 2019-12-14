#pragma once

// Copyright (C) Force67 2019

#include <utl/File.h>

namespace utl {
	class File;
}

namespace vfs
{
	struct pup_header
	{
		uint32_t magic;
		uint32_t unk;
		uint8_t contentType;
		uint8_t productType;
		uint16_t pad;
		uint16_t headerSize;
		uint16_t sigSize;
		uint32_t sizeSELF;
		uint32_t pad2;
		uint16_t numSegments;
		uint16_t unk2;
		uint32_t pad3;
	};

	struct pup_entry
	{
		uint32_t flags;
		uint32_t unk; //< always zero
		uint64_t offset;
		uint64_t sizeCompressed;
		uint64_t sizeUncompressed;
	};

	static_assert(sizeof(pup_header) == 32);
	static_assert(sizeof(pup_entry) == 32);

	class pupReader
	{
	public:
		pupReader(const std::string&);

		bool load();
		void extractAll();
		utl::File extractFile(uint16_t id);

	private:
		utl::File file;
		pup_header header{};
		std::vector<pup_entry> entries;
	};
}