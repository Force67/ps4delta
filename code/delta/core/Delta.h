#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <memory>
#include <kernel/Process.h>

class Delta
{
	// current process
	std::unique_ptr<krnl::Process> proc;

public:

	Delta();

	void Boot(const std::wstring& fromdir);

	static Delta& Get();
};