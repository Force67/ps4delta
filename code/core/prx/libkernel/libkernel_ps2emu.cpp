
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

int PS4ABI sceKernelJitCreateAliasOfSharedMemory2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelJitCreateSharedMemory2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelJitGetSharedMemoryInfo2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelJitMapSharedMemory2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI lk_socketpair2()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

}

IMP_FUNC(sceKernelJitCreateAliasOfSharedMemory2)
IMP_FUNC(sceKernelJitCreateSharedMemory2)
IMP_FUNC(sceKernelJitGetSharedMemoryInfo2)
IMP_FUNC(sceKernelJitMapSharedMemory2)
IMP_FUNC(lk_socketpair2)
