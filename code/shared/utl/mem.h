#pragma once

// Copyright (C) Force67 2019

namespace utl
{
	enum class allocationType {
		reserve,
		commit,
		reservecommit
	};

	enum class pageProtection {
		noaccess,
		read,
		write,
		execute,
		rwx
	};

	void* allocMem(void* preferredAddr, size_t len, pageProtection, allocationType);
	bool protectMem(void* addr, size_t len, pageProtection);

	size_t getAvailableMem();
}