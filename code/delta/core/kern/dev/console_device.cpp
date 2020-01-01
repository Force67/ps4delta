
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "console_device.h"

namespace krnl
{
	consoleDevice::consoleDevice(proc* p) :
		device(p)
	{}

	bool consoleDevice::init(const char*, uint32_t, uint32_t)
	{
		return true;
	}
}