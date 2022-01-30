# Converts the JSON localizations to C++ unordered_map code

# pip install PyGithub
# from github import Github

import json
import os


def indent(i, amount=4):
    return ' ' * (i * amount)

def hpplocalization(localizationName):
    return f"""
#pragma once

#include "veracruz/shared/i18n.hpp"


namespace {modNamespace} {{
    class {localizationName} {{
    public:
        Localization getLocalization();
    }};
}}
"""


def cpplocalization(localizationName: str, dict_json):

    localeMap = "{\n"

    for key in dict_json:
        localeMap += indent(1) + "{" 
        localeMap += str(key) + "," + str(dict_json[key])
        localeMap += " }" + '\n'

    localeMap += "}"

    langArr = localizationName.split("_")
    lang = langArr[0]
    region = langArr[1] if langArr.count() > 1 else ""

    return f"""

#include "veracruz/shared/i18n.hpp"
#include "{localizationHppFolder}/{localizationName}.hpp"

using namespace Veracruz;

Localization {localizationName}::getLocalization() {{
    return localizationLightDataT(
        {localeMap}
    );
}}

InstallLocalization({localizationName}, Lang({lang}, {region}))
"""


def alllocalizationsHpp(localizationListParam):
    header = "#pragma once\n"

    for localization in localizationListParam:
        header += f"#include \"{localizationHppFolder}/{localization}.hpp\"\n"

    return header


# assign directory
directory = 'localizations'
 
modNamespace = "mod"
localizationHppFolder = "./include/localizations"
localizationCppFolder = "./src/localizations"

localizationList = []

os.makedirs(directory, exist_ok=True)

# iterate over files in
# that directory
for filename in os.listdir(directory):
    f = os.path.join(directory, filename)
    # checking if it is a file
    if os.path.isfile(f):
        print(f)

    

for filename in os.listdir(directory):
    file = os.path.join(directory, filename)
    localizationName = filename
    print(f"Doing {localizationName}")

    with open(f"{localizationHppFolder}/{localizationName}.hpp", "w+") as file_converted:
        file_converted.write(hpplocalization(localizationName))

    with open(filename, 'r') as rf:
        json_data = json.loads(rf)
        with open(f"{localizationCppFolder}/{localizationName}.cpp", "w+") as file_converted:
            file_converted.write(cpplocalization(localizationName, json_data))

    localizationList.append(localizationName)

with open(f"{localizationHppFolder}/Alllocalizations.hpp", "w+") as file_converted:
    file_converted.write(alllocalizationsHpp(localizationList))
