
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

	int PS4ABI sys_get_authinfo(int pid)
	{
		/*i had some odd clang error*/
		sys_is_in_sandbox();
		return 1;
	}

	int PS4ABI sys_sysctl(int* name, uint32_t namelen, void* oldp, size_t* oldlenp, const void* newp, size_t newlen)
	{
		// for sceKernelGetAppInfo
		if (name[0] == 1 && name[1] == 14 && name[2] == 35 && namelen == 4) {
			std::memset(oldp, 0, 72);
			return 0;
		}

		// kern.userstack
		else if (name[0] == 1 && name[1] == 33 && namelen == 2) {
			auto& info = proc::getActive()->getEnv();
			*static_cast<void**>(oldp) = info.userStack + info.userStackSize;
			std::printf("userstack -> base %p, end %p\n", info.userStack, oldp);
			return 0;
		}

		// kern.pagesize
		else if (name[0] == 6 && name[1] == 7 && namelen == 2) {
			*reinterpret_cast<uint32_t*>(oldp) = 4096;
			return 0;
		}

#if 0
		else if (name[0] == 0x1337 && name[1] == 1 && namelen == 2) {
			*reinterpret_cast<uint64_t*>(oldp) = 1357;
			return 0;
		}
#endif

		else if (name[0] == 0x1337 && name[1] == 1 && namelen == 2) {
			*reinterpret_cast<uint64_t*>(oldp) = 1;
			return 0;
		}

		// cxx init stuff
		else if (name[0] == 1 && name[1] == 37 && namelen == 2) {
			auto length = *oldlenp;
			if (length > 256) length = 256;
			memset(oldp, 4, length);
			*oldlenp = length;
			return 0;
		}

		// answer kern.prot.ptc
		else if (name[0] == 0x1337 && name[1] == 2 && namelen == 2) {
			*reinterpret_cast<uint64_t*>(oldp) = 1357;
			return 0;
		}

		if (name[0] == 0 && name[1] == 3 && namelen == 2) {
			auto name = std::string_view(static_cast<const char*>(newp), newlen);
			if (name == "kern.neomode") {

			}

			if (name == "kern.smp.cpus") {
				static_cast<uint32_t*>(oldp)[0] = 0x1337;
				static_cast<uint32_t*>(oldp)[1] = 1;
				*oldlenp = 8;
				return 0;
			}
			else if (name == "kern.proc.ptc")
			{
				static_cast<uint32_t*>(oldp)[0] = 0x1337;
				static_cast<uint32_t*>(oldp)[1] = 2;
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