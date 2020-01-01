#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "device.h"

namespace krnl
{
	class consoleDevice : public device
	{
	public:
		consoleDevice(proc*);

		bool init(const char*, uint32_t, uint32_t) override;
	};
}