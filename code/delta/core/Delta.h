#pragma once

// Copyright (C) Force67 2019

#include <string>
#include <memory>
#include <kernel/Process.h>

class delta
{
public:
	void boot(const std::wstring& fromdir);

	static delta& get();
private:
	std::unique_ptr<krnl::Process> proc;
};