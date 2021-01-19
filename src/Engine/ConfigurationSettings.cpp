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
    m_screenMode(Windowed),
    m_aspectRatio(0),
    m_fov(25),
    m_textureFilter(IRenderer::Nearest),
    m_showFps(Off),
    m_screenResolution(High),
    m_soundMode(2),
    m_musicOn(true),
    m_mouseSensitivity(10),
    m_turnSpeed(100),
    m_autoMapMode(TopDownHD),
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

        ConsoleVariable& cvarPathAbyssv113 = GetCVarMutable(CVarIdPathAbyssv113);
        const auto pathAbyssv113Pair = keyValuePairs.find(cvarPathAbyssv113.GetNameInConfigFile());
        if (pathAbyssv113Pair != keyValuePairs.end())
        {
            cvarPathAbyssv113.Deserialize(pathAbyssv113Pair->second);
        }

        ConsoleVariable& cvarPathAbyssv124 = GetCVarMutable(CVarIdPathAbyssv124);
        auto pathAbyssv124Pair = keyValuePairs.find(cvarPathAbyssv124.GetNameInConfigFile());
        if (pathAbyssv124Pair != keyValuePairs.end())
        {
            cvarPathAbyssv124.Deserialize(pathAbyssv124Pair->second);
        }

        ConsoleVariable& cvarPathArmageddonv102 = GetCVarMutable(CVarIdPathArmageddonv102);
        const auto pathArmageddonv102Pair = keyValuePairs.find(cvarPathArmageddonv102.GetNameInConfigFile());
        if (pathArmageddonv102Pair != keyValuePairs.end())
        {
            cvarPathArmageddonv102.Deserialize(pathArmageddonv102Pair->second);
        }

        ConsoleVariable& cvarPathApocalypsev101 = GetCVarMutable(CVarIdPathApocalypsev101);
        const auto pathApocalypsev101Pair = keyValuePairs.find(cvarPathApocalypsev101.GetNameInConfigFile());
        if (pathApocalypsev101Pair != keyValuePairs.end())
        {
            cvarPathApocalypsev101.Deserialize(pathApocalypsev101Pair->second);
        }

        ConsoleVariable& cvarPathCatacomb3Dv122 = GetCVarMutable(CVarIdPathCatacomb3Dv122);
        const auto pathCatacomb3Dv122Pair = keyValuePairs.find(cvarPathCatacomb3Dv122.GetNameInConfigFile());
        if (pathCatacomb3Dv122Pair != keyValuePairs.end())
        {
            cvarPathCatacomb3Dv122.Deserialize(pathCatacomb3Dv122Pair->second);
        }

        auto screenModePair = keyValuePairs.find("screenmode");
        if (screenModePair != keyValuePairs.end())
        {
            m_screenMode =
                (screenModePair->second.compare("windowed") == 0) ? Windowed :
                (screenModePair->second.compare("fullscreen") == 0) ? Fullscreen :
                BorderlessWindowed;
        }

        ConsoleVariable& cvarDepthShading = GetCVarMutable(CVarIdDepthShading);
        const auto depthShadingPair = keyValuePairs.find(cvarDepthShading.GetNameInConfigFile());
        if (depthShadingPair != keyValuePairs.end())
        {
            cvarDepthShading.Deserialize(depthShadingPair->second);
        }
        
        auto showfpsPair = keyValuePairs.find("showfps");
        if (showfpsPair != keyValuePairs.end())
        {
            m_showFps = 
                (showfpsPair->second.compare("true") == 0 || showfpsPair->second.compare("minimal") == 0) ? Minimal :
                (showfpsPair->second.compare("extended") == 0) ? Extended :
                Off;
        }

        ConsoleVariable& cvarVSync = GetCVarMutable(CVarIdVSync);
        const auto vsyncPair = keyValuePairs.find(cvarVSync.GetNameInConfigFile());
        if (vsyncPair != keyValuePairs.end())
        {
            cvarVSync.Deserialize(vsyncPair->second);
        }

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

        auto screenResolutionPair = keyValuePairs.find("screenresolution");
        if (screenResolutionPair != keyValuePairs.end())
        {
            m_screenResolution = (screenResolutionPair->second.compare("original") == 0) ? Original : High;
        }

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

        ConsoleVariable& cvarMouseLook = GetCVarMutable(CVarIdMouseLook);
        auto mouseLookPair = keyValuePairs.find(cvarMouseLook.GetNameInConfigFile());
        if (mouseLookPair != keyValuePairs.end())
        {
            cvarMouseLook.Deserialize(mouseLookPair->second);
        }

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

        ConsoleVariable& cvarAlwaysRun = GetCVarMutable(CVarIdAlwaysRun);
        const auto alwaysRunPair = keyValuePairs.find(cvarAlwaysRun.GetNameInConfigFile());
        if (alwaysRunPair != keyValuePairs.end())
        {
            cvarAlwaysRun.Deserialize(alwaysRunPair->second);
        }

        ConsoleVariable& cvarAutoFire = GetCVarMutable(CVarIdAutoFire);
        const auto autoFirePair = keyValuePairs.find(cvarAutoFire.GetNameInConfigFile());
        if (autoFirePair != keyValuePairs.end())
        {
            cvarAutoFire.Deserialize(autoFirePair->second);
        }

        auto autoMapModePair = keyValuePairs.find("automapmode");
        if (autoMapModePair != keyValuePairs.end())
        {
            m_autoMapMode =
                (autoMapModePair->second.compare("original") == 0) ? OriginalDebug :
                (autoMapModePair->second.compare("isometric") == 0) ? Isometric :
                (autoMapModePair->second.compare("topdown") == 0) ? TopDown :
                TopDownHD;
        }

        ConsoleVariable& cvarManaBar = GetCVarMutable(CVarIdManaBar);
        const auto manaBarPair = keyValuePairs.find(cvarManaBar.GetNameInConfigFile());
        if (manaBarPair != keyValuePairs.end())
        {
            cvarManaBar.Deserialize(manaBarPair->second);
        }

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
        file << "pathabyssv113=" << GetCVar(CVarIdPathAbyssv113).Serialize() << "\n";
        file << "pathabyssv124=" << GetCVar(CVarIdPathAbyssv124).Serialize() << "\n";
        file << "patharmageddonv102=" << GetCVar(CVarIdPathArmageddonv102).Serialize() << "\n";
        file << "pathapocalypsev101=" << GetCVar(CVarIdPathApocalypsev101).Serialize() << "\n";
        file << "pathcatacomb3dv122=" << GetCVar(CVarIdPathCatacomb3Dv122).Serialize() << "\n";
        file << "# Video settings\n";
        const std::string screenModeValue =
            (m_screenMode == Windowed) ? "windowed" :
            (m_screenMode == Fullscreen) ? "fullscreen" :
            "borderlesswindowed";
        file << "screenmode=" << screenModeValue << "\n";
        const std::string screenResolutionValue = (m_screenResolution == Original) ? "original" : "high";
        file << "screenresolution=" << screenResolutionValue << "\n";
        const std::string aspectRatioValue = (m_aspectRatio == 0) ? "Classic" : "FitToScreen";
        file << "aspectratio=" << aspectRatioValue << "\n";
        const std::string depthShadingValue = GetCVar(CVarIdDepthShading).Serialize();
        file << "depthshading=" << depthShadingValue << "\n";
        const std::string showfpsValue =
            (m_showFps == Minimal) ? "minimal" :
            (m_showFps == Extended) ? "extended" :
            "off";
        file << "showfps=" << showfpsValue << "\n";
        const std::string vsyncValue = GetCVar(CVarIdVSync).Serialize();
        file << "vsync=" << vsyncValue << "\n";
        const std::string textureFilterValue = (m_textureFilter == IRenderer::TextureFilterSetting::Nearest) ? "Nearest" : "Linear";
        file << "texturefilter=" << textureFilterValue << "\n";
        std::string fovValue = std::to_string(m_fov);
        file << "fov=" << fovValue << "\n";
        const std::string autoMapModeValue =
            (m_autoMapMode == OriginalDebug) ? "original" :
            (m_autoMapMode == Isometric) ? "isometric" :
            (m_autoMapMode == TopDown) ? "topdown" :
            "topdownhd";
        file << "automapmode=" << autoMapModeValue << "\n";
        file << "# Sound settings\n";
        const std::string modeValue = (m_soundMode == 0) ? "Off" : (m_soundMode == 1) ? "PCSpeaker" : "Adlib";
        file << "soundmode=" << modeValue << "\n";
        const std::string musicValue = (m_musicOn) ? "Adlib" : "Off";
        file << "music=" << musicValue << "\n";
        file << "# Controls settings\n";
        const std::string mlookValue = GetCVar(CVarIdMouseLook).Serialize();
        file << "mlook=" << mlookValue << "\n"; 
        std::string mouseSensitivityValue = std::to_string(m_mouseSensitivity);
        file << "mouseSensitivity=" << mouseSensitivityValue << "\n";
        std::string turnSpeedValue = std::to_string(m_turnSpeed);
        file << "turnSpeed=" << turnSpeedValue << "\n";
        const std::string alwaysRunValue = GetCVar(CVarIdAlwaysRun).Serialize();
        file << "alwaysRun=" << alwaysRunValue << "\n";
        const std::string autoFireValue = GetCVar(CVarIdAutoFire).Serialize();
        file << "autoFire=" << autoFireValue << "\n";
        const std::string manaBarValue = GetCVar(CVarIdManaBar).Serialize();
        file << "manaBar=" << manaBarValue << "\n";
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

