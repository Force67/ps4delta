
// Copyright (C) Force67 2019

#include "../ModuleLinker.h"

struct sceProcParam
{
	uint32_t len;
};

static void* sceKernelGetProcParam()
{
	return nullptr;
}

static void sceKernelRtldSetApplicationHeapAPI(void* api) {

}

static const mlink::FunctionInfo functions[] = {
	{0, &sceKernelGetProcParam, "959qrazPIrg"},
	{1, &sceKernelRtldSetApplicationHeapAPI, "p5EcQeEeJAE"},
};

MODULE_INIT(libkernel)