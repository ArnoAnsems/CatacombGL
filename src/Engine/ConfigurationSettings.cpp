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
    m_aspectRatio(0),
    m_fov(25),
    m_textureFilter(IRenderer::Nearest),
    m_soundMode(2),
    m_musicOn(true),
    m_mouseSensitivity(10),
    m_turnSpeed(100),
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
    m_cvarsEnum(
        {
            std::make_pair(CVarIdScreenMode, &m_screenMode),
            std::make_pair(CVarIdAutoMapMode, &m_autoMapMode),
            std::make_pair(CVarIdShowFpsMode, &m_showFps),
            std::make_pair(CVarIdScreenResolution, &m_screenResolution)
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

        auto aspectRatioPair = keyValuePairs.find("aspectratio");
        if (aspectRatioPair != keyValuePairs.end())
        {
            m_aspectRatio = (aspectRatioPair->second.compare("Classic") == 0) ? 0 : 1;
        }

        auto textureFilterPair = keyValuePairs.find("texturefilter");
        if (textureFilterPair != keyValuePairs.end())
        {
            m_textureFilter = (textureFilterPair->second.compare("Linear") == 0) ? IRenderer::Linear : IRenderer::Nearest;
        }

        auto fovPair = keyValuePairs.find("fov");
        if (fovPair != keyValuePairs.end())
        {
            int32_t fov = std::stoi(fovPair->second);
            m_fov = (fov < 25) ? 25 : (fov > 45) ? 45 : fov;
        }

        DeserializeCVar(keyValuePairs, CVarIdScreenResolution);

        auto soundModePair = keyValuePairs.find("soundmode");
        if (soundModePair != keyValuePairs.end())
        {
            m_soundMode = (soundModePair->second.compare("Off") == 0) ? 0 :
                          (soundModePair->second.compare("PCSpeaker") == 0) ? 1 : 2;
        }

        auto musicPair = keyValuePairs.find("music");
        if (musicPair != keyValuePairs.end())
        {
            m_musicOn = (musicPair->second.compare("Adlib") == 0);
        }

        DeserializeCVar(keyValuePairs, CVarIdMouseLook);

        auto mouseSensitivityPair = keyValuePairs.find("mouseSensitivity");
        if (mouseSensitivityPair != keyValuePairs.end())
        {
            int32_t sensitivity = std::stoi(mouseSensitivityPair->second);
            m_mouseSensitivity = (sensitivity < 1) ? 1 : (sensitivity > 20) ? 20: sensitivity;
        }

        auto turnSpeedPair = keyValuePairs.find("turnSpeed");
        if (turnSpeedPair != keyValuePairs.end())
        {
            int32_t turnSpeed = std::stoi(turnSpeedPair->second);
            m_turnSpeed = (turnSpeed < 100) ? 100 : (turnSpeed > 250) ? 250 : (uint8_t)turnSpeed;
        }

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
        const std::string aspectRatioValue = (m_aspectRatio == 0) ? "Classic" : "FitToScreen";
        file << "aspectratio=" << aspectRatioValue << "\n";
        SerializeCVar(file, CVarIdDepthShading);
        SerializeCVar(file, CVarIdShowFpsMode);
        SerializeCVar(file, CVarIdVSync);
        const std::string textureFilterValue = (m_textureFilter == IRenderer::TextureFilterSetting::Nearest) ? "Nearest" : "Linear";
        file << "texturefilter=" << textureFilterValue << "\n";
        std::string fovValue = std::to_string(m_fov);
        file << "fov=" << fovValue << "\n";
        SerializeCVar(file, CVarIdAutoMapMode);
        file << "# Sound settings\n";
        const std::string modeValue = (m_soundMode == 0) ? "Off" : (m_soundMode == 1) ? "PCSpeaker" : "Adlib";
        file << "soundmode=" << modeValue << "\n";
        const std::string musicValue = (m_musicOn) ? "Adlib" : "Off";
        file << "music=" << musicValue << "\n";
        file << "# Controls settings\n";
        SerializeCVar(file, CVarIdMouseLook);
        std::string mouseSensitivityValue = std::to_string(m_mouseSensitivity);
        file << "mouseSensitivity=" << mouseSensitivityValue << "\n";
        std::string turnSpeedValue = std::to_string(m_turnSpeed);
        file << "turnSpeed=" << turnSpeedValue << "\n";
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

uint8_t ConfigurationSettings::GetAspectRatio() const
{
    return m_aspectRatio;
}

void ConfigurationSettings::SetAspectRatio(const uint8_t ratio)
{
    m_aspectRatio = ratio;
}

uint8_t ConfigurationSettings::GetFov() const
{
    return m_fov;
}

void ConfigurationSettings::SetFov(const uint8_t fov)
{
    m_fov = fov;
}

IRenderer::TextureFilterSetting ConfigurationSettings::GetTextureFilter() const
{
    return m_textureFilter;
}

void ConfigurationSettings::SetTextureFilter(const IRenderer::TextureFilterSetting filter)
{
    m_textureFilter = filter;
}

ControlsMap& ConfigurationSettings::GetControlsMap()
{
    return m_controlsMap;
}

const ControlsMap& ConfigurationSettings::GetConstControlsMap() const
{
    return m_controlsMap;
}

uint8_t ConfigurationSettings::GetSoundMode() const
{
    return m_soundMode;
}

void ConfigurationSettings::SetSoundMode(const uint8_t mode)
{
    m_soundMode = mode;
}

bool ConfigurationSettings::GetMusicOn() const
{
    return m_musicOn;
}

void ConfigurationSettings::SetMusicOn(const bool on)
{
    m_musicOn = on;
}

uint8_t ConfigurationSettings::GetMouseSensitivity() const
{
    return m_mouseSensitivity;
}

void ConfigurationSettings::SetMouseSensitivity(const uint8_t sensitivity)
{
    m_mouseSensitivity = sensitivity;
}

uint8_t ConfigurationSettings::GetTurnSpeed() const
{
    return m_turnSpeed;
}

void ConfigurationSettings::SetTurnSpeed(const uint8_t speed)
{
    m_turnSpeed = speed;
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

    return (const ConsoleVariable&)GetCVarEnum(cvarId);
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

    return (ConsoleVariable&)GetCVarEnumMutable(cvarId);
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