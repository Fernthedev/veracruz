#pragma once

#include <unordered_map>
#include <string>
#include <utility>
#include <unordered_set>
#include <optional>
#include <vector>

#include "beatsaber-hook/shared/utils/typedefs-wrappers.hpp"

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
         * @note Check Register overload for exceptions thrown
         */
        void Register(ModKey info, LocaleMap const& locale);

        /**
         * Registers a localization for a language specific to a mod
         * Will register the language alongside if not already registered
         *
         * @param info The mod associated with the locale
         * @param langKey The language key
         * @param locale The locale
         * @throws std::runtime_error if mod is already registered
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
         * Returns true if a language is selected
         * @return
         */
        bool IsLanguageSelected() noexcept;

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

        std::optional<std::reference_wrapper<Localization const>> TryGetCurrentLocale(ModKey info);

        /**
         * Retrieves the localization for the selected language and mod
         * @param info
         * @return
         * @throws std::runtime_error if language is not selected or mod is not registered to langauge
         */
        Localization const& GetCurrentLocale(ModKey info);

        /**
         * @brief Get the Locale Event Handler object
         * 
         * Is called on language select
         * 
         * @param info The mod for the callback
         * @return LanguageSelectedEvent& 
         */
        LanguageSelectedEvent& GetLocaleEventHandler(ModKey info);

        /**
         * @brief Get the Basic Locale Event Handler object
         * Is called on language load regardless of how many mods support the locale
         * 
         * @return BasicLanguageSelectedEvent& 
         */
        BasicLanguageSelectedEvent& GetBasicLocaleEventHandler();
    };
}