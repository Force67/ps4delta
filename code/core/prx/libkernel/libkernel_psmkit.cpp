
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

// This file was generated on 26/02/2020

#include <base.h>
#include "kernel/module.h"

namespace {

int PS4ABI lk_access1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_chdir1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_dup1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_dup22()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_ioctl2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_lstat1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_pipe1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_poll2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_pthread_get_user_context_np1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_pthread_kill2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_pthread_mutex_setname_np1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_pthread_suspend_user_context_np1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelDebugRaiseException1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelGetFsSandboxRandomWord1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelGetModuleInfoFromAddr1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelJitCreateAliasOfSharedMemory3()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelJitCreateSharedMemory3()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelJitMapSharedMemory3()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelOpenSema1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_sigaction2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_sigaddset2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_sigaltstack1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_sigdelset2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_sigemptyset2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_sigfillset2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_sigismember2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_signal1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_sigprocmask2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_sigsuspend2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_socketpair3()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_sysconf2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelDebugRaiseExceptionWithContext1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_pthread_cond_setname_np1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_pthread_resume_user_context_np1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelSetProcessProperty1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_sem_setname1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

}

IMP_FUNC(lk_access1)
IMP_FUNC(lk_chdir1)
IMP_FUNC(lk_dup1)
IMP_FUNC(lk_dup22)
IMP_FUNC(lk_ioctl2)
IMP_FUNC(lk_lstat1)
IMP_FUNC(lk_pipe1)
IMP_FUNC(lk_poll2)
IMP_FUNC(lk_pthread_get_user_context_np1)
IMP_FUNC(lk_pthread_kill2)
IMP_FUNC(lk_pthread_mutex_setname_np1)
IMP_FUNC(lk_pthread_suspend_user_context_np1)
IMP_FUNC(sceKernelDebugRaiseException1)
IMP_FUNC(sceKernelGetFsSandboxRandomWord1)
IMP_FUNC(sceKernelGetModuleInfoFromAddr1)
IMP_FUNC(sceKernelJitCreateAliasOfSharedMemory3)
IMP_FUNC(sceKernelJitCreateSharedMemory3)
IMP_FUNC(sceKernelJitMapSharedMemory3)
IMP_FUNC(sceKernelOpenSema1)
IMP_FUNC(lk_sigaction2)
IMP_FUNC(lk_sigaddset2)
IMP_FUNC(lk_sigaltstack1)
IMP_FUNC(lk_sigdelset2)
IMP_FUNC(lk_sigemptyset2)
IMP_FUNC(lk_sigfillset2)
IMP_FUNC(lk_sigismember2)
IMP_FUNC(lk_signal1)
IMP_FUNC(lk_sigprocmask2)
IMP_FUNC(lk_sigsuspend2)
IMP_FUNC(lk_socketpair3)
IMP_FUNC(lk_sysconf2)
IMP_FUNC(sceKernelDebugRaiseExceptionWithContext1)
IMP_FUNC(lk_pthread_cond_setname_np1)
IMP_FUNC(lk_pthread_resume_user_context_np1)
IMP_FUNC(sceKernelSetProcessProperty1)
IMP_FUNC(lk_sem_setname1)
