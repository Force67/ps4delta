
/*
 * UTL : The universal utility library
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <algorithm>

#include "path.h"
#include "string_util.h"

#ifdef _WIN32
#include <Windows.h>
#include <shlobj.h> // for SHGetFolderPath
#endif

#include "logger/logger.h"

namespace utl {
#ifdef _WIN32
std::string make_abs_path(std::string_view rel) {
    static std::string filePath;
    if (filePath.empty()) {
        wchar_t buf[2048]{};

        GetModuleFileNameW(nullptr, buf, 2048);
        wchar_t* dirPtr = std::wcsrchr(buf, L'\\');
        dirPtr[1] = L'\0';

        filePath = utl::utf16_to_utf8(buf);
    }

    auto newPath = rel.empty() ? filePath : filePath + rel.data();

    // sanitize the path (this is a windows thing, on linux & osx we
    // need to do it the other way around)
    std::replace(newPath.begin(), newPath.end(), '/', '\\');

    return newPath;
}

static std::string get_home_dir() {
    PWSTR localPath = nullptr;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &localPath);
    auto newPath = utl::utf16_to_utf8(localPath);
    CoTaskMemFree(localPath);
    return newPath;
}

bool exists(std::string_view rel) {
    if (rel.empty())
        return false;

    auto newPath = utl::utf8_to_utf16_w(std::string(rel));
    return GetFileAttributesW(newPath.c_str()) != INVALID_FILE_ATTRIBUTES;
}

bool make_dir(std::string_view rel) {
    if (rel.empty())
        return false;

    auto newPath = utl::utf8_to_utf16_w(std::string(rel));

    // allows for recursive directory creation
    return SHCreateDirectoryEx(nullptr, newPath.c_str(), nullptr);
}

#endif

std::string make_app_path(app_path path, std::string_view rel /* = nullptr */) {
    static std::string filePaths[app_path::count];

    if (filePaths[path].empty()) {
        auto& newPath = filePaths[path];

        switch (path) {
        case app_path::data_dir: {

            // TODO: change backslashes
            newPath = get_home_dir() + "\\" + FXNAME + "\\";
            if (!exists(newPath))
                make_dir(newPath);

            return rel.empty() ? newPath : newPath + rel.data();
        }
        default:
            return make_abs_path(rel);
        }
    }

    return rel.empty() ? filePaths[path] : filePaths[path] + rel.data();
}
} // namespace utl