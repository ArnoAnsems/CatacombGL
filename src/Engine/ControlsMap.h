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
// ControlsMap
//
// Binds player actions (shoot, walk, etc.) to keyboard and mouse controls.
//
#pragma once

#include <vector>
#include <map>
#include <string>
#include <SDL_keycode.h>
#include <SDL_gamecontroller.h>

enum ControlAction
{
    None,
    MoveForward,
    MoveBackward,
    TurnLeft,
    TurnRight,
    QuickTurn,
    Strafe,
    StrafeLeft,
    StrafeRight,
    Shoot,
    ShootZappper,
    ShootXterminator,
    UsePotion,
    Run,
    ShowAutoMap,
    MaxControlAction
};

class ControlsMap
{
public:
    ControlsMap();
    ~ControlsMap() = default;
    const std::map<ControlAction, std::string>& GetActionLabels() const;

    bool AssignActionToKey(const ControlAction action, const SDL_Keycode keyCode);
    bool AssignActionToMouseButton(const ControlAction action, const uint8_t buttonCode);
    bool AssignActionToGameControllerButton(const ControlAction action, const SDL_GameControllerButton button);
    bool AssignActionToGameControllerAxis(const ControlAction action, const SDL_GameControllerAxis axis);
    std::string GetKeyStringFromAction(const ControlAction action) const;
    std::vector<SDL_Keycode> GetKeysFromAction(const ControlAction action) const;
    std::vector<uint8_t> GetMouseButtonsFromAction(const ControlAction action) const;
    std::vector< SDL_GameControllerButton> GetGameControllerButtonsFromAction(const ControlAction action) const;
    std::vector< SDL_GameControllerAxis> GetGameControllerAxisFromAction(const ControlAction action) const;
    static std::string GetMouseButtonName(const uint8_t buttonCode);
    static std::string GetGameControllerButtonName(const SDL_GameControllerButton button);
    static const ControlAction StringToAction(const std::string& str);
    void ResetToDefaults();
    void Clear();
    void AssignUnusedKeysToDefaults();

private:
    void AssignDefaultActionToKey(const ControlAction action, const SDL_Keycode keyCode);
    void AssignDefaultActionToMouseButton(const ControlAction action, const uint8_t buttonCode);
    void AssignDefaultActionToGameControllerButton(const ControlAction action, const SDL_GameControllerButton button);
    void AssignDefaultActionToGameControllerAxis(const ControlAction action, const SDL_GameControllerAxis axis);
    ControlAction GetActionFromKey(const SDL_Keycode keyCode) const;
    ControlAction GetActionFromMouseButton(const uint8_t buttonCode) const;
    ControlAction GetActionFromGameControllerButton(const SDL_GameControllerButton button) const;
    ControlAction GetActionFromGameControllerAxis(const SDL_GameControllerAxis axis) const;
    static const std::vector<SDL_Keycode>& GetNotAllowedKeys();

    std::map<SDL_Keycode, ControlAction> m_KeyToActionMap;
    std::map<uint8_t, ControlAction> m_mouseButtonToActionMap;
    std::map<SDL_GameControllerButton, ControlAction> m_GameControllerButtonToActionMap;
    std::map<SDL_GameControllerAxis, ControlAction> m_GameControllerAxisToActionMap;
};
