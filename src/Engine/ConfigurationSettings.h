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

    ScreenMode GetScreenMode() const;
    void SetScreenMode(const ScreenMode screenMode);

    uint8_t GetAspectRatio() const;
    void SetAspectRatio(const uint8_t ratio);

    uint8_t GetFov() const;
    void SetFov(const uint8_t fov);

    IRenderer::TextureFilterSetting GetTextureFilter() const;
    void SetTextureFilter(const IRenderer::TextureFilterSetting filter);

    bool GetDepthShading() const;
    void SetDepthShading(const bool enabled);

    bool GetShowFps() const;
    void SetShowFps(const bool enabled);

    bool GetVSync() const;
    void SetVSync(const bool enabled);

    ControlsMap& GetControlsMap();
    const ControlsMap& GetConstControlsMap() const;

    uint8_t GetSoundMode() const;
    void SetSoundMode(const uint8_t mode);

    uint8_t GetMouseSensitivity() const;
    void SetMouseSensitivity(const uint8_t sensitivity);

    bool GetMouseLook() const;
    void SetMouseLook(const bool enabled);

    uint8_t GetTurnSpeed() const;
    void SetTurnSpeed(const uint8_t speed);

    bool GetAlwaysRun() const;
    void SetAlwaysRun(const bool alwaysRun);

private:
    ScreenMode m_screenMode;
    uint8_t m_aspectRatio;
    uint8_t m_fov;
    IRenderer::TextureFilterSetting m_textureFilter;
    bool m_depthShading;
    bool m_showFps;
    bool m_vsync;
    ControlsMap m_controlsMap;
    uint8_t m_soundMode;
    uint8_t m_mouseSensitivity;
    bool m_mouseLook;
    uint8_t m_turnSpeed;
    bool m_alwaysRun;

    std::string m_pathAbyssv113;
    std::string m_pathAbyssv124;
    std::string m_pathArmageddonv102;
};