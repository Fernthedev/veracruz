#pragma once

#include "modloader/shared/modloader.hpp"

#include <utility>
#include <string>
#include <string_view>

namespace std {
    template<>
    struct hash<ModInfo> {
        size_t operator()(const ModInfo &k) const {
            std::hash<std::string_view> hash;
            return hash(k.id) ^ hash(k.version);
        }
    };

    template<>
    struct hash<ModInfo const> {
        size_t operator()(const ModInfo &k) const {
            std::hash<std::string_view> hash;
            return hash(k.id) ^ hash(k.version);
        }
    };
}

static constexpr bool operator ==(ModInfo const& a, ModInfo const& b) {
    return a.id == b.id && a.version == b.version;
}