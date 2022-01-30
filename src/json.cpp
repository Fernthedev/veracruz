#include "main.hpp"

#include "json.hpp"
#include "localization.hpp"

#include <string>
#include <string_view>

using namespace Veracruz;

Localization JSON::parseJSON(rapidjson::Value const& json) {
    std::unordered_map<StringKey, const LocaleValue> keys;

    // TODO: Support array?

    keys.reserve(json.MemberCount());

    for (auto const& keyVal : json.GetObject()) {
        keys.try_emplace(keyVal.name.GetString(), keyVal.value.GetString());
    }

    return Localization(keys);
}