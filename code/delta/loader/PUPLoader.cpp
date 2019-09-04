
// Copyright (C) Force67 2019

#include "loader/PUPLoader.h"

namespace loaders
{
	PUP_Loader::PUP_Loader(utl::File& file) :
		file(file)
	{
		if (file.IsOpen()) {

			header = {};
			file.Read(header);

			entries.resize(header.fileCount);
			file.Read(entries);
		}
	}

	utl::File PUP_Loader::GetFile(const char *name)
	{
		if (file.IsOpen())
		{
			for (auto& e : entries) {
				if (std::strcmp(name, e.fileName) == 0) {
					file.Seek(e.offset, utl::seekMode::seek_set);

					// gather the data
					std::vector<uint8_t> data(e.fileSize);
					file.Read(data);

					return utl::make_stream(std::move(data));
				}
			}
		}

		return utl::File();
	}
}