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
#include "..\..\ThirdParty\SDL\include\SDL_keyboard.h"
#include "..\..\ThirdParty\SDL\include\SDL_mouse.h"

ControlsMap::ControlsMap()
{
    m_mouseButtonToActionMap.insert(std::make_pair(SDL_BUTTON_LEFT, None));
    m_mouseButtonToActionMap.insert(std::make_pair(SDL_BUTTON_MIDDLE, None));
    m_mouseButtonToActionMap.insert(std::make_pair(SDL_BUTTON_RIGHT, None));
    m_mouseButtonToActionMap.insert(std::make_pair(SDL_BUTTON_X1, None));
    m_mouseButtonToActionMap.insert(std::make_pair(SDL_BUTTON_X2, None));

    // Define default action to key map
    AssignActionToKey(MoveForward, SDLK_UP);
    AssignActionToKey(MoveBackward, SDLK_DOWN);
    AssignActionToKey(TurnLeft, SDLK_LEFT);
    AssignActionToKey(TurnRight, SDLK_RIGHT);
    AssignActionToKey(Strafe, SDLK_RALT);
    AssignActionToKey(StrafeLeft, SDLK_a);
    AssignActionToKey(StrafeRight, SDLK_d);
    AssignActionToKey(MoveForward, SDLK_w);
    AssignActionToKey(MoveBackward, SDLK_s);
    AssignActionToKey(QuickTurn, SDLK_v);
    AssignActionToKey(QuickTurn, SDLK_TAB);
    AssignActionToKey(Shoot, SDLK_RCTRL);
    AssignActionToKey(ShootZappper, SDLK_z);
    AssignActionToKey(ShootXterminator, SDLK_x);
    AssignActionToKey(UsePotion, SDLK_c);
    AssignActionToKey(UsePotion, SDLK_SPACE);
    // Since the original game did not have a run action, by default no key is assigned to the run action.
    AssignActionToKey(ShowAutoMap, SDLK_o);

    AssignActionToMouseButton(Shoot, SDL_BUTTON_LEFT);
    AssignActionToMouseButton(ShootZappper, SDL_BUTTON_MIDDLE);
    AssignActionToMouseButton(ShootXterminator, SDL_BUTTON_RIGHT);
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