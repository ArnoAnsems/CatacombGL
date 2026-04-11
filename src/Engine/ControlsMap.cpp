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

const std::pair<ControlAction, std::string> controlActionLabelArray[MaxControlAction] =
{
    std::make_pair(None, "None"),
    std::make_pair(MoveForward, "MoveForward"),
    std::make_pair(MoveBackward, "MoveBackward"),
    std::make_pair(TurnLeft, "TurnLeft"),
    std::make_pair(TurnRight, "TurnRight"),
    std::make_pair(QuickTurn, "QuickTurn"),
    std::make_pair(Strafe, "Strafe"),
    std::make_pair(StrafeLeft, "StrafeLeft"),
    std::make_pair(StrafeRight, "StrafeRight"),
    std::make_pair(Shoot, "Shoot"),
    std::make_pair(ShootZappper, "ShootZappper"),
    std::make_pair(ShootXterminator, "ShootXterm"),
    std::make_pair(UsePotion, "UsePotion"),
    std::make_pair(Run, "Run"),
    std::make_pair(ShowAutoMap, "Automap")
};

const std::map<ControlAction, std::string> controlActionLabels(controlActionLabelArray, controlActionLabelArray + sizeof(controlActionLabelArray) / sizeof(std::pair<ControlAction, std::string>));

ControlsMap::ControlsMap()
{
    ResetToDefaults();
}

bool ControlsMap::AssignActionToKey(const ControlAction action, const SDL_Keycode keyCode)
{
    if (GetActionFromKey(keyCode) == action)
    {
        // It's a valid key, but already assigned to this action.
        return true;
    }

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
    if (GetActionFromKey(keyCode) == None)
    {
        // There is no action bound to this key yet.
        const std::vector<SDL_Keycode> otherKeysWithThisAction = GetKeysFromAction(action);
        const std::vector<uint8_t> otherMouseButtonsWithThisAction = GetMouseButtonsFromAction(action);
        if (otherKeysWithThisAction.size() + otherMouseButtonsWithThisAction.size() < 2)
        {
            // Less than 2 other keys or mouse buttons are bound to this action, so there is room for one more.
            m_KeyToActionMap.insert(std::make_pair(keyCode, action));
        }
    }
}

