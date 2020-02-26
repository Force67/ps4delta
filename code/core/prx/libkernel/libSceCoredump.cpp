
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

int PS4ABI sceCoredumpAttachMemoryRegion()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceCoredumpAttachMemoryRegionAsUserFile()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceCoredumpAttachUserFile()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceCoredumpConfigDumpMode()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceCoredumpDebugTextOut()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceCoredumpGetStopInfoCpu()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceCoredumpGetStopInfoGpu()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceCoredumpGetThreadContextInfo()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceCoredumpRegisterCoredumpHandler()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceCoredumpSetUserDataType()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceCoredumpUnregisterCoredumpHandler()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceCoredumpWriteUserData()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

int PS4ABI sceCoredumpInternalGetThreadContextInfo()
{
	LOG_UNIMPLEMENTED;
	return 0;
}

}

IMP_FUNC(sceCoredumpAttachMemoryRegion)
IMP_FUNC(sceCoredumpAttachMemoryRegionAsUserFile)
IMP_FUNC(sceCoredumpAttachUserFile)
IMP_FUNC(sceCoredumpConfigDumpMode)
IMP_FUNC(sceCoredumpDebugTextOut)
IMP_FUNC(sceCoredumpGetStopInfoCpu)
IMP_FUNC(sceCoredumpGetStopInfoGpu)
IMP_FUNC(sceCoredumpGetThreadContextInfo)
IMP_FUNC(sceCoredumpRegisterCoredumpHandler)
IMP_FUNC(sceCoredumpSetUserDataType)
IMP_FUNC(sceCoredumpUnregisterCoredumpHandler)
IMP_FUNC(sceCoredumpWriteUserData)
IMP_FUNC(sceCoredumpInternalGetThreadContextInfo)
