
// Copyright (C) Force67 2019

#include <vector>
#include "UnSELF.h"

namespace crypto
{
	using namespace loaders;

	class ElfBuilder
	{
		// elf header
		ELFHeader elf;

		// section headers
		std::vector<ELFPgHeader> elfpg;

		// actual ELF image data (new size)
		std::vector<uint8_t> data;

		size_t deltaSELF;

	public:

		explicit ElfBuilder(const SELFHeader& self)
		{
			// calculate the initial removeable delta
			deltaSELF = sizeof(SELFHeader) + (self.numSegments * sizeof(SELFSegmentTable));
		}

		void ExportBuffer(std::unique_ptr<uint8_t> &out)
		{

		}
	};

	void convert_self(std::FILE *fptr, std::unique_ptr<uint8_t>& out)
	{
		// determine file length
		std::fseek(fptr, 0, SEEK_END);
		uint32_t file_len = std::ftell(fptr);

		std::fseek(fptr, 0, SEEK_SET);

		SELFHeader self{};
		std::fread(&self, sizeof(SELFHeader), 1, fptr);

		// check if it really is a SELF file
		if (self.magic != SELF_MAGIC) {

			ElfBuilder builder(self);
		}

	}
}