bool ControlsMap::AssignActionToMouseButton(const ControlAction action, const uint8_t buttonCode)
{
    const auto it = m_mouseButtonToActionMap.find(buttonCode);
    if (it != m_mouseButtonToActionMap.end())
    {
        if (action != None && it->second != action)
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
    if (GetActionFromMouseButton(buttonCode) == None)
    {
        // There is no action bound to this mouse button yet.
        const std::vector<SDL_Keycode> otherKeysWithThisAction = GetKeysFromAction(action);
        const std::vector<uint8_t> otherMouseButtonsWithThisAction = GetMouseButtonsFromAction(action);
        if (otherKeysWithThisAction.size() + otherMouseButtonsWithThisAction.size() < 2)
        {
            // Less than 2 other keys or mouse buttons are bound to this action, so there is room for one more.
            m_mouseButtonToActionMap[buttonCode] = action;
        }
    }
}

bool ControlsMap::AssignActionToGameControllerButton(const ControlAction action, const SDL_GameControllerButton button)
{
    if (GetActionFromGameControllerButton(button) == action)
    {
        // It's a valid button, but already assigned to this action.
        return true;
    }

    if (button == SDL_CONTROLLER_BUTTON_START)
    {
        // Not allowed to bind
        return false;
    }

    if (action != None)
    {
        const std::vector<SDL_GameControllerButton> otherButtonsWithThisAction = GetGameControllerButtonsFromAction(action);
        const std::vector<SDL_GameControllerAxis> otherAxisWithThisAction = GetGameControllerAxisFromAction(action);
        if (otherButtonsWithThisAction.size() + otherAxisWithThisAction.size() > 1)
        {
            // More than 2 buttons or axis bound to the same action not allowed.
            // Clear the action from the other buttons and axis.
            for (uint8_t i = 0; i < otherButtonsWithThisAction.size(); i++)
            {
                AssignActionToGameControllerButton(None, otherButtonsWithThisAction.at(i));
            }
            for (uint8_t i = 0; i < otherAxisWithThisAction.size(); i++)
            {
                AssignActionToGameControllerAxis(None, otherAxisWithThisAction.at(i));
            }
        }
    }

    const auto it = m_GameControllerButtonToActionMap.find(button);
    if (it == m_GameControllerButtonToActionMap.end())
    {
        m_GameControllerButtonToActionMap.insert(std::make_pair(button, action));
    }
    else
    {
        m_GameControllerButtonToActionMap[button] = action;
    }

    return true;
}

void ControlsMap::AssignDefaultActionToGameControllerButton(const ControlAction action, const SDL_GameControllerButton button)
{
    if (GetActionFromGameControllerButton(button) == None)
    {
        // There is no action bound to this button yet.
        const std::vector<SDL_GameControllerButton> otherButtonsWithThisAction = GetGameControllerButtonsFromAction(action);
        const std::vector<SDL_GameControllerAxis> otherAxisWithThisAction = GetGameControllerAxisFromAction(action);
        if (otherButtonsWithThisAction.size() + otherAxisWithThisAction.size() < 2)
        {
            // Less than 2 other buttons or axis are bound to this action, so there is room for one more.
            m_GameControllerButtonToActionMap.insert(std::make_pair(button, action));
        }
    }
}

bool ControlsMap::AssignActionToGameControllerAxis(const ControlAction action, const SDL_GameControllerAxis axis)
{
    if (GetActionFromGameControllerAxis(axis) == action)
    {
        // It's a valid axis, but already assigned to this action.
        return true;
    }

    if (axis != SDL_CONTROLLER_AXIS_TRIGGERLEFT && axis != SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
    {
        // Not allowed to bind
        return false;
    }

    if (action != None)
    {
        const std::vector<SDL_GameControllerButton> otherButtonsWithThisAction = GetGameControllerButtonsFromAction(action);
        const std::vector<SDL_GameControllerAxis> otherAxisWithThisAction = GetGameControllerAxisFromAction(action);
        if (otherButtonsWithThisAction.size() + otherAxisWithThisAction.size() > 1)
        {
            // More than 2 buttons or axis bound to the same action not allowed.
            // Clear the action from the other buttons and axis.
            for (uint8_t i = 0; i < otherButtonsWithThisAction.size(); i++)
            {
                AssignActionToGameControllerButton(None, otherButtonsWithThisAction.at(i));
            }
            for (uint8_t i = 0; i < otherAxisWithThisAction.size(); i++)
            {
                AssignActionToGameControllerAxis(None, otherAxisWithThisAction.at(i));
            }
        }
    }

    const auto it = m_GameControllerAxisToActionMap.find(axis);
    if (it == m_GameControllerAxisToActionMap.end())
    {
        m_GameControllerAxisToActionMap.insert(std::make_pair(axis, action));
    }
    else
    {
        m_GameControllerAxisToActionMap[axis] = action;
    }

    return true;
}

void ControlsMap::AssignDefaultActionToGameControllerAxis(const ControlAction action, const SDL_GameControllerAxis axis)
{
    if (GetActionFromGameControllerAxis(axis) == None)
    {
        // There is no action bound to this axis yet.
        const std::vector<SDL_GameControllerButton> otherButtonsWithThisAction = GetGameControllerButtonsFromAction(action);
        const std::vector<SDL_GameControllerAxis> otherAxisWithThisAction = GetGameControllerAxisFromAction(action);
        if (otherButtonsWithThisAction.size() + otherAxisWithThisAction.size() < 2)
        {
            // Less than 2 other buttons or axis are bound to this action, so there is room for one more.
            m_GameControllerAxisToActionMap.insert(std::make_pair(axis, action));
        }
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

std::string ControlsMap::GetGameControllerStringFromAction(const ControlAction action) const
{
    std::string gameControllerString("");
    for (auto& pair : m_GameControllerButtonToActionMap)
    {
        if (action == pair.second)
        {
            if (gameControllerString.compare("") != 0)
            {
                gameControllerString += " or ";
            }
            gameControllerString += GetGameControllerButtonName(pair.first);
        }
    }

    for (auto& pair : m_GameControllerAxisToActionMap)
    {
        if (action == pair.second)
        {
            if (gameControllerString.compare("") != 0)
            {
                gameControllerString += " or ";
            }
            gameControllerString += GetGameControllerAxisName(pair.first);
        }
    }

    return gameControllerString;
}

std::string ControlsMap::GetGameControllerButtonName(const SDL_GameControllerButton button)
{
    const std::map<SDL_GameControllerButton, const std::string> buttonToStrMap =
    {
        {SDL_CONTROLLER_BUTTON_A, "Button A"},
        {SDL_CONTROLLER_BUTTON_B, "Button B"},
        {SDL_CONTROLLER_BUTTON_X, "Button X"},
        {SDL_CONTROLLER_BUTTON_Y, "Button Y"},
        {SDL_CONTROLLER_BUTTON_BACK, "Back"},
        {SDL_CONTROLLER_BUTTON_GUIDE, "Guide"},
        {SDL_CONTROLLER_BUTTON_START, "Start"},
        {SDL_CONTROLLER_BUTTON_LEFTSTICK, "Left Stick"},
        {SDL_CONTROLLER_BUTTON_RIGHTSTICK, "Right Stick"},
        {SDL_CONTROLLER_BUTTON_LEFTSHOULDER, "Left Shoulder"},
        {SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, "Right Shoulder"},
        {SDL_CONTROLLER_BUTTON_DPAD_UP, "DPAD Up"},
        {SDL_CONTROLLER_BUTTON_DPAD_DOWN, "DPAD Down"},
        {SDL_CONTROLLER_BUTTON_DPAD_LEFT, "DPAD Left"},
        {SDL_CONTROLLER_BUTTON_DPAD_RIGHT, "DPAD Right"},
        {SDL_CONTROLLER_BUTTON_MISC1, "Misc 1"},
        {SDL_CONTROLLER_BUTTON_PADDLE1, "Paddle 1"},
        {SDL_CONTROLLER_BUTTON_PADDLE2, "Paddle 2"},
        {SDL_CONTROLLER_BUTTON_PADDLE3, "Paddle 3"},
        {SDL_CONTROLLER_BUTTON_PADDLE4, "Paddle 4"},
        {SDL_CONTROLLER_BUTTON_TOUCHPAD, "Touchpad"}
    };

    const auto& it = buttonToStrMap.find(button);
    return (it == buttonToStrMap.end()) ? "" : it->second;
}

std::string ControlsMap::GetGameControllerAxisName(const SDL_GameControllerAxis axis)
{
    const std::map<SDL_GameControllerAxis, const std::string> axisToStrMap =
    {
        {SDL_CONTROLLER_AXIS_TRIGGERLEFT, "Trigger Left"},
        {SDL_CONTROLLER_AXIS_TRIGGERRIGHT, "Trigger Right"}
    };

    const auto& it = axisToStrMap.find(axis);
    return (it == axisToStrMap.end()) ? "" : it->second;
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

ControlAction ControlsMap::GetActionFromGameControllerButton(const SDL_GameControllerButton button) const
{
    const auto it = m_GameControllerButtonToActionMap.find(button);
    return (it != m_GameControllerButtonToActionMap.end()) ? it->second : None;
}

ControlAction ControlsMap::GetActionFromGameControllerAxis(const SDL_GameControllerAxis axis) const
{
    const auto it = m_GameControllerAxisToActionMap.find(axis);
    return (it != m_GameControllerAxisToActionMap.end()) ? it->second : None;
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

std::vector<SDL_GameControllerButton> ControlsMap::GetGameControllerButtonsFromAction(const ControlAction action) const
{
    std::vector<SDL_GameControllerButton> result;
    for (auto& pair : m_GameControllerButtonToActionMap)
    {
        if (action == pair.second)
        {
            result.push_back(pair.first);
        }
    }
    return result;
}

std::vector< SDL_GameControllerAxis> ControlsMap::GetGameControllerAxisFromAction(const ControlAction action) const
{
    std::vector<SDL_GameControllerAxis> result;
    for (auto& pair : m_GameControllerAxisToActionMap)
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
    m_GameControllerButtonToActionMap.clear();
    m_GameControllerAxisToActionMap.clear();

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

    AssignDefaultActionToGameControllerButton(MoveForward, SDL_CONTROLLER_BUTTON_DPAD_UP);
    AssignDefaultActionToGameControllerButton(MoveBackward, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
    AssignDefaultActionToGameControllerButton(TurnLeft, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
    AssignDefaultActionToGameControllerButton(TurnRight, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
    AssignDefaultActionToGameControllerButton(ShootZappper, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
    AssignDefaultActionToGameControllerButton(ShootXterminator, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
    AssignDefaultActionToGameControllerButton(UsePotion, SDL_CONTROLLER_BUTTON_Y);
    AssignDefaultActionToGameControllerButton(ShowAutoMap, SDL_CONTROLLER_BUTTON_X);

    AssignDefaultActionToGameControllerAxis(Shoot, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
}
