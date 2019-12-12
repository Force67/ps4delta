
// Copyright (C) Force67 2019

#include <logger/logger.h>
#include "../vprx/vprx.h"

#include <kernel/proc.h>

namespace runtime 
{
	using namespace krnl;

	struct proc_param {
		uint64_t length;
		uint32_t magic;
		uint32_t unk;
		uint32_t kvers;
	};

	struct dynlib_seg {
		uint64_t addr;
		uint32_t size;
		uint32_t flags;
	};

	struct dynlib_info {
		size_t size;
		char name[256];
		int32_t id;
		uint32_t tls_index;
		uint64_t tls_init_addr;
		uint32_t tls_init_size;
		uint32_t tls_size;
		uint32_t tls_offset;
		uint32_t tls_align;
		uint64_t init_proc_addr;
		uint64_t fini_proc_addr;
		uint64_t reserved1;
		uint64_t reserved2;
		uint64_t eh_frame_hdr_addr;
		uint64_t eh_frame_addr;
		uint32_t eh_frame_hdr_size;
		uint32_t eh_frame_size;
		dynlib_seg segs[4];
		uint32_t segment_count;
		uint32_t ref_count;
	};

	static_assert(sizeof(dynlib_info) == 424);

	int sys_dynlib_get_info_ex(uint32_t handle, dynlib_info* info)
	{
		auto* mod = proc::getActive()->getModule(handle);
		if (!mod)
			/*TODO*/
			return -1;

		std::memset(info, 0, sizeof(dynlib_info));
		std::strncpy(info->name, mod->name.c_str(), 256);
		info->size = sizeof(dynlib_info);

		LOG_WARNING("FIXME: properly populate dynlib_info");
		return 0;
	}

	int PS4ABI sys_dynlib_get_proc_param(void** data, size_t* size)
	{
		static proc_param g_proc;
		g_proc.length = sizeof(proc_param);
		g_proc.magic = 0x4942524F;
		g_proc.unk = 1;
		g_proc.kvers = 0x5050000; /*pretend to be a 5.05 krnl*/

		*data = reinterpret_cast<void*>(&g_proc);
		*size = sizeof(proc_param);

		return 0;
	}

	int PS4ABI sys_dynlib_get_list(uint32_t* handles, size_t maxCount, size_t* count)
	{
		auto& list = proc::getActive()->getModuleList();

		int listCount = 0;
		for (auto& mod : list) {
			*(handles++) = mod->handle;
			listCount++;
		}

		*count = listCount;
		return 0;
	}

	int PS4ABI sys_dynlib_process_needed_and_relocate()
	{
		// we always return success here as we
		// automatically relocate everything on load
		return 0;
	}
}