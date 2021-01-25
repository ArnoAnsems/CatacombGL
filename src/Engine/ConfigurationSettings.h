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
#include "ControlsMap.h"
#include "ConsoleVariableBool.h"
#include "ConsoleVariableString.h"
#include "ConsoleVariableEnum.h"

static const uint8_t CVarIdDepthShading = 0;
static const uint8_t CVarIdVSync = 1;
static const uint8_t CVarIdMouseLook = 2;
static const uint8_t CVarIdAlwaysRun = 3;
static const uint8_t CVarIdAutoFire = 4;
static const uint8_t CVarIdManaBar = 5;
static const uint8_t CVarIdPathAbyssv113 = 10;
static const uint8_t CVarIdPathAbyssv124 = 11;
static const uint8_t CVarIdPathArmageddonv102 = 12;
static const uint8_t CVarIdPathApocalypsev101 = 13;
static const uint8_t CVarIdPathCatacomb3Dv122 = 14;
static const uint8_t CVarIdScreenMode = 20;
static const uint8_t CVarIdAutoMapMode = 21;
static const uint8_t CVarIdShowFpsMode = 22;
static const uint8_t CVarIdScreenResolution = 23;
static const uint8_t CVarIdSoundMode = 24;
static const uint8_t CVarIdMusicMode = 25;
static const uint8_t CVarIdTextureFilter = 26;
static const uint8_t CVarIdAspectRatio = 27;

static const uint8_t CVarItemIdScreenModeWindowed = 0;
static const uint8_t CVarItemIdScreenModeFullscreen = 1;
static const uint8_t CVarItemIdScreenModeBorderlessWindowed = 2;

static const uint8_t CVarItemIdAutoMapOriginal = 0;
static const uint8_t CVarItemIdAutoMapTopDown = 1;
static const uint8_t CVarItemIdAutoMapTopDownHD = 2;
static const uint8_t CVarItemIdAutoMapIsometric = 3;

static const uint8_t CVarItemIdShowFpsOff = 0;
static const uint8_t CVarItemIdShowFpsMinimal = 1;
static const uint8_t CVarItemIdShowFpsExtended = 2;

static const uint8_t CVarItemIdScreenResolutionOriginal = 0;
static const uint8_t CVarItemIdScreenResolutionHigh = 1;

static const uint8_t CVarItemIdSoundModeOff = 0;
static const uint8_t CVarItemIdSoundModePCSpeaker = 1;
static const uint8_t CVarItemIdSoundModeAdlib = 2;

static const uint8_t CVarItemIdMusicModeOff = 0;
static const uint8_t CVarItemIdMusicModeAdlib = 1;

static const uint8_t CVarItemIdTextureFilterNearest = 0;
static const uint8_t CVarItemIdTextureFilterLinear = 1;

static const uint8_t CVarItemIdAspectRatioOriginal = 0;
static const uint8_t CVarItemIdAspectRatioFitToWindow = 1;

class ConfigurationSettings
{
public:
    ConfigurationSettings();

    void LoadFromFile(const std::string& configurationFile);
    void StoreToFile(const std::string& configurationFile) const;

    uint8_t GetFov() const;
    void SetFov(const uint8_t fov);

    ControlsMap& GetControlsMap();
    const ControlsMap& GetConstControlsMap() const;

    uint8_t GetMouseSensitivity() const;
    void SetMouseSensitivity(const uint8_t sensitivity);

    uint8_t GetTurnSpeed() const;
    void SetTurnSpeed(const uint8_t speed);

    const ConsoleVariable& GetCVar(const uint8_t cvarId) const;
    ConsoleVariable& GetCVarMutable(const uint8_t cvarId);
    const ConsoleVariableBool& GetCVarBool(const uint8_t cvarId) const;
    ConsoleVariableBool& GetCVarBoolMutable(const uint8_t cvarId);
    const ConsoleVariableString& GetCVarString(const uint8_t cvarId) const;
    ConsoleVariableString& GetCVarStringMutable(const uint8_t cvarId);
    const ConsoleVariableEnum& GetCVarEnum(const uint8_t cvarId) const;
    ConsoleVariableEnum& GetCVarEnumMutable(const uint8_t cvarId);

private:
    void SerializeCVar(std::ofstream& file, const uint8_t cvarId) const;
    void DeserializeCVar(const std::map<std::string, std::string>& keyValuePairs, const uint8_t cvarId);

    uint8_t m_fov;
    ControlsMap m_controlsMap;
    uint8_t m_mouseSensitivity;
    uint8_t m_turnSpeed;

    std::map<const uint8_t, ConsoleVariableBool* const> m_cvarsBool;
    std::map<const uint8_t, ConsoleVariableString* const> m_cvarsString;
    std::map<const uint8_t, ConsoleVariableEnum* const> m_cvarsEnum;

    ConsoleVariableString m_dummyCvarString;
    ConsoleVariableString m_pathAbyssv113;
    ConsoleVariableString m_pathAbyssv124;
    ConsoleVariableString m_pathArmageddonv102;
    ConsoleVariableString m_pathApocalypsev101;
    ConsoleVariableString m_pathCatacomb3Dv122;

    ConsoleVariableBool m_dummyCvarBool;
    ConsoleVariableBool m_depthShading;
    ConsoleVariableBool m_vSync;
    ConsoleVariableBool m_mouseLook;
    ConsoleVariableBool m_alwaysRun;
    ConsoleVariableBool m_autoFire;
    ConsoleVariableBool m_manaBar;

    ConsoleVariableEnum m_dummyCvarEnum;
    ConsoleVariableEnum m_screenMode;
    ConsoleVariableEnum m_autoMapMode;
    ConsoleVariableEnum m_showFps;
    ConsoleVariableEnum m_screenResolution;
    ConsoleVariableEnum m_soundMode;
    ConsoleVariableEnum m_musicMode;
    ConsoleVariableEnum m_textureFilter;
    ConsoleVariableEnum m_aspectRatio;
};