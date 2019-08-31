
// Copyright (C) Force67 2019

#include <kernel/Process.h>

namespace krnl
{
	Process::Process() :
		creationStamp(std::chrono::steady_clock::now())
	{
	}

	void Process::Start()
	{

	}
}