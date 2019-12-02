
// Copyright (C) 2019 Force67

// This file was generated on Sat Sep  7 22:01:27 2019

#include "../../ModuleLinker.h"

#include "libScePadTracker.h"

static const mlink::FunctionInfo functions[] = {
	{0x14DE14B98D82533D, &scePadTrackerCalibrate},
	{0xF1B2BFE15C63AFC1, &scePadTrackerGetWorkingMemorySize},
	{0x1B5FD1C8F948AC86, &scePadTrackerInit},
	{0x3EA9713AAFA4D36A, &scePadTrackerReadState},
	{0x0A2D0FA843804DE4, &scePadTrackerTerm},
	{0x72B8BA837AAF0AD7, &scePadTrackerUpdate},
};

MODULE_INIT(libScePadTracker);
