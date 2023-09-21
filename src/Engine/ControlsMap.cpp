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

#include "ControlsMap.h"
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

ControlsMap::ControlsMap()
{
    ResetToDefaults();
}

ControlsMap::~ControlsMap()
{

}

bool ControlsMap::AssignActionToKey(const ControlAction action, const SDL_Keycode keyCode)
{
    const std::vector<SDL_Keycode>& notAllowedKeys = GetNotAllowedKeys();
    for (size_t i = 0; i < notAllowedKeys.size(); i++)
    {
        if (notAllowedKeys.at(i) == keyCode)
        {
            return false;
        }
    }

    if (action != None)
    {
        const std::vector<SDL_Keycode> otherKeysWithThisAction = GetKeysFromAction(action);
        const std::vector<uint8_t> otherMouseButtonsWithThisAction = GetMouseButtonsFromAction(action);
        if (otherKeysWithThisAction.size() + otherMouseButtonsWithThisAction.size() > 1)
        {
            // More than 2 keys or mouse buttons bound to the same action not allowed.
            // Clear the action from the other keys and buttons.
            for (uint8_t i = 0; i < otherKeysWithThisAction.size(); i++)
            {
                AssignActionToKey(None, otherKeysWithThisAction.at(i));
            }
            for (uint8_t i = 0; i < otherMouseButtonsWithThisAction.size(); i++)
            {
                AssignActionToMouseButton(None, otherMouseButtonsWithThisAction.at(i));
            }
        }
    }

    const auto it = m_KeyToActionMap.find(keyCode);
    if (it == m_KeyToActionMap.end())
    {
        m_KeyToActionMap.insert(std::make_pair(keyCode, action));
    }
    else
    {
        m_KeyToActionMap[keyCode] = action;
    }

	return true;
}

void ControlsMap::AssignDefaultActionToKey(const ControlAction action, const SDL_Keycode keyCode)
{
    if (GetKeysFromAction(action).empty() &&
        GetActionFromKey(keyCode) == None)
    {
        m_KeyToActionMap.insert(std::make_pair(keyCode, action));
    }
}

bool ControlsMap::AssignActionToMouseButton(const ControlAction action, const uint8_t buttonCode)
{
    const auto it = m_mouseButtonToActionMap.find(buttonCode);
    if (it != m_mouseButtonToActionMap.end())
    {
        if (action != None)
        {
            const std::vector<SDL_Keycode> otherKeysWithThisAction = GetKeysFromAction(action);
            const std::vector<uint8_t> otherMouseButtonsWithThisAction = GetMouseButtonsFromAction(action);
            if (otherKeysWithThisAction.size() + otherMouseButtonsWithThisAction.size() > 1)
            {
                // More than 2 keys or mouse buttons bound to the same action not allowed.
                // Clear the action from the other keys and buttons
                for (uint8_t i = 0; i < otherKeysWithThisAction.size(); i++)
                {
                    AssignActionToKey(None, otherKeysWithThisAction.at(i));
                }
                for (uint8_t i = 0; i < otherMouseButtonsWithThisAction.size(); i++)
                {
                    AssignActionToMouseButton(None, otherMouseButtonsWithThisAction.at(i));
                }
            }
        }

        m_mouseButtonToActionMap[buttonCode] = action;
		return true;
    }
	return false;
}

void ControlsMap::AssignDefaultActionToMouseButton(const ControlAction action, const uint8_t buttonCode)
{
    if (GetMouseButtonsFromAction(action).empty() &&
        GetActionFromMouseButton(buttonCode) == None)
    {
        m_mouseButtonToActionMap[buttonCode] = action;
    }
}

const std::map<ControlAction, std::string>& ControlsMap::GetActionLabels() const
{
    return controlActionLabels;
}

std::string ControlsMap::GetKeyStringFromAction(const ControlAction action) const
{
    std::string keyString("");
    for (auto& pair : m_KeyToActionMap)
    {
        if (action == pair.second)
        {
            if (keyString.compare("") != 0)
            {
                keyString += " or ";
            }
            keyString += SDL_GetKeyName(pair.first);
        }
    }

    for (auto& pair : m_mouseButtonToActionMap)
    {
        if (action == pair.second)
        {
            if (keyString.compare("") != 0)
            {
                keyString += " or ";
            }
            keyString += GetMouseButtonName(pair.first);
        }
    }

    return keyString;
}

std::string ControlsMap::GetMouseButtonName(const uint8_t buttonCode)
{
    return (buttonCode == SDL_BUTTON_LEFT) ? "Left Mouse" :
           (buttonCode == SDL_BUTTON_MIDDLE) ? "Middle Mouse" :
           (buttonCode == SDL_BUTTON_RIGHT) ? "Right Mouse" :
           (buttonCode == SDL_BUTTON_X1) ? "X1 Mouse" :
           (buttonCode == SDL_BUTTON_X2) ? "X2 Mouse" :
           "";
}

