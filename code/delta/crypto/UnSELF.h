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
	bool convert_self(utl::File&, std::vector<uint8_t> &&out);
	bool convert_self(utl::File&, const std::wstring& dest);
}