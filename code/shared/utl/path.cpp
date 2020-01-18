
/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <algorithm>
#include <Windows.h>
#include <utl/path.h>

namespace utl {
std::wstring make_abs_path(const std::wstring &rel) {
  static std::wstring filePath;
  if (filePath.empty()) {
    wchar_t buf[MAX_PATH]{};

    GetModuleFileNameW(nullptr, buf, MAX_PATH);
    wchar_t *dirPtr = std::wcsrchr(buf, L'\\');
    dirPtr[1] = L'\0';

    filePath = buf;
  }

  auto newPath = filePath + rel;

  // sanitize the path (this is a windows thing, on linux & osx we
  // need to do it the other way around)
  std::replace(newPath.begin(), newPath.end(), L'/', L'\\');

  return newPath;
}

std::string make_abs_path(const std::string &rel) {
  static std::string filePath;
  if (filePath.empty()) {
    char buf[MAX_PATH]{};

    GetModuleFileNameA(nullptr, buf, MAX_PATH);
    char *dirPtr = std::strrchr(buf, '\\');
    dirPtr[1] = '\0';

    filePath = buf;
  }

  auto newPath = filePath + rel;

  // sanitize the path (this is a windows thing, on linux & osx we
  // need to do it the other way around)
  std::replace(newPath.begin(), newPath.end(), '/', '\\');

  return newPath;
}

} // namespace utl