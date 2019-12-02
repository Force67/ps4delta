
// Copyright (C) 2019 Force67

// This file was generated on Sat Sep  7 22:01:24 2019

#include "../../ModuleLinker.h"

#include "libSceAvPlayer.h"

static const mlink::FunctionInfo functions[] = {
	{0x28C7046BEAC7B08A, &sceAvPlayerAddSource},
	{0xC7CBAFB8538F6615, &sceAvPlayerAddSourceEx},
	{0x3642700F32A6225C, &sceAvPlayerClose},
	{0xC3033DF608C57F56, &sceAvPlayerCurrentTime},
	{0x04E54A033466B934, &sceAvPlayerDisableStream},
	{0x38324ADAC9FDC380, &sceAvPlayerEnableStream},
	{0x5A7A7539572B6609, &sceAvPlayerGetAudioData},
	{0x77C15C6F37C0750C, &sceAvPlayerGetStreamInfo},
	{0xA37F915A71D58928, &sceAvPlayerGetVideoData},
	{0x25D92C42EF2935D4, &sceAvPlayerGetVideoDataEx},
	{0x692EBA448D201A0A, &sceAvPlayerInit},
	{0xA3D79646448BF8CE, &sceAvPlayerInitEx},
	{0x51B42861AC0EB1F6, &sceAvPlayerIsActive},
	{0x5C2F7033EC542F3F, &sceAvPlayerJumpToTime},
	{0xF72E6FF9F18DE169, &sceAvPlayerPause},
	{0x1C3D58295536EBF3, &sceAvPlayerPostInit},
	{0x6A08A0FE20D1AD31, &sceAvPlayerPrintf},
	{0xC399A80013709D16, &sceAvPlayerResume},
	{0x93FABEC4EC5D7371, &sceAvPlayerSetAvSyncMode},
	{0x7814EB799F382456, &sceAvPlayerSetLogCallback},
	{0x395B61B34C467E1A, &sceAvPlayerSetLooping},
	{0x6AFF19FBEF78AECD, &sceAvPlayerSetTrickSpeed},
	{0x113E06AFF52ED3BB, &sceAvPlayerStart},
	{0x642D7BC37BC1E4BA, &sceAvPlayerStop},
	{0x85D4F247309741E4, &sceAvPlayerStreamCount},
	{0xC8DEC986EBFC836E, &sceAvPlayerVprintf},
	{0x6EE3028897ED71FC, &unk_buMCiJftcfw},
};

MODULE_INIT(libSceAvPlayer);
