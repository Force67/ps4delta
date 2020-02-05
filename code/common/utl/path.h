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
enum app_path {
    data_dir,
    config_dir,
    count
};

// creates a sanitized path relative to the current executable
std::string make_abs_path(std::string_view rel = nullptr);

// creates a sanitized path relative to the specified link
std::string make_app_path(app_path, std::string_view rel = nullptr);

bool exists(std::string_view rel);

bool make_dir(std::string_view rel);
}