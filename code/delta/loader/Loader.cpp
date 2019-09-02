
// Copyright (C) Force67 2019

#include <loader/Loader.h>
#include <loader/SELFLoader.h>

namespace loaders
{
	namespace
	{
		FileType IdentifyType(utl::FileHandle &file)
		{
			FileType type;

#define CHECK_TYPE(name)                             \
    type = ##name##_Loader::IdentifyType(file);      \
    if (FileType::UNKNOWN != type)                   \
        return type;

				CHECK_TYPE(SELF)

#undef CHECK_TYPE

				return FileType::UNKNOWN;
		}
	}

	std::unique_ptr<AppLoader> CreateLoader(const std::wstring &name)
	{
		auto file = std::make_shared<utl::File>(name);
		if (file->IsOpen()) {

			// identify type by magic
			FileType type = IdentifyType(file);

			switch (type) {
			case FileType::SELF:
				return std::make_unique<SELF_Loader>(std::move(file));
			}
		}

		return nullptr;
	}
}