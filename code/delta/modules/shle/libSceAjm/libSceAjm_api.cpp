
// Copyright (C) 2019 Force67

// This file was generated on Sat Sep  7 22:01:24 2019

#include "../../ModuleLinker.h"

#include "libSceAjm.h"

static const mlink::FunctionInfo functions[] = {
	{0x3550D78947AC49B0, &sceAjmBatchCancel},
	{0x59F022056F1671E9, &sceAjmBatchErrorDump},
	{0x7660F26CDFFF167F, &sceAjmBatchJobControlBufferRa},
	{0xB2D96086789CDC97, &sceAjmBatchJobInlineBuffer},
	{0x125B25382A4E227B, &sceAjmBatchJobRunBufferRa},
	{0xEE37405CAFB67CCA, &sceAjmBatchJobRunSplitBufferRa},
	{0x7C5164934C5F196B, &sceAjmBatchStartBuffer},
	{0xFEA2EC7C3032C086, &sceAjmBatchWait},
	{0xD6DDE2C58357CAE7, &sceAjmDecAt9ParseConfigData},
	{0x78315E4F28BE1B76, &sceAjmDecMp3ParseFrame},
	{0x307BABEAA0AC52EB, &sceAjmFinalize},
	{0x765FB87874B352EE, &sceAjmInitialize},
	{0x7625E340D88CBBFB, &sceAjmInstanceCodecType},
	{0x031A03AC8369E09F, &sceAjmInstanceCreate},
	{0x45B2DBB8ABFCCE1A, &sceAjmInstanceDestroy},
	{0x603151D1D0D51808, &sceAjmInstanceExtend},
	{0xAE02E399F757A1C2, &sceAjmInstanceSwitch},
	{0x6E44471181BA9443, &sceAjmMemoryRegister},
	{0xA48A4689A6241E43, &sceAjmMemoryUnregister},
	{0x43777216EC069FAE, &sceAjmModuleRegister},
	{0x5A2EC3B652D5F8A2, &sceAjmModuleUnregister},
	{0x03185CA95BF90185, &sceAjmStrError},
};

MODULE_INIT(libSceAjm);
