
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

int PS4ABI sceKernelBacktraceSelf1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelRemoveExceptionHandler1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelInstallExceptionHandler1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelAddGpuExceptionEvent1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceKernelDeleteGpuExceptionEvent1()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

}

IMP_FUNC(sceKernelBacktraceSelf1)
IMP_FUNC(sceKernelRemoveExceptionHandler1)
IMP_FUNC(sceKernelInstallExceptionHandler1)
IMP_FUNC(sceKernelAddGpuExceptionEvent1)
IMP_FUNC(sceKernelDeleteGpuExceptionEvent1)
