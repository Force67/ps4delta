
/*
 * PS4Delta : PS4 emulation and research project
 *
 * Copyright 2019-2020 Force67.
 * For information regarding licensing see LICENSE
 * in the root of the source tree.
 */

#include "config.h"

#include <utl/file.h>
#include <utl/path.h>

#include <logger/logger.h>

namespace config {
bool load() {
    auto path = utl::make_app_path(utl::data_dir, "delta.cfg");

    // no config? write one
    if (!utl::exists(path))
        return save();

    try {
        const auto parse_result = toml::parseFile(path);
        if (!parse_result.valid()) {
            LOG_ERROR("Failed to parse config : {}", parse_result.errorReason);
            return false;
        }

        const toml::Value& doc = parse_result.value;

        // initialize the static opts
        for (optBase* i = optBase::root(); i; i = i->next) {
            if (i->name) {
                const auto* node = doc.find(i->name);
                if (node)
                    i->parse(*node);
                else
                    LOG_WARNING("Unable to initialize opt {}", i->name);
            } else
                LOG_WARNING("Encountered an empty opt");
        }
    } catch (const std::runtime_error& ex) {
        LOG_WARNING("Exception while parsing config: {}", ex.what());
        return false;
    }

    // optBase::root() = nullptr;
    return true;
}

bool save() {
    auto path = utl::make_app_path(utl::data_dir, "delta.cfg");

    try {
        utl::File out(path, utl::fileMode::write);
        if (!out.IsOpen()) return false;

        toml::Value root((toml::Table()));
        for (optBase* i = optBase::root(); i; i = i->next) {
            if (i->name) {
                std::string xname = i->name;

                // convert type to value
                toml::Value val;
                i->dump(val);

                // special behavior for tables
                const auto pos = xname.find_last_of('.');
                if (pos != -1) {
                    auto tabName = xname.substr(0, pos);

                    auto* child = root.findChild(tabName);
                    if (!child) child = root.setChild(tabName, toml::Table());

                    auto shortName = xname.substr(pos + 1, xname.length() - (pos - 1));
                    (*child)[shortName] = val;
                } else
                    root[xname] = val;
            } else
                LOG_WARNING("Encountered an empty opt. Can't serialize it!");
        }

        std::ostringstream os;
        root.write(&os);

        auto str = os.str();
        out.Write(str.c_str(), str.length());
    } catch (const std::runtime_error& ex) {
        LOG_WARNING("Exception while writing config: {}", ex.what());
        return false;
    }

    return true;
}
} // namespace config