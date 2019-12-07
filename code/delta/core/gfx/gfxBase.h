#pragma once

// Copyright (C) Force67 2019

#include <cstdint>
#include <vector>

namespace gfx
{
	class frameBase
	{
	public:
		virtual ~frameBase() = default;
		virtual void toggleFullscreen() = 0;
		virtual void takeScreenshot(std::vector<uint8_t>& data, uint32_t sizeX, uint32_t sizeY) = 0;
	};
}