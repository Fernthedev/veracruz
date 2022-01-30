#pragma once

#include "i18n.hpp"

#include "beatsaber-hook/shared/config/rapidjson-utils.hpp"

namespace Veracruz::JSON {

    /**
     * @brief Parses a JSON value which is expected to be {"key" -> "value"}
     * 
     * @param val 
     * @return Localization 
     */
    Localization parseJSON(rapidjson::Value const &val);
}