
// Copyright (C) Force67 2019

#include <cstdio>
#include "File.h"

namespace utl
{
	// todo: maybe make native?
	File::File(const std::wstring& dir, fileMode mode /* = fileMode::read */)
	{
		std::FILE* fptr = nullptr;

		const wchar_t* modeStr = L"rb";
		_wfopen_s(&fptr, dir.c_str(), modeStr);
		
		if (fptr) {
			handle = static_cast<native_handle>(fptr);

			// determine initial size
			std::fseek(fptr, 0, SEEK_END);
			sizeTracker = static_cast<size_t>(std::ftell(fptr));
			std::fseek(fptr, 0, SEEK_SET);
		}
	}

	File::~File() {
		Close();
	}

	void File::Close() {
		if (handle) {
			std::fclose(static_cast<std::FILE*>(handle));
			handle = nullptr;
		}
	}

	uint64_t File::Read(void* buf, size_t size) {
		return std::fread(buf, size, 1, static_cast<std::FILE*>(handle));
	}

	uint64_t File::Write(const void* buf, size_t size) {
		sizeTracker += size;
		return std::fwrite(buf, size, 1, static_cast<std::FILE*>(handle));
	}

	uint64_t File::Seek(int64_t ofs, seekMode mode)
	{
		// translate mode
		int32_t origin = 0;
		switch (mode){
		case seekMode::seek_cur: origin = SEEK_CUR; break;
		case seekMode::seek_end: origin = SEEK_END; break;
		case seekMode::seek_set: origin = SEEK_SET; break;
		default: return 0;
		}

		return std::fseek(static_cast<std::FILE*>(handle), static_cast<uint32_t>(ofs), origin);
	}

	uint64_t File::GetSize() {
		return sizeTracker;
	}

	// was intended in another way
	native_handle File::GetNativeHandle() {
		return handle;
	}
}