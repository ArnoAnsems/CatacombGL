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
#include "..\..\ThirdParty\SDL\include\SDL_keycode.h"

enum ControlAction
{
    None,
    MoveForward,
    MoveBackward,
    TurnLeft,
    TurnRight,
    QuickTurn,
    StrafeLeft,
    StrafeRight,
    Shoot,
    ShootZappper,
    ShootXterminator,
    UsePotion,
    Run,
    MaxControlAction
};

const std::pair<ControlAction, std::string> controlActionLabelArray[MaxControlAction] =
{
    std::make_pair(None, "None"),
    std::make_pair(MoveForward, "MoveForward"),
    std::make_pair(MoveBackward, "MoveBackward"),
    std::make_pair(TurnLeft, "TurnLeft"),
    std::make_pair(TurnRight, "TurnRight"),
    std::make_pair(QuickTurn, "QuickTurn"),
    std::make_pair(StrafeLeft, "StrafeLeft"),
    std::make_pair(StrafeRight, "StrafeRight"),
    std::make_pair(Shoot, "Shoot"),
    std::make_pair(ShootZappper, "ShootZappper"),
    std::make_pair(ShootXterminator, "ShootXterm"),
    std::make_pair(UsePotion, "UsePotion"),
    std::make_pair(Run, "Run")
};

const std::map<ControlAction, std::string> controlActionLabels(controlActionLabelArray, controlActionLabelArray + sizeof(controlActionLabelArray) / sizeof(std::pair<ControlAction, std::string>));

class ControlsMap
{
public:
    ControlsMap();
    ~ControlsMap();
    const std::map<ControlAction, std::string>& GetActionLabels() const;

    bool AssignActionToKey(const ControlAction action, const SDL_Keycode keyCode);
    bool AssignActionToMouseButton(const ControlAction action, const uint8_t buttonCode);

    std::string GetKeyStringFromAction(const ControlAction action) const;

    ControlAction GetActionFromKey(const SDL_Keycode keyCode) const;
    ControlAction GetActionFromMouseButton(const uint8_t buttonCode) const;
    std::vector<SDL_Keycode> GetKeysFromAction(const ControlAction action) const;
    std::vector<uint8_t> GetMouseButtonsFromAction(const ControlAction action) const;
    static std::string GetMouseButtonName(const uint8_t buttonCode);
    static const std::vector<SDL_Keycode>& GetNotAllowedKeys();
    static const ControlAction StringToAction(const std::string& str);

private:
    std::map<SDL_Keycode, ControlAction> m_KeyToActionMap;
    std::map<uint8_t, ControlAction> m_mouseButtonToActionMap;
};