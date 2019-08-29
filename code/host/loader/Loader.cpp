
// Copyright (C) Force67 2019

#include <loader/Loader.h>

#include <loader/SELFLoader.h>

namespace loaders
{
	namespace
	{
		FileType IdentifyType(const std::wstring& file)
		{
			FileType type;

#define CHECK_TYPE(name)                                                                         \
    type = ##name _Loader::IdentifyType(file);                                                 \
    if (FileType::Error != type)                                                                   \
        return type;

				CHECK_TYPE(SELF)

#undef CHECK_TYPE

				return FileType::UNKNOWN;
		}
	}

	std::unique_ptr<AppLoader> CreateLoader()
	{

	}
}