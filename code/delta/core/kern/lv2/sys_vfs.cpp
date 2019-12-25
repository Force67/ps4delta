
// Copyright (C) Force67 2019

#include <base.h>
#include <memory>
#include "../dev/console_dev.h"

namespace krnl
{
	std::shared_ptr<device> make_device(const char* deviceName)
	{
		std::shared_ptr<device> dev(nullptr);

		if (std::strcmp(deviceName, "console") == 0)
			dev = std::make_shared<consoleDevice>();

		return dev;
	}

	int PS4ABI sys_open(const char* path, uint32_t flags, uint32_t mode)
	{
		if (!path)
			return SysError::eINVAL;

		if (std::strncmp(path, "/dev/", 5) == 0) {
			const char* name = &path[5];

			if (std::strcmp(name, "deci_tty6") == 0)
				return 0;

			// this is far from finished
			return make_device(name)->init(path, flags, mode);
		}


		std::printf("open: %s, %x, %x\n", path, flags, mode);
		__debugbreak();
		return 0;
	}
}