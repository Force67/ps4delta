#pragma once

// Copyright (C) Force67 2019

#include "../lv2/error_table.h"

namespace krnl
{
	class device
	{
	public:
		virtual SysError init(const char*, uint32_t, uint32_t) = 0;
	};
}