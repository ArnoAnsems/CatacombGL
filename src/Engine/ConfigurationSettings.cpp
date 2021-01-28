// Copyright (C) 2018 Arno Ansems
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version. 
// 
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 
// 
// You should have received a copy of the GNU General Public License 
// along with this program.  If not, see http://www.gnu.org/licenses/ 

#include "ConfigurationSettings.h"
#include <fstream>
#include <iostream>
#include <string>
#include "..\..\ThirdParty\SDL\include\SDL_keyboard.h"

ConfigurationSettings::ConfigurationSettings() :
    m_controlsMap(),
    m_dummyCvarBool("Dummy", "Dummy", false),
    m_depthShading("Depth shading", "depthshading", true),
    m_vSync("VSync", "vsync", true),
    m_mouseLook("Mouse Look", "mlook", true),
    m_alwaysRun("Always Run", "alwaysRun", false),
    m_autoFire("Auto Fire", "autoFire", false),
    m_manaBar("Mana Bar", "manaBar", false),
    m_cvarsBool(
        {
            std::make_pair(CVarIdDepthShading, &m_depthShading),
            std::make_pair(CVarIdVSync, &m_vSync),
            std::make_pair(CVarIdMouseLook, &m_mouseLook),
            std::make_pair(CVarIdAlwaysRun, &m_alwaysRun),
            std::make_pair(CVarIdAutoFire, &m_autoFire),
            std::make_pair(CVarIdManaBar, &m_manaBar)
        }),
    m_dummyCvarString("Dummy", "Dummy", ""),
    m_pathAbyssv113("", "pathabyssv113", ""),
    m_pathAbyssv124("", "pathabyssv124", ""),
    m_pathArmageddonv102("", "patharmageddonv102", ""),
    m_pathApocalypsev101("", "pathapocalypsev101", ""),
    m_pathCatacomb3Dv122("", "pathcatacomb3dv122", ""),
    m_cvarsString(
    {
        std::make_pair(CVarIdPathAbyssv113, &m_pathAbyssv113),
        std::make_pair(CVarIdPathAbyssv124, &m_pathAbyssv124),
        std::make_pair(CVarIdPathArmageddonv102, &m_pathArmageddonv102),
        std::make_pair(CVarIdPathApocalypsev101, &m_pathApocalypsev101),
        std::make_pair(CVarIdPathCatacomb3Dv122, &m_pathCatacomb3Dv122)
    }),
    m_dummyCvarEnum("Dummy", "Dummy", { {"","",""} }, 0),
    m_screenMode("Screen Mode", "screenmode",
        {
            {"Windowed", "windowed", ""},
            {"Fullscreen", "fullscreen", ""} ,
            {"Borderless", "borderlesswindowed", ""}
        },
        CVarItemIdScreenModeWindowed),
    m_autoMapMode("Automap", "automapmode",
        {
            {"Original (Debug)", "original", ""},
            {"Top down", "topdown", ""} ,
            {"Top down HD", "topdownhd", ""},
            {"Isometric", "isometric", ""}
        },
        CVarItemIdAutoMapTopDownHD),
    m_showFps("Show frame rate", "showfps",
        {
            {"Off", "off", "false"},
            {"Minimal", "minimal", "true"},
            {"Extended", "extended", ""}
        },
        CVarItemIdShowFpsOff),
    m_screenResolution("Screen Resolution", "screenresolution",
        {
            {"Original", "original", ""},
            {"High", "high", ""}
        },
        CVarItemIdScreenResolutionHigh),
    m_soundMode("Sound mode", "soundmode",
        {
            {"Off", "Off", ""},
            {"PC Speaker", "PCSpeaker", ""},
            {"Adlib", "Adlib", ""}
        },
        CVarItemIdSoundModeAdlib),
    m_musicMode("MUSIC", "music",
        {
            {"NO MUSIC", "Off", ""},
            {"ADLIB/SOUNDBLASTER", "Adlib", ""}
        },
        CVarItemIdMusicModeAdlib),
    m_textureFilter("Texture filtering", "texturefilter",
        {
            {"Nearest", "Nearest", ""},
            {"Linear", "Linear", ""}
        },
        CVarItemIdTextureFilterNearest),
    m_aspectRatio("Aspect ratio", "aspectratio",
        {
            {"Original (4:3)", "Classic", ""},
            {"Fit to window", "FitToScreen", ""}
        },
        CVarItemIdAspectRatioOriginal),
    m_cvarsEnum(
        {
            std::make_pair(CVarIdScreenMode, &m_screenMode),
            std::make_pair(CVarIdAutoMapMode, &m_autoMapMode),
            std::make_pair(CVarIdShowFpsMode, &m_showFps),
            std::make_pair(CVarIdScreenResolution, &m_screenResolution),
            std::make_pair(CVarIdSoundMode, &m_soundMode),
            std::make_pair(CVarIdMusicMode, &m_musicMode),
            std::make_pair(CVarIdTextureFilter, &m_textureFilter),
            std::make_pair(CVarIdAspectRatio, &m_aspectRatio)
        }),
    m_dummyCvarInt("Dummy", "Dummy", 0, 0, 0),
    m_fov("Field Of View (Y)", "fov", 25, 45, 25),
    m_mouseSensitivity("Mouse Sensitiv.", "mouseSensitivity", 1, 20, 10),
    m_turnSpeed("Turn Speed", "turnSpeed", 100, 250, 100),
    m_cvarsInt(
        {
            std::make_pair(CVarIdFov, &m_fov),
            std::make_pair(CVarIdMouseSensitivity, &m_mouseSensitivity),
            std::make_pair(CVarIdTurnSpeed, &m_turnSpeed)
        })
{

}

