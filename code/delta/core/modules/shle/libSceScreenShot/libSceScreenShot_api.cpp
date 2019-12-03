
// Copyright (C) 2019 Force67

// This file was generated on Sat Sep  7 22:01:28 2019

#include "../../ModuleLinker.h"

#include "libSceScreenShot.h"

static const mlink::FunctionInfo functions[] = {
	{0x012E394286078DCE, &scec__Z5dummyv1},
	{0x26E30B2E69AF4609, &sceScreenShotCapture},
	{0xB4861FD16E554E2F, &sceScreenShotDisable},
	{0xCAC7F36BBD6B9BD3, &sceScreenShotDisableNotification},
	{0xDB1C54B6E0BF4731, &sceScreenShotEnable},
	{0x04351AAA555D4806, &sceScreenShotEnableNotification},
	{0x84D98AE127613D3D, &sceScreenShotGetAppInfo},
	{0xFD257FA133461509, &sceScreenShotIsDisabled},
	{0x202349FF53CEB3CE, &sceScreenShotIsVshScreenCaptureDisabled},
	{0x6A11E139FF903644, &sceScreenShotSetOverlayImage},
	{0xEF7590E098F49C92, &sceScreenShotSetOverlayImageWithOrigin},
	{0x1BB2A59886052197, &sceScreenShotSetParam},
	{0x5650102205DAD91D, &unk_VlAQIgXa2R0},
	{0x8AD9561565774F37, &unk_itlWFWV3Tzc},
};

MODULE_INIT(libSceScreenShot);
