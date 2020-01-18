#pragma once

// Copyright (C) Force67 2019

#include <utl/file.h>

namespace crypto {
#if 0

	// convert SELF to elf and return new image in out
	bool convert_self(utl::File&, std::vector<uint8_t> &&out);
	bool convert_self(utl::File&, const std::wstring& dest);
#endif
}