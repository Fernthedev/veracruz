#include "main.hpp"

#include "i18n.hpp"
#include "langs.hpp"

#include <fmt/core.h>

using namespace Veracruz;

using ModLocaleMap = std::unordered_map<ModKeyPtr, const Localization>;

static std::unordered_map<const LangKey, ModLocaleMap> registeredLocales;
static std::unordered_map<ModKeyPtr, LanguageSelectedEvent> languageLoadedEvents;
static BasicLanguageSelectedEvent basicLanguageLoadedEvent;

static ModLocaleMap const* selectedLanguageMap; // optional?
static std::optional<Lang> selectedLanguage;

void LocalizationHandler::Register(ModKey info,
                                   LocaleMap const &locale) {
    for (auto const& [lang, localization] : locale) {
        Register(info, lang, localization);
    }
}

void LocalizationHandler::Register(ModInfo const &info, LangKey const& lang, Localization const& locale) {
    ModKeyPtr modKey = &info;
    ModLocaleMap& modLocaleMap = registeredLocales[lang];

    fmtLog(Logging::INFO, "Registering mod {} for language {}:{}", info.id, lang.langName, lang.region);

    if (modLocaleMap.contains(modKey)) {
        fmtThrowError("Mod key for id {} is already registered", info.id);
    }

    modLocaleMap.try_emplace(modKey, locale);
}


void LocalizationHandler::Unregister(ModInfo const &info) {
    for (auto const& [lang, map] : registeredLocales) {
        Unregister(info, lang);
    }
}

void LocalizationHandler::Unregister(ModInfo const &info, LangKey const& langKey) {
    fmtLog(Logging::INFO, "Unregistering mod {} for language {}:{}", info.id, langKey.langName, langKey.region);

    registeredLocales[langKey].erase(&info);
}


void LocalizationHandler::SelectLanguage(Lang const &lang) {
    auto it = registeredLocales.find(lang);

    if (it == registeredLocales.end()) {
        fmtThrowError("Language not recognized {0}:{1}", lang.langName, lang.region);
    }

    fmtLog(Logging::INFO, "Language selected {0}:{1}", lang.langName, lang.region);

    selectedLanguageMap = &it->second;
    selectedLanguage.emplace(lang);

    // todo: const
    for (auto& [modInfo, callback] : languageLoadedEvents) {
        if (callback.size() > 0) {
            auto localeIt = selectedLanguageMap->find(modInfo);

            if (localeIt != selectedLanguageMap->end()) {
                callback.invoke(*selectedLanguage, std::cref(localeIt->second));
            } else {
                callback.invoke(*selectedLanguage, std::nullopt);
            }
        }
    }

    basicLanguageLoadedEvent.invoke(*selectedLanguage);
}

LangKey const &LocalizationHandler::GetSelectedLanguage() {
    return selectedLanguage.value();
}

std::optional<std::reference_wrapper<Localization const>>
LocalizationHandler::TryGetLocale(LangKey const &lang, ModKey info) {
    auto it = registeredLocales.find(lang);

    if (it == registeredLocales.end()) {
        return std::nullopt;
    }

    ModLocaleMap const& map = std::ref(it->second);

    auto mapIt = map.find(&info);

    if (mapIt == map.end()) {
        return std::nullopt;
    }

    return std::cref(mapIt->second);
}

Localization const & LocalizationHandler::GetCurrentLocale(ModKey info) {
    if (!selectedLanguageMap) {
        fmtThrowError("Language not recognized");
    }

    auto it = selectedLanguageMap->find(&info);

    if (it == selectedLanguageMap->end()) {
        fmtThrowError("Mod {} is not registered to language {}:{}", info.id, selectedLanguage->langName, selectedLanguage->region);
    }

    return it->second;
}

LanguageSelectedEvent &LocalizationHandler::GetLocaleEventHandler(ModInfo const &info) {
    return languageLoadedEvents[&info];
}

BasicLanguageSelectedEvent& LocalizationHandler::GetBasicLocaleEventHandler() {
    return basicLanguageLoadedEvent;
}
