
// Copyright (C) 2019 Force67

#include <utl/File.h>

#include <locale>
#include <codecvt>
#include <string>

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

int main(int argc, char** argv)
{
	std::puts("unpup - Copyright (C) Force67 2019");

	if (argc < 2) {

		std::puts(">Usage: unpup <filename>");
		return -1;
	}

	// its hacky i know - i also don't care
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

	utl::File file(converter.from_bytes(argv[1]));
	if (file.IsOpen()) {
		PUPHeader pup{};
		file.Read(pup);

		if (pup.magic != 0x32424C53) {
			std::puts("Bad magic");
			return -2;
		}

		std::printf("Found %d file entries\n", pup.fileCount);

		std::vector<PUPFile> entries(pup.fileCount);
		file.Read(entries);
		
		for (auto& e : entries) {
			file.Seek(e.offset, utl::seekMode::seek_set);
			
			std::vector<uint8_t> data(e.fileSize);
			file.Read(data);

			utl::File out(converter.from_bytes(e.fileName), utl::fileMode::write);
			if (out.IsOpen()) {
				out.Write(data);
			}
		}
	}

	return 0;
}