#pragma once

// Copyright (c) Nomad Group 2019

#include <string>

// implements various path utility functions
namespace utl
{
	std::wstring make_abs_path(const std::wstring &relative, void* handle = nullptr);
	std::string make_abs_path(const std::string &relative, void* handle = nullptr);
}