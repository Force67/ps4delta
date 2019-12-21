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

	void* allocMem(void* preferredAddr, size_t length, pageProtection, allocationType);
}