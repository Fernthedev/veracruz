#include "main.hpp"

#include "veracruz.hpp"

#include "localization.hpp"

static Veracruz::Localization SelectedLocale = Veracruz::Localization::EMPTY_LOCALE;

static void doInitTest() {
    getLogger().info("Init lang test");

    Veracruz::Localization locale({
                                          {"key1", "val1"},
                                          {"key2", "val2"},
                                          {"key3", "requires {1} param {0}"}
                                  });

    // check if we support the language
    // not strictly necessary but why not

    Veracruz::LocalizationHandler::Register(modInfo, Veracruz::Languages::EN_US, locale);

    // Listen to language select events
    Veracruz::LocalizationHandler::GetLocaleEventHandler(modInfo) += [](
            Veracruz::LangKey const&, std::optional<std::reference_wrapper<Veracruz::Localization const>> selectedLocale) {
        SelectedLocale = selectedLocale.value_or(Veracruz::Localization::EMPTY_LOCALE);
    };

    SelectedLocale = Veracruz::LocalizationHandler::TryGetCurrentLocale(modInfo).value_or(Veracruz::Localization::EMPTY_LOCALE);


    fmtLog(Logging::Level::INFO, "key 1 {}", SelectedLocale.get("key1"));
    fmtLog(Logging::Level::INFO, "key 2 {}", SelectedLocale.get("key2"));

    fmtLog(Logging::Level::INFO, "key 3 {}",
            // format the string at runtime
           fmt::format(fmt::runtime(SelectedLocale.get("key3")), "second", "first")
    );

}

static void selectLangTest() {
    getLogger().info("select lang test");
    // Assert langauge is not selected
    CRASH_UNLESS(!Veracruz::LocalizationHandler::IsLanguageSelected());

    Veracruz::LocalizationHandler::RegisterLanguage(Veracruz::Languages::EN_US);
    Veracruz::LocalizationHandler::SelectLanguage(Veracruz::Languages::EN_US);

    // Assert language is not default
    CRASH_UNLESS(SelectedLocale != Veracruz::Localization::EMPTY_LOCALE);
}

static void doLangTest() {
    getLogger().info("lang english test");
    // Assert langauge is selected
    CRASH_UNLESS(Veracruz::LocalizationHandler::IsLanguageSelected());

    // Assert language is English
    CRASH_UNLESS(Veracruz::LocalizationHandler::GetSelectedLanguage() == Veracruz::Languages::EN_US);

    // Assert language is not default
    CRASH_UNLESS(SelectedLocale != Veracruz::Localization::EMPTY_LOCALE);

    fmtLog(Logging::Level::INFO, "en_us key 1 {}", SelectedLocale.get("key1"));
    fmtLog(Logging::Level::INFO, "en_us key 2 {}", SelectedLocale.get("key2"));

    fmtLog(Logging::Level::INFO, "en_us key 3 {}",
            // format the string at runtime
           fmt::format(fmt::runtime(SelectedLocale.get("key3")), "second", "first")
    );
}


// Called at the early stages of game loading
extern "C" [[maybe_unused]] void setup(ModInfo& info) {
    info.id = MOD_ID;
    info.version = VERSION;
    info = modInfo;


    getLogger().info("Running tests");
    doInitTest();
    selectLangTest();
    doLangTest();
    getLogger().info("Done with tests");

    Veracruz::LocalizationHandler::UnSelectLanguage();

    getLogger().info("Completed setup!");
}

// Called later on in the game loading - a good time to install function hooks
extern "C" [[maybe_unused]] void load() {


}
