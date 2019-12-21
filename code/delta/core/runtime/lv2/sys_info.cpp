
// Copyright (C) Force67 2019

#include <string>
#include <base.h>

#include <intrin.h>

namespace runtime
{
	int PS4ABI sys_is_in_sandbox()
	{
		return 0;
	}

	int PS4ABI sys_sysctl(int* name, uint32_t namelen, void* oldp, size_t* oldlenp, const void* newp, size_t newlen)
	{
		// for sceKernelGetAppInfo
		if (name[0] == 1 && namelen == 4 && *oldlenp == 72) {
			return 0;
		}

		if (name[0] == 0 && name[1] == 3 && namelen == 2) {
			auto name = std::string_view(static_cast<const char*>(newp), newlen);
			if (name == "kern.neomode") {

			}
		}

		/*for sceKernelGetLibkernelTextLocation*/

		std::printf("sysctl referenced by %p\n", _ReturnAddress());
		__debugbreak();
		return 0;
	}
}