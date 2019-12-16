
// Copyright (C) Force67 2019

#include <logger/logger.h>
#include "../vprx/vprx.h"

namespace runtime
{
	int PS4ABI sys_mdbg_service()
	{
		__debugbreak();
		return 0;
	}

	struct nonsys_int
	{
		union
		{
			uint64_t encoded_id;
			struct
			{
				uint8_t data[4];
				uint8_t table;
				uint8_t index;
				uint16_t checksum;
			}
			encoded_id_parts;
		};
		uint32_t unknown;
		uint32_t value;
	};

	/*TODO: clearly does not belong here*/
	int PS4ABI sys_regmgr_call(uint32_t op, uint32_t id, void* result, void* value, uint64_t type)
	{
		__debugbreak();
		if (op == 25) // non-system get int
		{
			auto int_value = static_cast<nonsys_int*>(value);

			if (int_value->encoded_id == 0x0CAE671ADF3AEB34ull ||
				int_value->encoded_id == 0x338660835BDE7CB1ull)
			{
				int_value->value = 0;
				return 0;
			}

			return 0x800D0203;
		}

		return -1;
	}

	int PS4ABI sys_sysctl(int* name, uint32_t namelen, void* oldp, size_t* oldlenp, const void* newp, size_t newlen)
	{
		// for sceKernelGetAppInfo
		if (name[0] == 1 && namelen == 4 && *oldlenp == 72) {
			return 0;
		}

		/*for sceKernelGetLibkernelTextLocation*/

		std::printf("sysctl referenced by %p\n", _ReturnAddress());
		__debugbreak();
		return 0;
	}

	int PS4ABI sys_getpid()
	{
		return 0x1337;
	}

	bool PS4ABI sys_write(uint32_t fd, const void* buf, size_t nbytes)
	{
		if (fd == 1 || fd == 2) // stdout, stderr
		{
			auto b = static_cast<const char*>(buf);
			for (size_t i = 0; i < nbytes; ++i, ++b)
			{
				printf("%c", *b);
			}
			return true;
		}

		return false;
	}
}