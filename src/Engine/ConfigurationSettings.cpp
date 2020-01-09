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
    m_textureFilter(IRenderer::Linear),
    m_depthShading(true),
    m_showFps(false),
    m_vsync(true),
    m_soundMode(2),
    m_musicOn(true),
    m_mouseSensitivity(10),
    m_mouseLook(true),
    m_turnSpeed(100),
    m_alwaysRun(false),
    m_autoFire(false),
    m_controlsMap(),
    m_pathAbyssv113(""),
    m_pathAbyssv124(""),
    m_pathArmageddonv102(""),
    m_pathApocalypsev101(""),
    m_pathCatacomb3Dv122("")
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

        auto pathAbyssv113Pair = keyValuePairs.find("pathabyssv113");
        if (pathAbyssv113Pair != keyValuePairs.end())
        {
            m_pathAbyssv113 = pathAbyssv113Pair->second;
        }

        auto pathAbyssv124Pair = keyValuePairs.find("pathabyssv124");
        if (pathAbyssv124Pair != keyValuePairs.end())
        {
            m_pathAbyssv124 = pathAbyssv124Pair->second;
        }

        auto pathArmageddonv102Pair = keyValuePairs.find("patharmageddonv102");
        if (pathArmageddonv102Pair != keyValuePairs.end())
        {
            m_pathArmageddonv102 = pathArmageddonv102Pair->second;
        }

        auto pathApocalypsev101Pair = keyValuePairs.find("pathapocalypsev101");
        if (pathApocalypsev101Pair != keyValuePairs.end())
        {
            m_pathApocalypsev101 = pathApocalypsev101Pair->second;
        }

        auto pathCatacomb3Dv122Pair = keyValuePairs.find("pathcatacomb3dv122");
        if (pathCatacomb3Dv122Pair != keyValuePairs.end())
        {
            m_pathCatacomb3Dv122 = pathCatacomb3Dv122Pair->second;
        }

        auto screenModePair = keyValuePairs.find("screenmode");
        if (screenModePair != keyValuePairs.end())
        {
            m_screenMode =
                (screenModePair->second.compare("windowed") == 0) ? Windowed :
                (screenModePair->second.compare("fullscreen") == 0) ? Fullscreen :
                BorderlessWindowed;
        }

        auto depthShadingPair = keyValuePairs.find("depthshading");
        if (depthShadingPair != keyValuePairs.end())
        {
            m_depthShading = (depthShadingPair->second.compare("true") == 0);
        }
        
        auto showfpsPair = keyValuePairs.find("showfps");
        if (showfpsPair != keyValuePairs.end())
        {
            m_showFps = (showfpsPair->second.compare("true") == 0);
        }

        auto vsyncPair = keyValuePairs.find("vsync");
        if (vsyncPair != keyValuePairs.end())
        {
            m_vsync = (vsyncPair->second.compare("true") == 0);
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

        auto mouseLookPair = keyValuePairs.find("mlook");
        if (mouseLookPair != keyValuePairs.end())
        {
            m_mouseLook = (mouseLookPair->second.compare("Enabled") == 0);
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

        auto alwaysRunPair = keyValuePairs.find("alwaysRun");
        if (alwaysRunPair != keyValuePairs.end())
        {
            m_alwaysRun = (alwaysRunPair->second.compare("Enabled") == 0);
        }

        auto autoFirePair = keyValuePairs.find("autoFire");
        if (autoFirePair != keyValuePairs.end())
        {
            m_autoFire = (autoFirePair->second.compare("Enabled") == 0);
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
        file << "pathabyssv113=" << m_pathAbyssv113 << "\n";
        file << "pathabyssv124=" << m_pathAbyssv124 << "\n";
        file << "patharmageddonv102=" << m_pathArmageddonv102 << "\n";
        file << "pathapocalypsev101=" << m_pathApocalypsev101 << "\n";
        file << "pathcatacomb3dv122=" << m_pathCatacomb3Dv122 << "\n";
        file << "# Video settings\n";
        const std::string screenModeValue =
            (m_screenMode == Windowed) ? "windowed" :
            (m_screenMode == Fullscreen) ? "fullscreen" :
            "borderlesswindowed";
        file << "screenmode=" << screenModeValue << "\n";
        const std::string aspectRatioValue = (m_aspectRatio == 0) ? "Classic" : "FitToScreen";
        file << "aspectratio=" << aspectRatioValue << "\n";
        const std::string depthShadingValue = m_depthShading ? "true" : "false";
        file << "depthshading=" << depthShadingValue << "\n";
        const std::string showfpsValue = m_showFps ? "true" : "false";
        file << "showfps=" << showfpsValue << "\n";
        const std::string vsyncValue = m_vsync ? "true" : "false";
        file << "vsync=" << vsyncValue << "\n";
        const std::string textureFilterValue = (m_textureFilter == IRenderer::TextureFilterSetting::Nearest) ? "Nearest" : "Linear";
        file << "texturefilter=" << textureFilterValue << "\n";
        std::string fovValue = std::to_string(m_fov);
        file << "fov=" << fovValue << "\n";
        file << "# Sound settings\n";
        const std::string modeValue = (m_soundMode == 0) ? "Off" : (m_soundMode == 1) ? "PCSpeaker" : "Adlib";
        file << "soundmode=" << modeValue << "\n";
        const std::string musicValue = (m_musicOn) ? "Adlib" : "Off";
        file << "music=" << musicValue << "\n";
        file << "# Controls settings\n";
        const std::string mlookValue = (m_mouseLook) ? "Enabled" : "Disabled";
        file << "mlook=" << mlookValue << "\n"; 
        std::string mouseSensitivityValue = std::to_string(m_mouseSensitivity);
        file << "mouseSensitivity=" << mouseSensitivityValue << "\n";
        std::string turnSpeedValue = std::to_string(m_turnSpeed);
        file << "turnSpeed=" << turnSpeedValue << "\n";
        const std::string alwaysRunValue = (m_alwaysRun) ? "Enabled" : "Disabled";
        file << "alwaysRun=" << alwaysRunValue << "\n";
        const std::string autoFireValue = (m_autoFire) ? "Enabled" : "Disabled";
        file << "autoFire=" << autoFireValue << "\n";
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

const std::string& ConfigurationSettings::GetPathAbyssv113() const
{
    return m_pathAbyssv113;
}

void ConfigurationSettings::SetPathAbyssv113(const std::string path)
{
    m_pathAbyssv113 = path;
}

const std::string& ConfigurationSettings::GetPathAbyssv124() const
{
    return m_pathAbyssv124;
}

void ConfigurationSettings::SetPathAbyssv124(const std::string path)
{
    m_pathAbyssv124 = path;
}

const std::string& ConfigurationSettings::GetPathArmageddonv102() const
{
    return m_pathArmageddonv102;
}

void ConfigurationSettings::SetPathArmageddonv102(const std::string path)
{
    m_pathArmageddonv102 = path;
}

const std::string& ConfigurationSettings::GetPathApocalypsev101() const
{
    return m_pathApocalypsev101;
}

void ConfigurationSettings::SetPathApocalypsev101(const std::string path)
{
    m_pathApocalypsev101 = path;
}

const std::string& ConfigurationSettings::GetPathCatacomb3Dv122() const
{
    return m_pathCatacomb3Dv122;
}

void ConfigurationSettings::SetPathCatacomb3Dv122(const std::string path)
{
    m_pathCatacomb3Dv122 = path;
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

bool ConfigurationSettings::GetDepthShading() const
{
    return m_depthShading;
}

void ConfigurationSettings::SetDepthShading(const bool enabled)
{
    m_depthShading = enabled;
}

bool ConfigurationSettings::GetShowFps() const
{
    return m_showFps;
}

void ConfigurationSettings::SetShowFps(const bool enabled)
{
    m_showFps = enabled;
}

bool ConfigurationSettings::GetVSync() const
{
    return m_vsync;
}

void ConfigurationSettings::SetVSync(const bool enabled)
{
    m_vsync = enabled;
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

bool ConfigurationSettings::GetMouseLook() const
{
    return m_mouseLook;
}

void ConfigurationSettings::SetMouseLook(const bool enabled)
{
    m_mouseLook = enabled;
}

uint8_t ConfigurationSettings::GetTurnSpeed() const
{
    return m_turnSpeed;
}

void ConfigurationSettings::SetTurnSpeed(const uint8_t speed)
{
    m_turnSpeed = speed;
}

bool ConfigurationSettings::GetAlwaysRun() const
{
    return m_alwaysRun;
}

void ConfigurationSettings::SetAlwaysRun(const bool alwaysRun)
{
    m_alwaysRun = alwaysRun;
}

bool ConfigurationSettings::GetAutoFire() const
{
    return m_autoFire;
}

void ConfigurationSettings::SetAutoFire(const bool autoFire)
{
    m_autoFire = autoFire;
}
