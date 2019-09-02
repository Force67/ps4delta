#pragma once

// Copyright (C) Force67 2019

#include "loader/SELFLoader.h"
#include "utilities/File.h"

namespace crypto
{
	enum class convOpts
	{
		convLinux, //< convert as linux image
		convPE, //< convert PE
	};

	// convert SELF to elf and return new image in out
	bool convert_self(utl::FileHandle&, std::unique_ptr<uint8_t[]> &out);
	bool convert_self(utl::FileHandle&, const std::wstring& dest);
}