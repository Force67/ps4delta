
// Copyright (C) Force67 2019

#include <loader/Loader.h>

#include <loader/SELFLoader.h>

namespace loaders
{
	namespace
	{
		FileType IdentifyType(const uint8_t *data)
		{
			FileType type;

#define CHECK_TYPE(name)                             \
    type = ##name##_Loader::IdentifyType(data);      \
    if (FileType::UNKNOWN != type)                   \
        return type;

				CHECK_TYPE(SELF)

#undef CHECK_TYPE

				return FileType::UNKNOWN;
		}
	}

	AppLoader::AppLoader(uint8_t *data)
		: loaded(false),
		  data(data)
	{}

	std::unique_ptr<AppLoader> CreateLoader(const std::wstring &file)
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

			std::printf("[+] Processing file with size %d\n", len);

			FileType type = IdentifyType(data);

			switch (type) {
			case FileType::SELF: 
				return std::make_unique<SELF_Loader>(data);
			}
		}

		return nullptr;
	}
}