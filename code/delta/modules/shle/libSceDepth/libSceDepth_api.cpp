
// Copyright (C) 2019 Force67

// This file was generated on Sat Sep  7 22:01:24 2019

#include "../../ModuleLinker.h"

#include "libSceDepth.h"

static const mlink::FunctionInfo functions[] = {
	{0x9F58DF51F82F214C, &sceDepthGetCalibrationData},
	{0x9B6083CC02C7DF54, &sceDepthGetImage},
	{0x391894D2DF599F01, &sceDepthHandCandidateTrackerGetResult},
	{0x3D76D536E52A10B4, &sceDepthHasCalibrationData},
	{0x0C8DF2665ACB2F13, &sceDepthHeadCandidateTrackerGetResult},
	{0xF696D3343E36C7BE, &sceDepthHeadCandidateTrackerSetValidationInformation},
	{0x3DA6BD7601E71E94, &sceDepthInitialize},
	{0xE40B9615B56FA935, &sceDepthLoadCalibrationData},
	{0xE4CC31224DFF80BA, &sceDepthQueryMemory},
	{0xA0F8630985F539E8, &sceDepthSetCalibrationData},
	{0xEFEBF7B96447810E, &sceDepthSetCommands},
	{0xDE68ECBBAD3C8685, &sceDepthSetRoi},
	{0xF2750C4141B3DD5B, &sceDepthSetRoiset},
	{0x8CBC79A9DE2ACA39, &sceDepthSubmit},
	{0x97E74A842C6D4477, &sceDepthTerminate},
	{0xAB4357B104F3008D, &sceDepthUpdate},
	{0x0DDA262E4B4BE7E3, &sceDepthValidateCurrentCalibrationData},
	{0x7279CE316E9DC8F2, &sceDepthWaitAndExecutePostProcess},
	{0x02743DBA823710D6, &unk_AnQ9uoI3ENY},
	{0x0298815A4F978533, &unk_ApiBWk_XhTM},
	{0x0A1898EA01EB30AE, &unk_ChiY6gHrMK4},
	{0x1D9DA2879E3B9F96, &unk_HZ2ih547n5Y},
	{0x2C696CD0B8504866, &unk_LGls0LhQSGY},
	{0x2FBD0B9EB9B8833E, &unk_L70Lnrm4gz4},
	{0x5592612FDADABB4F, &unk_VZJhL9rau08},
	{0x5975EC3C791A835E, &unk_WXXsPHkag14},
	{0x5F917F7525E8514C, &unk_X5F_dSXoUUw},
	{0x6B30A640499BD7F2, &unk_azCmQEmb1_I},
	{0x6F57BCA0FAD49D8E, &unk_b1e8oPrUnY4},
	{0x84ED2CC503F99F44, &unk_hO0sxQP5n0Q},
	{0x92130745F2A15F2C, &unk_khMHRfKhXyw},
	{0x940876B5E447CCB5, &unk_lAh2teRHzLU},
	{0xB4A6F5346A832F7A, &unk_tKb1NGqDL3o},
	{0xBA72FDB16BB8208C, &unk_unL9sWu4IIw},
	{0xCBC24570B69CA66D, &unk_y8JFcLacpm0},
	{0xCC4DB0B37232AEA7, &unk_zE2ws3Iyrqc},
	{0xCF9493F422201DAD, &unk_z5ST9CIgHa0},
	{0xD5E0C52366F5F91E, &unk_1eDFI2b1_R4},
	{0xEDB4A2048F0AA81C, &unk_7bSiBI8KqBw},
};

MODULE_INIT(libSceDepth);
