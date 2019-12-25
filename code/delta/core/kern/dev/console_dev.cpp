
// Copyright (C) Force67 2019

#include "console_dev.h"

namespace krnl
{
	SysError krnl::consoleDevice::init(const char*, uint32_t, uint32_t)
	{
		return SysError::SUCCESS;
	}
}