void ConfigurationSettings::LoadFromFile(const std::string& configurationFile)
{
    std::ifstream file;
    file.open(configurationFile);
    if (file.is_open())
    {
        std::map<std::string, std::string> keyValuePairs;
        std::string line;
        while (std::getline(file, line) && file.eofbit)
        {
            size_t pos = line.rfind('=');
            if (pos != std::string::npos)
            {
                std:: string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                keyValuePairs.insert(std::make_pair(key, value));
            }
        }

        file.close();

        DeserializeCVar(keyValuePairs, CVarIdPathAbyssv113);
        DeserializeCVar(keyValuePairs, CVarIdPathAbyssv124);
        DeserializeCVar(keyValuePairs, CVarIdPathArmageddonv102);
        DeserializeCVar(keyValuePairs, CVarIdPathApocalypsev101);
        DeserializeCVar(keyValuePairs, CVarIdPathCatacomb3Dv122);

        DeserializeCVar(keyValuePairs, CVarIdScreenMode);
        DeserializeCVar(keyValuePairs, CVarIdDepthShading);
        DeserializeCVar(keyValuePairs, CVarIdShowFpsMode);
        DeserializeCVar(keyValuePairs, CVarIdVSync);
        DeserializeCVar(keyValuePairs, CVarIdAspectRatio);
        DeserializeCVar(keyValuePairs, CVarIdTextureFilter);
        DeserializeCVar(keyValuePairs, CVarIdFov);
        DeserializeCVar(keyValuePairs, CVarIdScreenResolution);
        DeserializeCVar(keyValuePairs, CVarIdSoundMode);
        DeserializeCVar(keyValuePairs, CVarIdMusicMode);
        DeserializeCVar(keyValuePairs, CVarIdMouseLook);
        DeserializeCVar(keyValuePairs, CVarIdMouseSensitivity);
        DeserializeCVar(keyValuePairs, CVarIdTurnSpeed);
        DeserializeCVar(keyValuePairs, CVarIdAlwaysRun);
        DeserializeCVar(keyValuePairs, CVarIdAutoFire);
        DeserializeCVar(keyValuePairs, CVarIdAutoMapMode);
        DeserializeCVar(keyValuePairs, CVarIdManaBar);

        for (auto keyPair : keyValuePairs)
        {
            SDL_Keycode keyCode = SDL_GetKeyFromName(keyPair.first.c_str());
            if (keyCode != SDLK_UNKNOWN)
            {
                const ControlAction action = ControlsMap::StringToAction(keyPair.second);
                m_controlsMap.AssignActionToKey(action, keyCode);
            }
        }

        for (uint8_t i = 1; i < 6; i++)
        {
            const std::string buttonName = ControlsMap::GetMouseButtonName(i);
            const auto buttonPair = keyValuePairs.find(buttonName);
            if (buttonPair != keyValuePairs.end())
            {
                const ControlAction action = ControlsMap::StringToAction(buttonPair->second);
                m_controlsMap.AssignActionToMouseButton(action, i);
            }
        }
    }
}

