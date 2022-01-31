#include "main.hpp"

#include "veracruz.hpp"

#include "localization.hpp"

static Veracruz::Localization SelectedLocale = Veracruz::Localization::EMPTY_LOCALE;

static void doTest() {
    Veracruz::Localization locale({
                                          {"key1", "val1"},
                                          {"key2", "val2"},
                                          {"key3", "requires {1} param {0}"}
                                  });

    // check if we support the language
    // not strictly necessary but why not
    if (Veracruz::LocalizationHandler::GetLanguages().contains(Veracruz::Languages::EN_US)) {
        Veracruz::LocalizationHandler::Register(modInfo, Veracruz::Languages::EN_US, locale);

        // Listen to language select events
        Veracruz::LocalizationHandler::GetLocaleEventHandler(modInfo) += [](
                Veracruz::LangKey const&, std::optional<std::reference_wrapper<Veracruz::Localization const>> selectedLocale) {
            SelectedLocale = selectedLocale.value_or(Veracruz::Localization::EMPTY_LOCALE);
        };

        SelectedLocale = Veracruz::LocalizationHandler::TryGetCurrentLocale(modInfo).value_or(Veracruz::Localization::EMPTY_LOCALE);
    }

    fmtLog(Logging::Level::INFO, "key 1 {}", SelectedLocale.get("key1"));
    fmtLog(Logging::Level::INFO, "key 2 {}", SelectedLocale.get("key2"));

    fmtLog(Logging::Level::INFO, "key 3 {}",
            // format the string at runtime
           fmt::format(fmt::runtime(SelectedLocale.get("key3")), "second", "first")
    );

}

