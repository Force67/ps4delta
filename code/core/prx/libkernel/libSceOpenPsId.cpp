
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

int PS4ABI sceKernelGetOpenPsId()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

}

IMP_FUNC(sceKernelGetOpenPsId)
