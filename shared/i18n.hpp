#pragma once

#include <unordered_map>
#include <string>
#include <utility>

#include "modloader/shared/modloader.hpp"

#include "localization.hpp"

namespace Veracruz {
    struct Lang;

    using ModKey = ModInfo const&;
    using ModKeyPtr = ModInfo const*; // for hash map key
    using LangKey = Lang;

    using Localization = BasicLocalization;

    using LocaleMap = std::unordered_map<LangKey const, const Localization>;
    using LanguageSelectedEvent = UnorderedEventCallback<LangKey const&, std::optional<std::reference_wrapper<Localization const>>>;
    using BasicLanguageSelectedEvent = UnorderedEventCallback<LangKey const&>;

    namespace LocalizationHandler {
        void Register(ModKey info, LocaleMap const& locale);
        void Register(ModKey info, const LangKey& langKey, Localization const& locale);

        void Unregister(ModKey info);
        void Unregister(ModKey info, LangKey const& langKey);

        void SelectLanguage(LangKey const& lang);
        LangKey const& GetSelectedLanguage();
        // TODO: Get available languages and register languages

        std::optional<std::reference_wrapper<Localization const>> TryGetLocale(LangKey const& lang, ModKey info);
        Localization const& GetCurrentLocale(ModKey info);
        LanguageSelectedEvent& GetLocaleEventHandler(ModKey info);

        BasicLanguageSelectedEvent& GetBasicLocaleEventHandler();
    };
}