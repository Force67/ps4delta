
// Copyright (C) Force67 2019

#include <logger/logger.h>
#include <base.h>

#include <kernel/proc.h>

namespace runtime 
{
	using namespace krnl;

	int PS4ABI sys_budget_get_ptype()
	{
		/*proc type, related to telemetry etc*/
		return 1;
	}
}