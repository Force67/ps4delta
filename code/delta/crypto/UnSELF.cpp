
// Copyright (C) Force67 2019

#include <vector>
#include "UnSELF.h"

namespace crypto
{
	using namespace loaders;

	class ElfBuilder
	{
		// in file
		utl::FileHandle &file;

		ELFHeader elf{};
		std::vector<ELFPgHeader> sections;
		std::unique_ptr<uint8_t[]> data;

		size_t deltaSELF;

	public:

		explicit ElfBuilder(utl::FileHandle &f) :
			file(f),
			deltaSELF(0)
		{}

		bool LoadHeaders(SELFHeader &self)
		{
			size_t selfdelta = sizeof(SELFHeader) + (sizeof(SELFSegmentTable) * self.numSegments);

			// get elf
			file->Seek(selfdelta, utl::seekMode::seek_set);
			file->Read(elf);

			// valid elf?
			if (elf.magic != 0x464C457F)
				return false;

			// read section info
			sections.reserve(elf.phnum);
			return file->Read(sections);
		}

		bool LoadData()
		{
			// skip SCE special (TODO: is it really always 41 bytes?!?)
			uint64_t offset = file->Seek(41, utl::seekMode::seek_cur);

			if (offset < file->GetSize()) {
				size_t delta = file->GetSize() - offset;
				data = std::make_unique<uint8_t[]>(delta);

				return file->Read(static_cast<void*>(data.get()), delta);
			}

			return false;
		}

		void ExportBuffer(std::unique_ptr<uint8_t[]> &&out) {
			out = std::move(data);
		}
	};

	bool convert_self(utl::FileHandle &file, std::unique_ptr<uint8_t[]>& out)
	{
		// reset
		file->Seek(0, utl::seekMode::seek_set);

		SELFHeader self{};
		file->Read(self);

		if (self.magic == SELF_MAGIC /*&& NotDebugging()*/) {
			ElfBuilder builder(file);

			if (!builder.LoadHeaders(self)) {
				std::puts("Unable to parse SELF headers!");
				return false;
			}

			if (!builder.LoadData()) {
				std::puts("Unable to gather ELF data!");
				return false;
			}

			// export it for us
			builder.ExportBuffer(std::move(out));

			return true;
		}

		return false;
	}

	bool convert_self(utl::FileHandle& in, const std::wstring& to)
	{
		std::unique_ptr<uint8_t[]> newElf;
		if (convert_self(in, newElf)) {

			utl::File output(to);
			if (output.IsOpen()) {
				//output.Write()
			}
		}

		return false;
	}
}