ScreenMode ConfigurationSettings::GetScreenMode() const
{
    return m_screenMode;
}

void ConfigurationSettings::SetScreenMode(const ScreenMode screenMode)
{
    m_screenMode = screenMode;
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

ShowFpsMode ConfigurationSettings::GetShowFps() const
{
    return m_showFps;
}

void ConfigurationSettings::SetShowFps(const ShowFpsMode showFpsMode)
{
    m_showFps = showFpsMode;
}

ScreenResolution ConfigurationSettings::GetScreenResolution() const
{
    return m_screenResolution;
}

void ConfigurationSettings::SetScreenResolution(const ScreenResolution screenResolution)
{
    m_screenResolution = screenResolution;
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

AutoMapMode ConfigurationSettings::GetAutoMapMode() const
{
    return m_autoMapMode;
}

void ConfigurationSettings::SetAutoMapMode(const AutoMapMode autoMapMode)
{
    m_autoMapMode = autoMapMode;
}

const ConsoleVariable& ConfigurationSettings::GetCVar(const uint8_t cvarId) const
{
    const ConsoleVariableBool& varBool = (const ConsoleVariableBool&)GetCVarBool(cvarId);
    if (varBool.GetNameInConfigFile() != m_dummyCvarBool.GetNameInConfigFile())
    {
        return (const ConsoleVariable&)varBool;
    }

    return (const ConsoleVariable&)GetCVarString(cvarId);
}

ConsoleVariable& ConfigurationSettings::GetCVarMutable(const uint8_t cvarId)
{
    ConsoleVariableBool& varBool = (ConsoleVariableBool&)GetCVarBoolMutable(cvarId);
    if (varBool.GetNameInConfigFile() != m_dummyCvarBool.GetNameInConfigFile())
    {
        return (ConsoleVariable&)varBool;
    }

    return (ConsoleVariable&)GetCVarStringMutable(cvarId);
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