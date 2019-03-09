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
        m_mouseXPos(0),
        m_mouseYPos(0),
        m_hasFocus(true)
{

    m_keyPressed.clear();
    m_keyJustPressed.clear();

    for (uint16_t i = 0; i < 6; i++)
    {
        m_buttonPressed[i] = false;
        m_buttonJustPressed[i] = false;
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