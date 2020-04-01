#pragma once

// Copyright 2013 Dolphin Emulator Project / 2014 Citra Emulator Project
// Licensed under GPLv2 or any later version
// Refer to the license.txt file included.

#include <string>
#include <vector>

namespace utl {
std::string strip_spaces(const std::string& s);
std::string strip_quotes(const std::string& s);

std::string from_bool(bool value);

std::string tabs_to_spaces(int tab_size, std::string in);

// "C:/Windows/winhelp.exe" to "C:/Windows/", "winhelp", ".exe"
bool split_path(const std::string& full_path, std::string* _pPath, std::string* _pFilename,
               std::string* _pExtension);

std::string utf16_to_utf8(const std::u16string& input);
std::u16string utf8_to_utf16(const std::string& input);

#ifdef _WIN32
std::string utf16_to_utf8(const std::wstring& input);
std::wstring utf8_to_utf16_w(const std::string& str);

// initializes utf8 command line arguments
void init_utf8_args(int& argc, char*** argv);
#endif
}