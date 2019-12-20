#pragma once

// Copyright (C) Force67 2019

#include <cstdint>
#include <vector>

namespace krnl
{
	struct pageInfo
	{
		uint8_t* ptr;
		size_t size;

		pageInfo(uint8_t* p, size_t s) :
			ptr(p), size(s)
		{}
	};

	class vmManager
	{
	public:
		uint8_t* mapCodeMemory(uint8_t* preference, size_t size);
		uint8_t* mapRtMemory(size_t size);

		void unmapRtMemory(uint8_t*);

	private:
		size_t codeMemTotal{ 0 };
		size_t rtMemTotal{ 0 };

		std::vector<pageInfo> codePages;
		std::vector<pageInfo> rtPages;
	};
}