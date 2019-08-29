
// Copyright (C) Force67 2019

#include <cstdio>
#include <loader/SELFLoader.h>

namespace loaders
{
	SELF_Loader::SELF_Loader(const std::wstring& dir) :
		AppLoader(dir)
	{}

	LoadErrorCode SELF_Loader::Load()
	{
		FILE* fh = nullptr;
		_wfopen_s(&fh, file.c_str(), L"rb");

		if (fh) {

			// determine the size
			fseek(fh, 0, SEEK_END);
			uint32_t len = ftell(fh);
			auto* data = new uint8_t[len];

			//read file in buffer
			fseek(fh, 0, SEEK_SET);
			fread(data, 1, len, fh);
			fclose(fh);

			// check for any bad magic
			SELFHeader* hdr = (SELFHeader*)data;
			if (hdr->magic != SELF_MAGIC)
				return LoadErrorCode::BadSELFMagic;
		}
	}

	LoadErrorCode SELF_Loader::Unload()
	{
		return LoadErrorCode::UNKNOWN;
	}
}