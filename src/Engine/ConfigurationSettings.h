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

//
// ConfigurationSettings
//
// Interface towards the configuration settings ini file.
//
#pragma once

#include <stdint.h>
#include "IRenderer.h"
#include "ControlsMap.h"
#include "ConsoleVariableBool.h"

static const uint8_t CVarIdDepthShading = 0;
static const uint8_t CVarIdVSync = 1;
static const uint8_t CVarIdMouseLook = 2;
static const uint8_t CVarIdAlwaysRun = 3;
static const uint8_t CVarIdAutoFire = 4;
static const uint8_t CVarIdManaBar = 5;

struct AspectRatioData
{
    float ratio;
    std::string description;
};

const AspectRatioData aspectRatios[2] =
{
    { 4.0f / 3.0f, "Original (4:3)" },
    { 10.0f, "Fit to window" }
};

enum ScreenMode
{
    Windowed,
    Fullscreen,
    BorderlessWindowed
};

enum ScreenResolution
{
    Original,
    High
};

enum ShowFpsMode
{
    Off,
    Minimal,
    Extended
};

enum AutoMapMode
{
    OriginalDebug,
    TopDown,
    TopDownHD,
    Isometric
};

class ConfigurationSettings
{
public:
    ConfigurationSettings();

    void LoadFromFile(const std::string& configurationFile);
    void StoreToFile(const std::string& configurationFile) const;

    const std::string& GetPathAbyssv113() const;
    void SetPathAbyssv113(const std::string path);

    const std::string& GetPathAbyssv124() const;
    void SetPathAbyssv124(const std::string path);

    const std::string& GetPathArmageddonv102() const;
    void SetPathArmageddonv102(const std::string path);

    const std::string& GetPathApocalypsev101() const;
    void SetPathApocalypsev101(const std::string path);

    const std::string& GetPathCatacomb3Dv122() const;
    void SetPathCatacomb3Dv122(const std::string path);

    ScreenMode GetScreenMode() const;
    void SetScreenMode(const ScreenMode screenMode);

    uint8_t GetAspectRatio() const;
    void SetAspectRatio(const uint8_t ratio);

    uint8_t GetFov() const;
    void SetFov(const uint8_t fov);

    IRenderer::TextureFilterSetting GetTextureFilter() const;
    void SetTextureFilter(const IRenderer::TextureFilterSetting filter);

    ShowFpsMode GetShowFps() const;
    void SetShowFps(const ShowFpsMode showFpsMode);

    ScreenResolution GetScreenResolution() const;
    void SetScreenResolution(const ScreenResolution screenResolution);

    ControlsMap& GetControlsMap();
    const ControlsMap& GetConstControlsMap() const;

    uint8_t GetSoundMode() const;
    void SetSoundMode(const uint8_t mode);

    bool GetMusicOn() const;
    void SetMusicOn(const bool on);

    uint8_t GetMouseSensitivity() const;
    void SetMouseSensitivity(const uint8_t sensitivity);

    uint8_t GetTurnSpeed() const;
    void SetTurnSpeed(const uint8_t speed);

    AutoMapMode GetAutoMapMode() const;
    void SetAutoMapMode(const AutoMapMode autoMapMode);

    const ConsoleVariable& GetCVar(const uint8_t cvarId) const;
    ConsoleVariable& GetCVarMutable(const uint8_t cvarId);
    const ConsoleVariableBool& GetCVarBool(const uint8_t cvarId) const;
    ConsoleVariableBool& GetCVarBoolMutable(const uint8_t cvarId);

private:
    ScreenMode m_screenMode;
    uint8_t m_aspectRatio;
    uint8_t m_fov;
    IRenderer::TextureFilterSetting m_textureFilter;
    ShowFpsMode m_showFps;
    ScreenResolution m_screenResolution;
    ControlsMap m_controlsMap;
    uint8_t m_soundMode;
    bool m_musicOn;
    uint8_t m_mouseSensitivity;
    uint8_t m_turnSpeed;
    AutoMapMode m_autoMapMode;

    std::string m_pathAbyssv113;
    std::string m_pathAbyssv124;
    std::string m_pathArmageddonv102;
    std::string m_pathApocalypsev101;
    std::string m_pathCatacomb3Dv122;

    std::map<const uint8_t, ConsoleVariableBool* const> m_cvarsBool;
    ConsoleVariableBool m_dummyCvarBool;
    ConsoleVariableBool m_depthShading;
    ConsoleVariableBool m_vSync;
    ConsoleVariableBool m_mouseLook;
    ConsoleVariableBool m_alwaysRun;
    ConsoleVariableBool m_autoFire;
    ConsoleVariableBool m_manaBar;
};