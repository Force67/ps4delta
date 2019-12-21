
// Copyright (C) Force67 2019

#include <string>
#include <base.h>

#include <intrin.h>

#include "kernel/proc.h"

namespace runtime
{
	using namespace krnl;

	int PS4ABI sys_is_in_sandbox()
	{
		return 0;
	}

	int PS4ABI sys_sysctl(int* name, uint32_t namelen, void* oldp, size_t* oldlenp, const void* newp, size_t newlen)
	{
		// for sceKernelGetAppInfo
		if (name[0] == 1 && name[1] == 14 && name[2] == 35 && namelen == 4) {
			std::memset(oldp, 0, 72);
			return 0;
		}

		// kern.userstack
		if (name[0] == 1 && name[1] == 33 && namelen == 2) {
			auto &info = proc::getActive()->getEnv();
			*static_cast<void**>(oldp) = info.userStack + info.userStackSize;
			return 0;
		}

		// kern.pagesize
		if (name[0] == 6 && name[1] == 6 && namelen == 2) {
			*reinterpret_cast<uint32_t*>(oldp) = 4096;
			return 0;
		}

		if (name[0] == 0 && name[1] == 3 && namelen == 2) {
			auto name = std::string_view(static_cast<const char*>(newp), newlen);
			if (name == "kern.neomode") {

			}
			if (name == "kern.smp.cpus") {
				static_cast<uint32_t*>(oldp)[0] = 1;
				*oldlenp = 8;
				return 0;
			}
		}

		/*for sceKernelGetLibkernelTextLocation*/

		std::printf("sysctl referenced by %p\n", _ReturnAddress());
		__debugbreak();
		return 0;
	}
}