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

#include "PlayerInput.h"

PlayerInput::PlayerInput() :
    m_mouseUpdateTick(0),
    m_mouseXPos(0),
    m_mouseYPos(0),
    m_hasFocus(true)
{
    m_keyPressed.clear();
    m_keyJustPressed.clear();
    m_gameControllerButtonPressed.clear();
    m_gameControllerButtonJustPressed.clear();
    m_gameControllerAxisPressed.clear();
    m_gameControllerAxisJustPressedTowardsNegative.clear();
    m_gameControllerAxisJustPressedTowardsPositive.clear();

    for (uint16_t i = 0; i < 6; i++)
    {
        m_buttonPressed[i] = false;
        m_buttonJustPressed[i] = false;
    }

    for (int i = SDL_CONTROLLER_AXIS_LEFTX; i < SDL_CONTROLLER_AXIS_MAX; i++)
    {
        const auto axis = static_cast<SDL_GameControllerAxis>(i);
        m_gameControllerAxisPressed.insert(std::make_pair(axis, 0u));
        m_gameControllerAxisJustPressedTowardsNegative.insert(std::make_pair(axis, false));
        m_gameControllerAxisJustPressedTowardsPositive.insert(std::make_pair(axis, false));
    }
}

PlayerInput::~PlayerInput()
{

}

void PlayerInput::SetKeyPressed(const SDL_Keycode keyCode, const bool pressed)
{
    const SDL_Keycode sdlCode = keyCode;
    auto it = m_keyPressed.find(sdlCode);
    if (it != m_keyPressed.end())
    {
        if (pressed && !it->second)
        {
            auto it2 = m_keyJustPressed.find(sdlCode);
            if (it2 != m_keyJustPressed.end())
            {
                it2->second = true;
            }
            else
            {
                const std::pair<SDL_Keycode, bool> pair = std::make_pair(sdlCode, true);
                m_keyJustPressed.insert(pair);
            }
        }
        it->second = pressed;
    }
    else
    {
        const std::pair<SDL_Keycode, bool> pair = std::make_pair(sdlCode, pressed);
        m_keyPressed.insert(pair);

        if (pressed)
        {
            auto it2 = m_keyJustPressed.find(sdlCode);
            if (it2 != m_keyJustPressed.end())
            {
                it2->second = true;
            }
            else
            {
                const std::pair<SDL_Keycode, bool> pair = std::make_pair(sdlCode, true);
                m_keyJustPressed.insert(pair);
            }
        }
    }
}

bool PlayerInput::IsKeyJustPressed(const SDL_Keycode keyCode) const
{
    const auto it = m_keyJustPressed.find(keyCode);
    if (it != m_keyJustPressed.end())
    {
        return it->second;
    }

    return false;
}

bool PlayerInput::IsKeyPressed(const SDL_Keycode keyCode) const
{
    const auto it = m_keyPressed.find(keyCode);
    if (it != m_keyPressed.end())
    {
        return it->second;
    }

    return false;
}

bool PlayerInput::IsAnyKeyPressed() const
{
    bool pressed = false;
    for (auto& pair : m_keyJustPressed)
    {
        pressed |= pair.second;
    }

    if (!pressed)
    {
        for (auto& pair : m_gameControllerButtonJustPressed)
        {
            pressed |= pair.second;
        }
    }

    return pressed;
}

void PlayerInput::ClearJustPressed()
{
    for (auto& pair : m_keyJustPressed)
    {
        pair.second = false;
    }

    for (uint8_t i = 0; i < 6; i++)
    {
        m_buttonJustPressed[i] = false;
    }

    for (auto& pair : m_gameControllerButtonJustPressed)
    {
        pair.second = false;
    }

    for (int i = SDL_CONTROLLER_AXIS_LEFTX; i < SDL_CONTROLLER_AXIS_MAX; i++)
    {
        const auto axis = static_cast<SDL_GameControllerAxis>(i);
        m_gameControllerAxisJustPressedTowardsNegative.at(axis) = false;
        m_gameControllerAxisJustPressedTowardsPositive.at(axis) = false;
    }
}

void PlayerInput::SetMouseButtonPressed(const uint8_t buttonCode, const bool pressed)
{
    if (pressed && !m_buttonPressed[buttonCode])
    {
        m_buttonJustPressed[buttonCode] = true;
    }
    m_buttonPressed[buttonCode] = pressed;
}

bool PlayerInput::IsMouseButtonPressed(const uint8_t buttonCode) const
{
    return m_buttonPressed[buttonCode];
}

bool PlayerInput::IsMouseButtonJustPressed(const uint8_t buttonCode) const
{
    return m_buttonJustPressed[buttonCode];
}

uint32_t PlayerInput::GetMouseUpdateTick() const
{
    return m_mouseUpdateTick;
}

void PlayerInput::SetMouseUpdateTick(const uint32_t ticks)
{
    m_mouseUpdateTick = ticks;
}

int32_t PlayerInput::GetMouseXPos() const
{
    return m_mouseXPos;
}

void PlayerInput::SetMouseXPos(const int32_t pos)
{
    m_mouseXPos = pos;
}

int32_t PlayerInput::GetMouseYPos() const
{
    return m_mouseYPos;
}

void PlayerInput::SetMouseYPos(const int32_t pos)
{
    m_mouseYPos = pos;
}

SDL_Keycode PlayerInput::GetFirstKeyPressed() const
{
    SDL_Keycode keyCode = SDLK_UNKNOWN; 
    for (auto& pair : m_keyJustPressed)
    {
        if (pair.second == true)
        {
            keyCode = pair.first;
            break;
        }
    }

    return keyCode;
}

