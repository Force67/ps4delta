#pragma once

/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <string>

// implements various path utility functions
namespace utl {
std::wstring make_abs_path(const std::wstring &relative);
std::string make_abs_path(const std::string &relative);
}