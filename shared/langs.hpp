#pragma once

#include <string>

namespace Veracruz {
    struct Lang {
        const std::string langName;
        const std::string region;

        Lang(std::string_view langName, std::string_view region) : langName(langName), region(region) {}

        bool operator ==(Lang const&) const = default;
    };

    namespace Languages {
        // TODO: Add more
        inline const Lang EN_US{"en", "us"};
    }
}

namespace std
{
    template <>
    struct hash<Veracruz::Lang>
    {
        size_t operator()( const Veracruz::Lang& k ) const
        {
            std::hash<std::string_view> hash;
            return hash(k.langName) ^ hash(k.region);
        }
    };

    template <>
    struct hash<Veracruz::Lang const>
    {
        size_t operator()( const Veracruz::Lang& k ) const
        {
            std::hash<std::string_view> hash;
            return hash(k.langName) ^ hash(k.region);
        }
    };
}

