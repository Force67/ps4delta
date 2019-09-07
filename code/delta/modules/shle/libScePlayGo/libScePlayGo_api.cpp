
// Copyright (C) 2019 Force67

// This file was generated on Sat Sep  7 22:01:27 2019

#include "../../ModuleLinker.h"

#include "libScePlayGo.h"

static const mlink::FunctionInfo functions[] = {
	{0xB84A8C7CC213BC42, &sceDbgPlayGoRequestNextChunk},
	{0xBD4F85AAB1FEA446, &sceDbgPlayGoSnapshot},
	{0x51CA352347650E2F, &scePlayGoClose},
	{0xEF77C5D4C154F210, &scePlayGoGetChunkId},
	{0xBFA119FD859174CB, &scePlayGoGetEta},
	{0xAEF0527D38A67A31, &scePlayGoGetInstallSpeed},
	{0xDCE31B61905A6B9D, &scePlayGoGetLanguageMask},
	{0xB962182C5924C2A9, &scePlayGoGetLocus},
	{0xFD125634C2B77C2F, &scePlayGoGetProgress},
	{0x367EF32B09C0E6AD, &scePlayGoGetToDoList},
	{0xB6CE8695938A46B1, &scePlayGoInitialize},
	{0x3351A66B5A1CAC61, &scePlayGoOpen},
	{0xFD0D7FBB56BBA748, &scePlayGoPrefetch},
	{0xE0001C4D4F51DD73, &scePlayGoSetInstallSpeed},
	{0x2E8B0B9473A936A4, &scePlayGoSetLanguageMask},
	{0x8143C688E435B664, &scePlayGoSetToDoList},
	{0x30F7B411E04633F1, &scePlayGoTerminate},
};

MODULE_INIT(libScePlayGo);