void ConfigurationSettings::StoreToFile(const std::string& configurationFile) const
{
    std::ofstream file;
    file.open(configurationFile);
    if (file.is_open())
    {
        file << "# This file was generated by CatacombGL\n";
        file << "# Path to game data\n";
        SerializeCVar(file, CVarIdPathAbyssv113);
        SerializeCVar(file, CVarIdPathAbyssv124);
        SerializeCVar(file, CVarIdPathArmageddonv102);
        SerializeCVar(file, CVarIdPathApocalypsev101);
        SerializeCVar(file, CVarIdPathCatacomb3Dv122);
        file << "# Video settings\n";
        SerializeCVar(file, CVarIdScreenMode);
        SerializeCVar(file, CVarIdScreenResolution);
        SerializeCVar(file, CVarIdAspectRatio);
        SerializeCVar(file, CVarIdDepthShading);
        SerializeCVar(file, CVarIdShowFpsMode);
        SerializeCVar(file, CVarIdVSync);
        SerializeCVar(file, CVarIdTextureFilter);
        SerializeCVar(file, CVarIdFov);
        SerializeCVar(file, CVarIdAutoMapMode);
        file << "# Sound settings\n";
        SerializeCVar(file, CVarIdSoundMode);
        SerializeCVar(file, CVarIdMusicMode);
        file << "# Controls settings\n";
        SerializeCVar(file, CVarIdMouseLook);
        SerializeCVar(file, CVarIdMouseSensitivity);
        SerializeCVar(file, CVarIdTurnSpeed);
        SerializeCVar(file, CVarIdAlwaysRun);
        SerializeCVar(file, CVarIdAutoFire);
        SerializeCVar(file, CVarIdManaBar);
        file << "# Key bindings\n";
        for (uint8_t i = (uint8_t)MoveForward; i < (uint8_t)MaxControlAction; i++)
	    {
            const std::vector<SDL_Keycode> keysForAction = m_controlsMap.GetKeysFromAction(ControlAction(i));
            const std::string& actionLabel = m_controlsMap.GetActionLabels().at((ControlAction)i);
            for (uint8_t j = 0; j < keysForAction.size(); j++)
            {
                const std::string keyLabel = SDL_GetKeyName(keysForAction.at(j));
                file << keyLabel << "=" << actionLabel << "\n";
            }
            const std::vector<uint8_t>buttonsForAction = m_controlsMap.GetMouseButtonsFromAction(ControlAction(i));
            for (uint8_t j = 0; j < buttonsForAction.size(); j++)
            {
                const std::string buttonLabel = ControlsMap::GetMouseButtonName(buttonsForAction.at(j));
                file << buttonLabel << "=" << actionLabel << "\n";
            }
        }
            
        file.close();
    }
}

ControlsMap& ConfigurationSettings::GetControlsMap()
{
    return m_controlsMap;
}

const ControlsMap& ConfigurationSettings::GetConstControlsMap() const
{
    return m_controlsMap;
}

const ConsoleVariable& ConfigurationSettings::GetCVar(const uint8_t cvarId) const
{
    const ConsoleVariableBool& varBool = (const ConsoleVariableBool&)GetCVarBool(cvarId);
    if (varBool.GetNameInConfigFile() != m_dummyCvarBool.GetNameInConfigFile())
    {
        return (const ConsoleVariable&)varBool;
    }

    const ConsoleVariableString& varString = (const ConsoleVariableString&)GetCVarString(cvarId);
    if (varString.GetNameInConfigFile() != m_dummyCvarString.GetNameInConfigFile())
    {
        return (const ConsoleVariable&)varString;
    }

    const ConsoleVariableEnum& varEnum = (const ConsoleVariableEnum&)GetCVarEnum(cvarId);
    if (varEnum.GetNameInConfigFile() != m_dummyCvarEnum.GetNameInConfigFile())
    {
        return (const ConsoleVariable&)varEnum;
    }

    return (const ConsoleVariable&)GetCVarInt(cvarId);
}

