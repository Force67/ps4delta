#pragma once

/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <base.h>

#include <shared_mutex>
#include <string>
#include <string_view>
#include <vector>

namespace utl {
class File;
}

namespace fs {
class fileSystem {
public:
    static UniquePtr<fileSystem> create();

    bool mount(std::string_view vpath, std::string_view path);
    std::string get(std::string_view vpath);

private:
    std::shared_mutex mutex;

    // toplevel root mounts
    //std::vector<vfs_directory> root_mounts;
    std::vector<std::pair<std::string, std::string>> mounts;
};
} // namespace vfs