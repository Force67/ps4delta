
// Copyright (C) 2019 Force67

// This file was generated on Sat Sep  7 22:01:24 2019

#include "../../ModuleLinker.h"

#include "libSceAudiodec.h"

static const mlink::FunctionInfo functions[] = {
	{0xE957FD5932C3A2CB, &sceAudiodecClearContext},
	{0x3B77F5B0B31646FB, &sceAudiodecCreateDecoder},
	{0x2875C73032E420BC, &sceAudiodecDecode},
	{0xF9E14E187FF603C3, &sceAudiodecDecode2},
	{0x4E9F99132EBD98B9, &sceAudiodecDeleteDecoder},
	{0x56386C9B1A5C7B32, &sceAudiodecInitLibrary},
	{0x8798D20764080D5D, &sceAudiodecTermLibrary},
	{0x4B8A5EAFCDD50126, &unk_S4per83VASY},
	{0x55430EDA401BA3BC, &unk_VUMO2kAbo7w},
};

MODULE_INIT(libSceAudiodec);
