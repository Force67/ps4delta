#pragma once

// Copyright (C) Force67 2019

#include <type_traits>
#include <cstdint>

namespace utl
{
	enum class allocationType {
		reserve,
		commit,
		reservecommit
	};

	enum class pageProtection : uint32_t {
		priv = 0,
		r = 1,
		w = 1 << 1,
		x = 1 << 2,
		rx = r | x,
		rwx = r | w | x,
	};

	inline bool operator &(pageProtection lhs, pageProtection rhs) {
		return (static_cast<uint32_t>(lhs)& static_cast<uint32_t>(rhs));
	}

	inline pageProtection operator | (pageProtection lhs, pageProtection rhs) {
		return static_cast<pageProtection>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs));
	}

	inline pageProtection& operator |= (pageProtection& lhs, pageProtection rhs)
	{
		lhs = lhs | rhs;
		return lhs;
	}

	void* allocMem(void* preferredAddr, size_t len, pageProtection, allocationType);
	void freeMem(void* addr);
	bool protectMem(void* addr, size_t len, pageProtection);

	size_t getAvailableMem();
}