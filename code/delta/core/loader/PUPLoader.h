#pragma once

// Copyright (C) Force67 2019

#include <cstdint>
#include <vector>

#include <utl/File.h>

namespace loaders
{
	struct PUPHeader
	{
		uint32_t magic;
		uint32_t version;
		uint32_t flags;
		uint32_t fileCount;
		uint32_t blockCount;
		uint32_t unk[3];
	};

	struct PUPFile
	{
		uint32_t offset;
		uint32_t fileSize;
		uint32_t unk[2];
		char fileName[32];
	};

	static_assert(sizeof(PUPHeader) == 32, "PUPHeader size mismatch");
	static_assert(sizeof(PUPFile) == 48, "PUPFile entry size mismatch");

	class PUP_Loader
	{
		utl::File& file;
		PUPHeader header;
		std::vector<PUPFile> entries;

	public:

		PUP_Loader(utl::File&);
		utl::File GetFile(const char*);
	};
}