ControlAction ControlsMap::GetActionFromKey(const SDL_Keycode keyCode) const
{
    const auto it = m_KeyToActionMap.find(keyCode);
    return (it != m_KeyToActionMap.end()) ? it->second : None;
}

ControlAction ControlsMap::GetActionFromMouseButton(const uint8_t buttonCode) const
{
    const auto it = m_mouseButtonToActionMap.find(buttonCode);
    return (it != m_mouseButtonToActionMap.end()) ? it->second : None;
}

std::vector<SDL_Keycode> ControlsMap::GetKeysFromAction(const ControlAction action) const
{
    std::vector<SDL_Keycode> result;
    for (auto& pair : m_KeyToActionMap)
    {
        if (action == pair.second)
        {
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector<uint8_t> ControlsMap::GetMouseButtonsFromAction(const ControlAction action) const
{
    std::vector<uint8_t> result;
    for (auto& pair : m_mouseButtonToActionMap)
    {
        if (action == pair.second)
        {
            result.push_back(pair.first);
        }
    }
    return result;
}

const std::vector<SDL_Keycode>& ControlsMap::GetNotAllowedKeys()
{
    static const std::vector<SDL_Keycode> allNotAllowedKeys =
    {
        SDLK_F1, SDLK_F2, SDLK_F3, SDLK_F4, SDLK_F5, SDLK_F6, SDLK_F7, SDLK_F8, SDLK_F9, SDLK_F10,
        SDLK_1, SDLK_2, SDLK_3, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8,
        SDLK_BACKSPACE, SDLK_ESCAPE, SDLK_BACKQUOTE
    };

    return allNotAllowedKeys;
}

const ControlAction ControlsMap::StringToAction(const std::string& str)
{
    for (const auto pair : controlActionLabels)
    {
        if (str.compare(pair.second) == 0)
        {
            return pair.first;
        }
    }

    return None;
}

void ControlsMap::ResetToDefaults()
{
    Clear();
    AssignUnusedKeysToDefaults();
}

void ControlsMap::Clear()
{
    m_mouseButtonToActionMap.clear();
    m_KeyToActionMap.clear();

    m_mouseButtonToActionMap.insert(std::make_pair(SDL_BUTTON_LEFT, None));
    m_mouseButtonToActionMap.insert(std::make_pair(SDL_BUTTON_MIDDLE, None));
    m_mouseButtonToActionMap.insert(std::make_pair(SDL_BUTTON_RIGHT, None));
    m_mouseButtonToActionMap.insert(std::make_pair(SDL_BUTTON_X1, None));
    m_mouseButtonToActionMap.insert(std::make_pair(SDL_BUTTON_X2, None));
}

void ControlsMap::AssignUnusedKeysToDefaults()
{
    // Define default action to key map
    AssignDefaultActionToKey(MoveForward, SDLK_UP);
    AssignDefaultActionToKey(MoveBackward, SDLK_DOWN);
    AssignDefaultActionToKey(TurnLeft, SDLK_LEFT);
    AssignDefaultActionToKey(TurnRight, SDLK_RIGHT);
    AssignDefaultActionToKey(Strafe, SDLK_RALT);
    AssignDefaultActionToKey(StrafeLeft, SDLK_a);
    AssignDefaultActionToKey(StrafeRight, SDLK_d);
    AssignDefaultActionToKey(MoveForward, SDLK_w);
    AssignDefaultActionToKey(MoveBackward, SDLK_s);
    AssignDefaultActionToKey(QuickTurn, SDLK_v);
    AssignDefaultActionToKey(QuickTurn, SDLK_TAB);
    AssignDefaultActionToKey(Shoot, SDLK_RCTRL);
    AssignDefaultActionToKey(ShootZappper, SDLK_z);
    AssignDefaultActionToKey(ShootXterminator, SDLK_x);
    AssignDefaultActionToKey(UsePotion, SDLK_c);
    AssignDefaultActionToKey(UsePotion, SDLK_SPACE);
    AssignDefaultActionToKey(Run, SDLK_RSHIFT);
    AssignDefaultActionToKey(ShowAutoMap, SDLK_o);

    AssignDefaultActionToMouseButton(Shoot, SDL_BUTTON_LEFT);
    AssignDefaultActionToMouseButton(ShootZappper, SDL_BUTTON_MIDDLE);
    AssignDefaultActionToMouseButton(ShootXterminator, SDL_BUTTON_RIGHT);
}
