
// Copyright (C) Force67 2019

#include <cstdint>
#include <cstdio>

#include "../vprx/vprx.h"

namespace runtime
{
	int sys_mdbg_service();
	bool sys_write(uint32_t fd, const void* buf, size_t nbytes);
	int sys_sysctl(int*, uint32_t, void*, size_t*, const void*, size_t);
	int sys_regmgr_call(uint32_t op, uint32_t id, void* result, void* value, uint64_t type);

	int sys_dynlib_process_needed_and_relocate();
	int sys_dynlib_get_proc_param(void**, size_t*);
	int sys_dynlib_get_list(uint32_t*, size_t, size_t*);
	int sys_dynlib_get_info_ex(uint32_t, int32_t, struct dynlib_info*);

	int sys_budget_get_ptype();

	int sys_getpid();

	struct syscall_Reg {
		uint32_t id;
		const void* ptr;
	};

	static const syscall_Reg syscall_dpt[] = {
		{4, (void*)&sys_write},
		{20, (void*)&sys_getpid},
		{99, nullptr}, //sys_netcontrol
		{101, nullptr}, //sys_netabort
		{102, nullptr}, //sys_netgetsockinfo
		{113, nullptr}, //sys_socketex
		{114, nullptr}, //sys_socketclose
		{125, nullptr}, //sys_netgetiflist
		{141, nullptr}, //sys_kqueueex
		{202, (void*)&sys_sysctl},
		{379, nullptr}, //sys_mtypeprotect
		{532, (void*)&sys_regmgr_call},
		{533, nullptr}, //sys_jitshm_create
		{534, nullptr}, //sys_jitshm_alias
		{535, nullptr}, //sys_dl_get_list
		{536, nullptr}, //sys_dl_get_info
		{537, nullptr}, //sys_dl_notify_event
		{538, nullptr}, //sys_evf_create
		{539, nullptr}, //sys_evf_delete
		{540, nullptr}, //sys_evf_open
		{541, nullptr}, //sys_evf_close
		{542, nullptr}, //sys_evf_wait
		{543, nullptr}, //sys_evf_trywait
		{544, nullptr}, //sys_evf_set
		{545, nullptr}, //sys_evf_clear
		{546, nullptr}, //sys_evf_cancel
		{547, nullptr}, //sys_query_memory_protection
		{548, nullptr}, //sys_batch_map
		{549, nullptr}, //sys_osem_create
		{550, nullptr}, //sys_osem_delete
		{551, nullptr}, //sys_osem_open
		{552, nullptr}, //sys_osem_close
		{553, nullptr}, //sys_osem_wait
		{554, nullptr}, //sys_osem_trywait
		{555, nullptr}, //sys_osem_post
		{556, nullptr}, //sys_osem_cancel
		{557, nullptr}, //sys_namedobj_create
		{558, nullptr}, //sys_namedobj_delete
		{559, nullptr}, //sys_set_vm_container
		{560, nullptr}, //sys_debug_init
		{561, nullptr}, //sys_suspend_process
		{562, nullptr}, //sys_resume_process
		{563, nullptr}, //sys_opmc_enable
		{564, nullptr}, //sys_opmc_disable
		{565, nullptr}, //sys_opmc_set_ctl
		{566, nullptr}, //sys_opmc_set_ctr
		{567, nullptr}, //sys_opmc_get_ctr
		{568, nullptr}, //sys_budget_create
		{569, nullptr}, //sys_budget_delete
		{570, nullptr}, //sys_budget_get
		{571, nullptr}, //sys_budget_set
		{572, nullptr}, //sys_virtual_query
		{573, nullptr}, //sys_mdbg_call
		{574, nullptr}, //sys_sblock_create
		{575, nullptr}, //sys_sblock_delete
		{576, nullptr}, //sys_sblock_enter
		{577, nullptr}, //sys_sblock_exit
		{578, nullptr}, //sys_sblock_xenter
		{579, nullptr}, //sys_sblock_xexit
		{580, nullptr}, //sys_eport_create
		{581, nullptr}, //sys_eport_delete
		{582, nullptr}, //sys_eport_trigger
		{583, nullptr}, //sys_eport_open
		{584, nullptr}, //sys_eport_close
		{585, nullptr}, //sys_is_in_sandbox
		{586, nullptr}, //sys_dmem_container
		{587, nullptr}, //sys_get_authinfo
		{588, nullptr}, //sys_mname
		{589, nullptr}, //sys_dynlib_dlopen
		{590, nullptr}, //sys_dynlib_dlclose
		{591, nullptr}, //sys_dynlib_dlsym
		{592, (void*)&sys_dynlib_get_list},
		{593, nullptr}, //sys_dynlib_get_info
		{594, nullptr}, //sys_dynlib_load_prx
		{595, nullptr}, //sys_dynlib_unload_prx
		{596, nullptr}, //sys_dynlib_do_copy_relocations
		{597, nullptr}, //sys_dynlib_prepare_dlclose
		{598, (void*)&sys_dynlib_get_proc_param},
		{599, (void*)&sys_dynlib_process_needed_and_relocate},
		{600, nullptr}, //sys_sandbox_path
		{601, (void*)&sys_mdbg_service},
		{602, nullptr}, //sys_randomized_path
		{603, nullptr}, //sys_rdup
		{604, nullptr}, //sys_dl_get_metadata
		{605, nullptr}, //sys_workaround8849
		{606, nullptr}, //sys_is_development_mode
		{607, nullptr}, //sys_get_self_auth_info
		{608, (void*)&sys_dynlib_get_info_ex},
		{609, nullptr}, //sys_budget_getid
		{610, (void*)&sys_budget_get_ptype},
		{611, nullptr}, //sys_get_paging_stats_of_all_threads
		{612, nullptr}, //sys_get_proc_type_info
		{613, nullptr}, //sys_get_resident_count
		{614, nullptr}, //sys_prepare_to_suspend_process
		{615, nullptr}, //sys_get_resident_fmem_count
		{616, nullptr}, //sys_thr_get_name
		{617, nullptr}, //sys_set_gpo
		{618, nullptr}, //sys_get_paging_stats_of_all_objects
		{619, nullptr}, //sys_test_debug_rwmem
		{620, nullptr}, //sys_free_stack
		{621, nullptr}, //sys_suspend_system
		{622, nullptr}, //sys_ipmimgr_call
		{623, nullptr}, //sys_get_gpo
		{624, nullptr}, //sys_get_vm_map_timestamp
		{625, nullptr}, //sys_opmc_set_hw
		{626, nullptr}, //sys_opmc_get_hw
		{627, nullptr}, //sys_get_cpu_usage_all
		{628, nullptr}, //sys_mmap_dmem
		{629, nullptr}, //sys_physhm_open
		{630, nullptr}, //sys_physhm_unlink
		{631, nullptr}, //sys_resume_internal_hdd
		{632, nullptr}, //sys_thr_suspend_ucontext
		{633, nullptr}, //sys_thr_resume_ucontext
		{634, nullptr}, //sys_thr_get_ucontext
		{635, nullptr}, //sys_thr_set_ucontext
		{636, nullptr}, //sys_set_timezone_info
		{637, nullptr}, //sys_set_phys_fmem_limit
		{638, nullptr}, //sys_utc_to_localtime
		{639, nullptr}, //sys_localtime_to_utc
		{640, nullptr}, //sys_set_uevt
		{641, nullptr}, //sys_get_cpu_usage_proc
		{642, nullptr}, //sys_get_map_statistics
		{643, nullptr}, //sys_set_chicken_switches
		{644, nullptr}, //sys_extend_page_table_pool
		{645, nullptr}, //sys_#645
		{646, nullptr}, //sys_get_kernel_mem_statistics
		{647, nullptr}, //sys_get_sdk_compiled_version
		{648, nullptr}, //sys_app_state_change
		{649, nullptr}, //sys_dynlib_get_obj_member
		{650, nullptr}, //sys_budget_get_ptype_of_budget
		{651, nullptr}, //sys_prepare_to_resume_process
		{652, nullptr}, //sys_process_terminate
		{653, nullptr}, //sys_blockpool_open
		{654, nullptr}, //sys_blockpool_map
		{655, nullptr}, //sys_blockpool_unmap
		{656, nullptr}, //sys_dynlib_get_info_for_libdbg
		{657, nullptr}, //sys_blockpool_batch
		{658, nullptr}, //sys_fdatasync
		{659, nullptr}, //sys_dynlib_get_list2
		{660, nullptr}, //sys_dynlib_get_info2
		{661, nullptr}, //sys_aio_submit
		{662, nullptr}, //sys_aio_multi_delete
		{663, nullptr}, //sys_aio_multi_wait
		{664, nullptr}, //sys_aio_multi_poll
		{665, nullptr}, //sys_aio_get_data
		{666, nullptr}, //sys_aio_multi_cancel
		{667, nullptr}, //sys_get_bio_usage_all
		{668, nullptr}, //sys_aio_create
		{669, nullptr}, //sys_aio_submit_cmd
		{670, nullptr}, //sys_aio_init
		{671, nullptr}, //sys_get_page_table_stats
		{672, nullptr}, //sys_dynlib_get_list_for_libdbg
		{673, nullptr}, //unk
		{674, nullptr}, //unk
		{675, nullptr}, //unk
		{676, nullptr}, //unk
		{677, nullptr}, //unk
	};

	uintptr_t lv2_get(uint32_t sid)
	{
		for (auto& it : syscall_dpt) {
			if (sid == it.id) {
				return reinterpret_cast<uintptr_t>(it.ptr);
			}
		}

		return 0;
	}
}