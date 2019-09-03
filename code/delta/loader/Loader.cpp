
// Copyright (C) Force67 2019

#include <loader/Loader.h>
#include <loader/SELFLoader.h>
#include <loader/ELFLoader.h>
#include <crypto/UnSELF.h>

namespace loaders
{
	namespace
	{
		FileType IdentifyType(utl::File &file)
		{
			FileType type;

#define CHECK_TYPE(name)                             \
    type = ##name##_Loader::IdentifyType(file);      \
    if (FileType::UNKNOWN != type)                   \
        return type;

				//CHECK_TYPE(SELF)
				CHECK_TYPE(ELF)

#undef CHECK_TYPE

				return FileType::UNKNOWN;
		}
	}

	std::unique_ptr<AppLoader> CreateLoader(const std::wstring &name)
	{
		auto file = std::make_unique<utl::File>(name);
		if (file->IsOpen()) {

			FileType type = IdentifyType(*file);
			if (type == FileType::UNKNOWN)
				return nullptr;

			std::vector<uint8_t> buffer(file->GetSize());
			file->Read(buffer);

			switch (type) {
			case FileType::SELF:
			{
				// convert to elf
				bool result = crypto::convert_self(*file, LR"(C:\Users\vince\Desktop\.nomad\JOURNEY_HD\CUSA02172\decrypt.elf)");
				if (!result)
					__debugbreak();

				break;
			}
			case FileType::ELF:
			{
				auto stream = utl::make_stream(std::move(buffer));
				return std::make_unique<ELF_Loader>(stream);
			}
			}
		}

		return nullptr;
	}
}