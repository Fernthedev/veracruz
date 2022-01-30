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
        /**
         * Registers a list of locale for a mod
         * Will register the language alongside if not already registered
         *
         * @param info the mod associated with the locales
         * @param locale the locales associated with their language
         */
        void Register(ModKey info, LocaleMap const& locale);

        /**
         * Registers a localization for a language specific to a mod
         * Will register the language alongside if not already registered
         *
         * @param info The mod associated with the locale
         * @param langKey The language key
         * @param locale The locale
         */
        void Register(ModKey info, LangKey const& langKey, Localization const& locale);

        void Unregister(ModKey info);
        void Unregister(ModKey info, LangKey const& langKey);

        /**
         * Selects the langauge which other mods will use
         * @param lang language
         * @throws If the language is not registered, this will throw an exception
         */
        void SelectLanguage(LangKey const& lang);

        /**
         * Registers the language
         * @param lang
         */
        void RegisterLanguage(LangKey const& lang);

        /**
         * Gets the selected language
         * @return selected language
         * @note If a language has not been selected at this point in time, this will throw an exception
         */
        LangKey const& GetSelectedLanguage();

        /**
         * Gets a set of all registered languages
         * @return
         */
        std::unordered_set<LangKey> GetLanguages();

        /**
         * Attempts to find a suitable language
         * Finds the first language that is registered and contains the modInfo
         * @param languages
         * @return First language that is registered and contains the modInfo
         */
        std::optional<LangKey> FindSuitableFallback(ModInfo const& info, std::vector<LangKey> const& supportedLanguages) noexcept;

        std::optional<std::reference_wrapper<Localization const>> TryGetLocale(LangKey const& lang, ModKey info);
        Localization const& GetCurrentLocale(ModKey info);
        LanguageSelectedEvent& GetLocaleEventHandler(ModKey info);

        BasicLanguageSelectedEvent& GetBasicLocaleEventHandler();
    };
}