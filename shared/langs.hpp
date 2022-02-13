#pragma once

#include <string>

namespace Veracruz {
    enum struct LangDirection {
        LEFT_TO_RIGHT,
        RIGHT_TO_LEFT
    };
    struct Lang {
        const std::string langName; // the name of the language in the native language
        const std::string region; // region of the language
        const std::string shortName; // the name of the language shortened
        const LangDirection langDirection;

        Lang(std::string_view langName, std::string_view region, LangDirection langDir = LangDirection::LEFT_TO_RIGHT)
        : langName(langName), region(region), shortName(langName), langDirection(langDir) {}

        Lang(std::string_view langName, std::string_view region, std::string_view shortName, LangDirection langDir = LangDirection::LEFT_TO_RIGHT) :
        langName(langName), region(region), shortName(shortName), langDirection(langDir) {}

        bool operator ==(Lang const&) const = default;
    };

    namespace Languages {
        // TODO: Add more
        inline const Lang EN_US{"English", "us", "en"};
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
            return hash(k.shortName) ^ hash(k.region);
        }
    };

    template <>
    struct hash<Veracruz::Lang const>
    {
        size_t operator()( const Veracruz::Lang& k ) const
        {
            std::hash<std::string_view> hash;
            return hash(k.shortName) ^ hash(k.region);
        }
    };
}

