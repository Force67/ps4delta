
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include <mutex>
#include <shared_mutex>
#include <vector>

#include <utl/file.h>
#include <utl/path.h>
#include <utl/logger/logger.h>

#include "file_sys.h"

namespace fs {
UniquePtr<fileSystem> fileSystem::create() {
    auto fileSys = std::make_unique<fileSystem>();

    // user data (save games etc)
    fileSys->mount("/user", utl::make_app_path(utl::data_dir, "user"));

    // firmware modules
    fileSys->mount("/system/common/lib", utl::make_app_path(utl::data_dir, "lib"));

    // delta cache
    fileSys->mount("/dcache", utl::make_app_path(utl::data_dir, "delta_cache"));
    return fileSys;
}

bool fileSystem::mount(std::string_view vpath, std::string_view path) {
    if (!utl::exists(path))
        utl::make_dir(path);

    // ensure that the _same_ vpath wasn't mounted before
    auto iter = std::find_if(mounts.begin(), mounts.end(),
                             [&vpath](const auto& e) { return e.first == vpath; });

    if (iter == mounts.end()) {
        std::lock_guard lock(mutex);

        mounts.emplace_back(std::make_pair(vpath, path));
        return true;
    }

    return false;
}

std::string fileSystem::get(std::string_view vpath) {
    auto iter = std::find_if(mounts.begin(), mounts.end(),
                            [&vpath](const auto& e) { return vpath.find(e.first) != -1; });

    if (iter == mounts.end()) {
        LOG_WARNING("fs: couldnd find a mount for {}", vpath);
        return "";
    }

    const size_t vlen = iter->first.length();
    return iter->second +
           std::string(vpath.substr(vlen, vpath.length() - vlen));
}
} // namespace fs