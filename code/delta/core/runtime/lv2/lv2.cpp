
// Copyright (C) Force67 2019

#include <cstdint>
#include <cstdio>

#include "../vprx/vprx.h"

#include "sys_mem.h"

namespace runtime
{
	int sys_mdbg_service();
	int sys_write(uint32_t fd, const void* buf, size_t nbytes);
	int sys_sysctl(int*, uint32_t, void*, size_t*, const void*, size_t);
	int sys_sigprocmask(int, const int*, int*);
	int sys_regmgr_call(uint32_t op, uint32_t id, void* result, void* value, uint64_t type);

	int sys_namedobj_create(int);
	int sys_namedobj_delete();

	int sys_dynlib_process_needed_and_relocate();
	int sys_dynlib_get_proc_param(void**, size_t*);
	int sys_dynlib_get_list(uint32_t*, size_t, size_t*);
	int sys_dynlib_get_info_ex(uint32_t, int32_t, struct dynlib_info*);

	int sys_budget_get_ptype();

	int sys_getpid();
	int sys_exit();
	int sys_rfork();
	int sys_execve();

	static int PS4ABI null_handler()
	{
		std::printf(">>>>>>>>>>>>> NULL HANDLER CALLED BY %p\n", _ReturnAddress());
		return 0;
	}

	static int PS4ABI null_handler_notable()
	{
		std::printf(">>>>>>>>>>>>> NULL HANDLER NULLTABLE CALLED BY %p\n", _ReturnAddress());
		return 0;
	}

	struct syscall_Reg {
		uint32_t id;
		const void* ptr;
	};

