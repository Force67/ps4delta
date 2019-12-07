
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
				return FileType::UNKNOWN;
		}
	}

	std::unique_ptr<AppLoader> CreateLoader(const std::wstring &name)
	{
		utl::File file(name);
		if (file.IsOpen()) {

			FileType type = IdentifyType(file);
			if (type == FileType::UNKNOWN)
				return nullptr;

			// allocate a temp buffer
			file.Seek(0, utl::seekMode::seek_set);

			auto buf = std::make_unique<uint8_t[]>(file.GetSize());
			file.Read(buf.get(), file.GetSize());

			switch (type) {
			case FileType::SELF:
			{
				// convert to elf
				bool result = crypto::convert_self(file, LR"(C:\Users\vince\Desktop\.nomad\JOURNEY_HD\CUSA02172\decrypt.elf)");
				if (!result)
					__debugbreak();

				break;
			}
			case FileType::ELF:
			{
				return std::make_unique<ELF_Loader>(std::move(buf));
			}
			}
		}

		return nullptr;
	}
}