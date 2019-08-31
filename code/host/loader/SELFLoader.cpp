
// Copyright (C) Force67 2019

#include <cstdio>
#include <loader/SELFLoader.h>

namespace loaders
{
	FileType SELF_Loader::IdentifyType(const uint8_t* data)
	{
		SELFHeader* hdr = (SELFHeader*)data;
		if (hdr->magic == SELF_MAGIC)
			return FileType::SELF;

		return FileType::UNKNOWN;
	}

	LoadErrorCode SELF_Loader::Load()
	{
		return LoadErrorCode::UNKNOWN;
	}

	LoadErrorCode SELF_Loader::Unload()
	{
		return LoadErrorCode::UNKNOWN;
	}
}