	static const syscall_Reg syscall_dpt[] = {
		{1, (void*)&sys_exit},
		{4, (void*)&sys_write},
		{20, (void*)&sys_getpid},
		{59, (void*)&sys_execve},
		{99, (void*)&null_handler}, //sys_netcontrol
		{101, (void*)&null_handler}, //sys_netabort
		{102, (void*)&null_handler}, //sys_netgetsockinfo
		{113, (void*)&null_handler}, //sys_socketex
		{114, (void*)&null_handler}, //sys_socketclose
		{125, (void*)&null_handler}, //sys_netgetiflist
		{141, (void*)&null_handler}, //sys_kqueueex
		{202, (void*)&sys_sysctl},
		{251, (void*)&sys_rfork},
		{340, (void*)&sys_sigprocmask},
		{379, (void*)&null_handler}, //sys_mtypeprotect
		{477, (void*)&sys_mmap},
		{532, (void*)&sys_regmgr_call},
		{533, (void*)&null_handler}, //sys_jitshm_create
		{534, (void*)&null_handler}, //sys_jitshm_alias
		{535, (void*)&null_handler}, //sys_dl_get_list
		{536, (void*)&null_handler}, //sys_dl_get_info
		{537, (void*)&null_handler}, //sys_dl_notify_event
		{538, (void*)&null_handler}, //sys_evf_create
		{539, (void*)&null_handler}, //sys_evf_delete
		{540, (void*)&null_handler}, //sys_evf_open
		{541, (void*)&null_handler}, //sys_evf_close
		{542, (void*)&null_handler}, //sys_evf_wait
		{543, (void*)&null_handler}, //sys_evf_trywait
		{544, (void*)&null_handler}, //sys_evf_set
		{545, (void*)&null_handler}, //sys_evf_clear
		{546, (void*)&null_handler}, //sys_evf_cancel
		{547, (void*)&null_handler}, //sys_query_memory_protection
		{548, (void*)&null_handler}, //sys_batch_map
		{549, (void*)&null_handler}, //sys_osem_create
		{550, (void*)&null_handler}, //sys_osem_delete
		{551, (void*)&null_handler}, //sys_osem_open
		{552, (void*)&null_handler}, //sys_osem_close
		{553, (void*)&null_handler}, //sys_osem_wait
		{554, (void*)&null_handler}, //sys_osem_trywait
		{555, (void*)&null_handler}, //sys_osem_post
		{556, (void*)&null_handler}, //sys_osem_cancel
		{557, (void*)&sys_namedobj_create},
		{558, (void*)&sys_namedobj_delete},
		{559, (void*)&null_handler}, //sys_set_vm_container
		{560, (void*)&null_handler}, //sys_debug_init
		{561, (void*)&null_handler}, //sys_suspend_process
		{562, (void*)&null_handler}, //sys_resume_process
		{563, (void*)&null_handler}, //sys_opmc_enable
		{564, (void*)&null_handler}, //sys_opmc_disable
		{565, (void*)&null_handler}, //sys_opmc_set_ctl
		{566, (void*)&null_handler}, //sys_opmc_set_ctr
		{567, (void*)&null_handler}, //sys_opmc_get_ctr
		{568, (void*)&null_handler}, //sys_budget_create
		{569, (void*)&null_handler}, //sys_budget_delete
		{570, (void*)&null_handler}, //sys_budget_get
		{571, (void*)&null_handler}, //sys_budget_set
		{572, (void*)&null_handler}, //sys_virtual_query
		{573, (void*)&null_handler}, //sys_mdbg_call
		{574, (void*)&null_handler}, //sys_sblock_create
		{575, (void*)&null_handler}, //sys_sblock_delete
		{576, (void*)&null_handler}, //sys_sblock_enter
		{577, (void*)&null_handler}, //sys_sblock_exit
		{578, (void*)&null_handler}, //sys_sblock_xenter
		{579, (void*)&null_handler}, //sys_sblock_xexit
		{580, (void*)&null_handler}, //sys_eport_create
		{581, (void*)&null_handler}, //sys_eport_delete
		{582, (void*)&null_handler}, //sys_eport_trigger
		{583, (void*)&null_handler}, //sys_eport_open
		{584, (void*)&null_handler}, //sys_eport_close
		{585, (void*)&null_handler}, //sys_is_in_sandbox
		{586, (void*)&null_handler}, //sys_dmem_container
		{587, (void*)&null_handler}, //sys_get_authinfo
		{588, (void*)&sys_mname}, 
		{589, (void*)&null_handler}, //sys_dynlib_dlopen
		{590, (void*)&null_handler}, //sys_dynlib_dlclose
		{591, (void*)&null_handler}, //sys_dynlib_dlsym
		{592, (void*)&sys_dynlib_get_list},
		{593, (void*)&null_handler}, //sys_dynlib_get_info
		{594, (void*)&null_handler}, //sys_dynlib_load_prx
		{595, (void*)&null_handler}, //sys_dynlib_unload_prx
		{596, (void*)&null_handler}, //sys_dynlib_do_copy_relocations
		{597, (void*)&null_handler}, //sys_dynlib_prepare_dlclose
		{598, (void*)&sys_dynlib_get_proc_param},
		{599, (void*)&sys_dynlib_process_needed_and_relocate},
		{600, (void*)&null_handler}, //sys_sandbox_path
		{601, (void*)&sys_mdbg_service},
		{602, (void*)&null_handler}, //sys_randomized_path
		{603, (void*)&null_handler}, //sys_rdup
		{604, (void*)&null_handler}, //sys_dl_get_metadata
		{605, (void*)&null_handler}, //sys_workaround8849
		{606, (void*)&null_handler}, //sys_is_development_mode
		{607, (void*)&null_handler}, //sys_get_self_auth_info
		{608, (void*)&sys_dynlib_get_info_ex},
		{609, (void*)&null_handler}, //sys_budget_getid
		{610, (void*)&sys_budget_get_ptype},
		{611, (void*)&null_handler}, //sys_get_paging_stats_of_all_threads
		{612, (void*)&null_handler}, //sys_get_proc_type_info
		{613, (void*)&null_handler}, //sys_get_resident_count
		{614, (void*)&null_handler}, //sys_prepare_to_suspend_process
		{615, (void*)&null_handler}, //sys_get_resident_fmem_count
		{616, (void*)&null_handler}, //sys_thr_get_name
		{617, (void*)&null_handler}, //sys_set_gpo
		{618, (void*)&null_handler}, //sys_get_paging_stats_of_all_objects
		{619, (void*)&null_handler}, //sys_test_debug_rwmem
		{620, (void*)&null_handler}, //sys_free_stack
		{621, (void*)&null_handler}, //sys_suspend_system
		{622, (void*)&null_handler}, //sys_ipmimgr_call
		{623, (void*)&null_handler}, //sys_get_gpo
		{624, (void*)&null_handler}, //sys_get_vm_map_timestamp
		{625, (void*)&null_handler}, //sys_opmc_set_hw
		{626, (void*)&null_handler}, //sys_opmc_get_hw
		{627, (void*)&null_handler}, //sys_get_cpu_usage_all
		{628, (void*)&null_handler}, //sys_mmap_dmem
		{629, (void*)&null_handler}, //sys_physhm_open
		{630, (void*)&null_handler}, //sys_physhm_unlink
		{631, (void*)&null_handler}, //sys_resume_internal_hdd
		{632, (void*)&null_handler}, //sys_thr_suspend_ucontext
		{633, (void*)&null_handler}, //sys_thr_resume_ucontext
		{634, (void*)&null_handler}, //sys_thr_get_ucontext
		{635, (void*)&null_handler}, //sys_thr_set_ucontext
		{636, (void*)&null_handler}, //sys_set_timezone_info
		{637, (void*)&null_handler}, //sys_set_phys_fmem_limit
		{638, (void*)&null_handler}, //sys_utc_to_localtime
		{639, (void*)&null_handler}, //sys_localtime_to_utc
		{640, (void*)&null_handler}, //sys_set_uevt
		{641, (void*)&null_handler}, //sys_get_cpu_usage_proc
		{642, (void*)&null_handler}, //sys_get_map_statistics
		{643, (void*)&null_handler}, //sys_set_chicken_switches
		{644, (void*)&null_handler}, //sys_extend_page_table_pool
		{645, (void*)&null_handler}, //sys_#645
		{646, (void*)&null_handler}, //sys_get_kernel_mem_statistics
		{647, (void*)&null_handler}, //sys_get_sdk_compiled_version
		{648, (void*)&null_handler}, //sys_app_state_change
		{649, (void*)&null_handler}, //sys_dynlib_get_obj_member
		{650, (void*)&null_handler}, //sys_budget_get_ptype_of_budget
		{651, (void*)&null_handler}, //sys_prepare_to_resume_process
		{652, (void*)&null_handler}, //sys_process_terminate
		{653, (void*)&null_handler}, //sys_blockpool_open
		{654, (void*)&null_handler}, //sys_blockpool_map
		{655, (void*)&null_handler}, //sys_blockpool_unmap
		{656, (void*)&null_handler}, //sys_dynlib_get_info_for_libdbg
		{657, (void*)&null_handler}, //sys_blockpool_batch
		{658, (void*)&null_handler}, //sys_fdatasync
		{659, (void*)&null_handler}, //sys_dynlib_get_list2
		{660, (void*)&null_handler}, //sys_dynlib_get_info2
		{661, (void*)&null_handler}, //sys_aio_submit
		{662, (void*)&null_handler}, //sys_aio_multi_delete
		{663, (void*)&null_handler}, //sys_aio_multi_wait
		{664, (void*)&null_handler}, //sys_aio_multi_poll
		{665, (void*)&null_handler}, //sys_aio_get_data
		{666, (void*)&null_handler}, //sys_aio_multi_cancel
		{667, (void*)&null_handler}, //sys_get_bio_usage_all
		{668, (void*)&null_handler}, //sys_aio_create
		{669, (void*)&null_handler}, //sys_aio_submit_cmd
		{670, (void*)&null_handler}, //sys_aio_init
		{671, (void*)&null_handler}, //sys_get_page_table_stats
		{672, (void*)&null_handler}, //sys_dynlib_get_list_for_libdbg
		{673, (void*)&null_handler}, //unk
		{674, (void*)&null_handler}, //unk
		{675, (void*)&null_handler}, //unk
		{676, (void*)&null_handler}, //unk
		{677, (void*)&null_handler}, //unk
	};

	uintptr_t lv2_get(uint32_t sid)
	{
		for (auto& it : syscall_dpt) {
			if (sid == it.id) {
				return reinterpret_cast<uintptr_t>(it.ptr);
			}
		}

		return reinterpret_cast<uintptr_t>(&null_handler_notable);
	}
}