uint8_t PlayerInput::GetFirstMouseButtonPressed() const
{
    uint8_t i = 0;
    while (i < 6 && m_buttonPressed[i] == false)
    {
        i++;
    }

    return (i == 6) ? 0 : i;
}

SDL_GameControllerButton PlayerInput::GetFirstGameControllerButtonPressed() const
{
    SDL_GameControllerButton button = SDL_CONTROLLER_BUTTON_INVALID;
    for (auto& pair : m_gameControllerButtonJustPressed)
    {
        if (pair.second == true)
        {
            button = pair.first;
            break;
        }
    }

    return button;
}

SDL_GameControllerAxis PlayerInput::GetFirstGameControllerAxisPressed() const
{
    SDL_GameControllerAxis axis = SDL_CONTROLLER_AXIS_INVALID;
    for (auto& pair : m_gameControllerAxisJustPressedTowardsNegative)
    {
        if (pair.second == true)
        {
            axis = pair.first;
            break;
        }
    }

    if (axis == SDL_CONTROLLER_AXIS_INVALID)
    {
        for (auto& pair : m_gameControllerAxisJustPressedTowardsPositive)
        {
            if (pair.second == true)
            {
                axis = pair.first;
                break;
            }
        }
    }

    return axis;
}

void PlayerInput::SetHasFocus(const bool focus)
{
    m_hasFocus = focus;
}

bool PlayerInput::HasFocus() const
{
    return m_hasFocus;
}

void PlayerInput::ClearAll()
{
    ClearJustPressed();

    for (auto& pair : m_keyPressed)
    {
        pair.second = false;
    }

    for (uint8_t i = 0; i < 6; i++)
    {
        m_buttonPressed[i] = false;
    }
}

void PlayerInput::SetGameControllerButtonPressed(const SDL_GameControllerButton gameControllerButton, const bool pressed)
{
    auto it = m_gameControllerButtonPressed.find(gameControllerButton);
    if (it != m_gameControllerButtonPressed.end())
    {
        if (pressed && !it->second)
        {
            auto it2 = m_gameControllerButtonJustPressed.find(gameControllerButton);
            if (it2 != m_gameControllerButtonJustPressed.end())
            {
                it2->second = true;
            }
            else
            {
                const std::pair<SDL_GameControllerButton, bool> pair = std::make_pair(gameControllerButton, true);
                m_gameControllerButtonJustPressed.insert(pair);
            }
        }
        it->second = pressed;
    }
    else
    {
        const std::pair<SDL_GameControllerButton, bool> pair = std::make_pair(gameControllerButton, pressed);
        m_gameControllerButtonPressed.insert(pair);

        if (pressed)
        {
            auto it2 = m_gameControllerButtonPressed.find(gameControllerButton);
            if (it2 != m_gameControllerButtonPressed.end())
            {
                it2->second = true;
            }
            else
            {
                const std::pair<SDL_GameControllerButton, bool> pair = std::make_pair(gameControllerButton, true);
                m_gameControllerButtonJustPressed.insert(pair);
            }
        }
    }
}

bool PlayerInput::IsGameControllerButtonJustPressed(const SDL_GameControllerButton gameControllerButton) const
{
    const auto it = m_gameControllerButtonJustPressed.find(gameControllerButton);
    if (it != m_gameControllerButtonJustPressed.end())
    {
        return it->second;
    }

    return false;
}

bool PlayerInput::IsGameControllerButtonPressed(const SDL_GameControllerButton gameControllerButton) const
{
    const auto it = m_gameControllerButtonPressed.find(gameControllerButton);
    if (it != m_gameControllerButtonPressed.end())
    {
        return it->second;
    }

    return false;
}

int16_t PlayerInput::GetGameControllerAxisPressed(const SDL_GameControllerAxis gameControllerAxis) const
{
    return m_gameControllerAxisPressed.at(gameControllerAxis);
}

void PlayerInput::SetGameControllerAxisPressed(const SDL_GameControllerAxis gameControllerAxis, const int16_t value)
{
    const int16_t previousValue = m_gameControllerAxisPressed.at(gameControllerAxis);
    m_gameControllerAxisPressed.at(gameControllerAxis) = value;

    const int16_t minGameControllerAxisDeflection = 10000;
    const bool justPressedTowardsNegative = (previousValue >= -minGameControllerAxisDeflection) && (value < -minGameControllerAxisDeflection);
    const bool justPressedTowardsPositive = (previousValue <= minGameControllerAxisDeflection) && (value > minGameControllerAxisDeflection);
    if (justPressedTowardsNegative)
    {
        m_gameControllerAxisJustPressedTowardsNegative.at(gameControllerAxis) = true;
    }
    if (justPressedTowardsPositive)
    {
        m_gameControllerAxisJustPressedTowardsPositive.at(gameControllerAxis) = true;
    }
}

bool PlayerInput::GetGameControllerAxisJustPressedTowardsNegative(const SDL_GameControllerAxis gameControllerAxis) const
{
    return m_gameControllerAxisJustPressedTowardsNegative.at(gameControllerAxis);
}

bool PlayerInput::GetGameControllerAxisJustPressedTowardsPositive(const SDL_GameControllerAxis gameControllerAxis) const
{
    return m_gameControllerAxisJustPressedTowardsPositive.at(gameControllerAxis);
}

void PlayerInput::SetGameControllerDetected(const bool detected)
{
    m_isGameControllerDetected = detected;
}

bool PlayerInput::IsGameControllerDetected() const
{
    return m_isGameControllerDetected;
}