ConsoleVariable& ConfigurationSettings::GetCVarMutable(const uint8_t cvarId)
{
    ConsoleVariableBool& varBool = (ConsoleVariableBool&)GetCVarBoolMutable(cvarId);
    if (varBool.GetNameInConfigFile() != m_dummyCvarBool.GetNameInConfigFile())
    {
        return (ConsoleVariable&)varBool;
    }

    ConsoleVariableString& varString = (ConsoleVariableString&)GetCVarStringMutable(cvarId);
    if (varString.GetNameInConfigFile() != m_dummyCvarString.GetNameInConfigFile())
    {
        return (ConsoleVariable&)varString;
    }

    ConsoleVariableEnum& varEnum = (ConsoleVariableEnum&)GetCVarEnumMutable(cvarId);
    if (varEnum.GetNameInConfigFile() != m_dummyCvarEnum.GetNameInConfigFile())
    {
        return (ConsoleVariable&)varEnum;
    }

    return (ConsoleVariable&)GetCVarIntMutable(cvarId);
}

const ConsoleVariableBool& ConfigurationSettings::GetCVarBool(const uint8_t cvarId) const
{
    const auto it = m_cvarsBool.find(cvarId);
    if (it != m_cvarsBool.end())
    {
        return *it->second;
    }

    return m_dummyCvarBool;
}

ConsoleVariableBool& ConfigurationSettings::GetCVarBoolMutable(const uint8_t cvarId)
{
    auto it = m_cvarsBool.find(cvarId);
    if (it != m_cvarsBool.end())
    {
        return *it->second;
    }

    return m_dummyCvarBool;
}

const ConsoleVariableString& ConfigurationSettings::GetCVarString(const uint8_t cvarId) const
{
    const auto it = m_cvarsString.find(cvarId);
    if (it != m_cvarsString.end())
    {
        return *it->second;
    }

    return m_dummyCvarString;
}

ConsoleVariableString& ConfigurationSettings::GetCVarStringMutable(const uint8_t cvarId)
{
    auto it = m_cvarsString.find(cvarId);
    if (it != m_cvarsString.end())
    {
        return *it->second;
    }

    return m_dummyCvarString;
}

const ConsoleVariableEnum& ConfigurationSettings::GetCVarEnum(const uint8_t cvarId) const
{
    const auto it = m_cvarsEnum.find(cvarId);
    if (it != m_cvarsEnum.end())
    {
        return *it->second;
    }

    return m_dummyCvarEnum;
}

ConsoleVariableEnum& ConfigurationSettings::GetCVarEnumMutable(const uint8_t cvarId)
{
    auto it = m_cvarsEnum.find(cvarId);
    if (it != m_cvarsEnum.end())
    {
        return *it->second;
    }

    return m_dummyCvarEnum;
}

const ConsoleVariableInt& ConfigurationSettings::GetCVarInt(const uint8_t cvarId) const
{
    const auto it = m_cvarsInt.find(cvarId);
    if (it != m_cvarsInt.end())
    {
        return *it->second;
    }

    return m_dummyCvarInt;
}

ConsoleVariableInt& ConfigurationSettings::GetCVarIntMutable(const uint8_t cvarId)
{
    auto it = m_cvarsInt.find(cvarId);
    if (it != m_cvarsInt.end())
    {
        return *it->second;
    }

    return m_dummyCvarInt;
}

void ConfigurationSettings::SerializeCVar(std::ofstream& file, const uint8_t cvarId) const
{
    const ConsoleVariable& cvar = GetCVar(cvarId);
    const std::string& configName = cvar.GetNameInConfigFile();
    const std::string configValue = cvar.Serialize();
    file << configName << "=" << configValue << "\n";
}

void ConfigurationSettings::DeserializeCVar(const std::map<std::string, std::string>& keyValuePairs, const uint8_t cvarId)
{
    ConsoleVariable& cvar = GetCVarMutable(cvarId);
    const auto cvarPair = keyValuePairs.find(cvar.GetNameInConfigFile());
    if (cvarPair != keyValuePairs.end())
    {
        cvar.Deserialize(cvarPair->second);
    }
}