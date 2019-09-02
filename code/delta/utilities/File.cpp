
// Copyright (C) Force67 2019

#include <cstdio>
#include "File.h"

namespace utl
{
	// todo: maybe make native?
	File::File(const std::wstring& dir, fileMode mode /* = fileMode::read */)
	{
		std::FILE* fptr = nullptr;

		// convert access mode
		const wchar_t* modeStr = L"a+";
		if (mode == fileMode::read)
			modeStr = L"rb";
		else if (mode == fileMode::write)
			modeStr = L"wb";

		_wfopen_s(&fptr, dir.c_str(), modeStr);
		handle = static_cast<native_handle>(fptr);
		
		// we can cache the size now
		if (fptr && mode == fileMode::read) {

			// determine initial size
			std::fseek(fptr, 0, SEEK_END);
			sizeTracker = static_cast<size_t>(std::ftell(fptr));
		}
		else {
			sizeTracker = 0;
		}

		if (fptr) {
			// ensure that we can always start from offset 0
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

		uint64_t x = std::fread(buf, size, 1, static_cast<std::FILE*>(handle));
#if DELTA_DBG
		if (x != 1) {
			__debugbreak();
		}
#endif

		if (x == 1)
			return size;

		return x;
	}

	uint64_t File::Write(const void* buf, size_t size) {
		sizeTracker += size;
		return std::fwrite(buf, size, 1, static_cast<std::FILE*>(handle));
	}

	uint64_t File::Seek(int64_t ofs, seekMode mode)
	{
		std::FILE* fptr = static_cast<std::FILE*>(handle);

		// translate mode
		int32_t origin = 0;
		switch (mode){
		case seekMode::seek_cur: origin = SEEK_CUR; break;
		case seekMode::seek_end: origin = SEEK_END; break;
		case seekMode::seek_set: origin = SEEK_SET; break;
		default: return 0;
		}

		uint64_t x = std::fseek(fptr, static_cast<uint32_t>(ofs), origin);
		if (x == 0)
			return static_cast<uint64_t>(std::ftell(fptr));

		return x;
	}

	uint64_t File::Tell() {
		return std::ftell(static_cast<std::FILE*>(handle));
	}

	uint64_t File::GetSize() {

		/*if (sizeTracker == 0) {
			std::FILE* fptr = static_cast<std::FILE*>(handle);

			std::fseek(fptr, 0, SEEK_END);
			sizeTracker = static_cast<size_t>(std::ftell(fptr));
			std::fseek(fptr, 0, SEEK_SET);
		}*/

		return sizeTracker;
	}

	// was intended in another way
	native_handle File::GetNativeHandle() {
		return handle;
	}
}