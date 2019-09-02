#pragma once

// Copyright (C) Force67 2019

#include "loader/SELFLoader.h"

namespace crypto
{
	enum class convOpts
	{
		convLinux, //< convert as linux image
		convPE, //< convert PE
	};

	// convert SELF to elf and return new image in out
	void convert_self(std::FILE*, std::unique_ptr<uint8_t> &out);
}