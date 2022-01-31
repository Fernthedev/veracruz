#pragma once

#include <concepts>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Veracruz {
    template<typename T>
    concept IsLocalization = requires(T const t, std::string_view key, std::string_view& out) {
        {t.get(key)} -> std::same_as<std::string_view>;
        {t.tryGet(key, out)} -> std::same_as<bool>;
        {t.hasKey(key)} -> std::same_as<bool>;
        {T::EMPTY_LOCALE} -> std::same_as<T>;
    };

    using StringKey = std::string_view;
    using LocaleValue = std::string;

    struct BasicLocalization {
    private:
        BasicLocalization() = default;

    public:
        static BasicLocalization const EMPTY_LOCALE;

        BasicLocalization(std::unordered_map<StringKey, LocaleValue> keys) : keys(std::move(keys)) {}

        template<IsLocalization FallbackLocale>
        BasicLocalization(std::unordered_map<StringKey, LocaleValue> keys, FallbackLocale const& fallbackLocale) : keys(std::move(keys)) {
            for (auto const& [fallbackKey, fallbackVal] : fallbackLocale.keys) {
                this->keys.try_emplace(fallbackKey, fallbackVal);
            }
        }

        [[nodiscard]] std::string_view const get(StringKey const key) const noexcept
        {
            auto it = keys.find(key);

            if (it == keys.end())
                return key;

            return it->second;
        }

        [[nodiscard]] bool tryGet(StringKey const key, std::string_view &out) const noexcept
        {
            auto it = keys.find(key);

            if (it == keys.end()) {
                out = key;
                return false;
            }

            out = it->second;

            return true;
        }

        // TODO: Test
#ifdef FMT_CORE_H_
        template <typename... TArgs>
        [[nodiscard]] std::string get(StringKey const key, TArgs &&...args) const
        {
            std::string ret;
            tryGet(key, ret, std::forward<TArgs>(args)...);

            return ret;
        }

        template <typename... TArgs>
        [[nodiscard]] bool tryGet(StringKey const key, std::string &out, TArgs &&...args) const
        {
            auto ret = tryGet(key, out);

            if (ret) {
                out = fmt::format<TArgs...>(out, std::forward<TArgs>(args)...);
            }

            return ret;
        }
#endif

        [[nodiscard]] constexpr bool hasKey(StringKey const key) const noexcept
        {
            return keys.contains(key);
        }

    protected:
        std::unordered_map<StringKey, LocaleValue> keys;
    };

//
//    namespace detail {
//        template<typename FallbackLocale>
//        requires(IsLocalization<FallbackLocale>)
//        struct SmartLocalization : protected BasicLocalization {
//            SmartLocalization(std::unordered_map<StringKey, LocaleValue> keys, std::optional<FallbackLocale> const &fallbackLocale) :  BasicLocalization(keys), fallbackLocale(fallbackLocale) {}
//
//            SmartLocalization(std::unordered_map<StringKey, LocaleValue> keys, std::optional<FallbackLocale> const &fallbackLocale) :  BasicLocalization(keys), fallbackLocale(fallbackLocale) {}
//        };
//
//        static_assert(IsLocalization<SmartLocalization<BasicLocalization>>);
//        static_assert(IsLocalization<SmartLocalization<SmartLocalization<BasicLocalization>>>);
//    }
//
//    template<typename FallbackLocale>
//    using SmartLocalization = detail::SmartLocalization<FallbackLocale>;
}