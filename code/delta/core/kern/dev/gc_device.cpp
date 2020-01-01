
// Copyright (C) Force67 2019

#include "gc_device.h"

namespace krnl
{
	gcDevice::gcDevice(proc* p) :
		device(p)
	{}

	bool gcDevice::init(const char*, uint32_t, uint32_t)
	{
		return true;
	}

	/*map to gfx memory*/
	uint8_t* gcDevice::map(void* addr, size_t, uint32_t, uint32_t, size_t)
	{
		//__debugbreak();
		return reinterpret_cast<uint8_t*>(-1);
	}
}