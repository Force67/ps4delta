
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
    auto path = utl::make_app_path(utl::data_dir, "delta.yml");

    // no config? write one
    if (!utl::exists(path))
        return save();

    try {
        auto doc = YAML::LoadFile(path);

        // initialize the static opts
        for (optBase* i = optBase::root(); i; i = i->next) {
            if (i->name) {
                std::string xstr = i->name;

                // 'namespace'
                const auto pos = xstr.find_first_of('.');
                if (pos != -1) {
                    auto family = xstr.substr(0, pos);
                    auto familyNode = doc[family];
                    if (familyNode) {
                        auto child = xstr.substr(pos + 1, xstr.length() - (pos - 1));
                        auto childNode = familyNode[child];
                        if (childNode)
                            i->parseNode(childNode);
                        else
                            LOG_WARNING("Unable to initialize family opt {}", i->name);
                    } else
                        LOG_WARNING("Unable to find family node for {}", family);
                } else {
                    auto node = doc[xstr];
                    if (node)
                        i->parseNode(node);
                    else 
                        LOG_WARNING("Unable to initialize global opt {}", i->name);
                }
            } else
                LOG_WARNING("Encountered an empty opt");
        }

    } catch (std::exception& ex) {
        LOG_WARNING("Exception while parsing config: {}", ex.what());
    }

    // optBase::root() = nullptr;
    return true;
}

bool save() {
    auto path = utl::make_app_path(utl::data_dir, "delta.yml");

    try {
        utl::File out(path, utl::fileMode::write);
        if (!out.IsOpen())
            return false;

        YAML::Node doc;
        for (optBase* i = optBase::root(); i; i = i->next) {
            if (i->name) {
                std::string xstr = i->name;

                // convert type to value
                YAML::Node val;
                i->dumpNode(val);

                // is it a 'namespace' member
                const auto pos = xstr.find_first_of('.');
                if (pos != -1) {
                    auto family = xstr.substr(0, pos);
                    auto child = xstr.substr(pos + 1, xstr.length() - (pos - 1));
                    doc[family][child] = val;
                } else
                    doc[xstr] = val;
            } else
                LOG_WARNING("Encountered an empty opt. Can't serialize it!");
        }

        YAML::Emitter emit;
        emit << doc;

        out.Write(emit.c_str(), emit.size());
    } catch (const std::runtime_error& ex) {
        LOG_WARNING("Exception while writing config: {}", ex.what());
        return false;
    }

    return true;
}
} // namespace config