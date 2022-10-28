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
// PlayerInput
//
// Holds the actual state of the keyboard and mouse input.
//
#pragma once

#include <SDL_keycode.h>
#include <SDL_stdinc.h>
#include <cstdint>
#include <map>

class PlayerInput
{
public:
    PlayerInput();
    ~PlayerInput();
    void SetKeyPressed(const SDL_Keycode keyCode, const bool pressed);
    bool IsKeyJustPressed(const SDL_Keycode keyCode) const;
    bool IsKeyPressed(const SDL_Keycode keyCode) const;
    bool IsAnyKeyPressed() const;
    void ClearJustPressed();
    void SetMouseButtonPressed(const uint8_t buttonCode, const bool pressed);
    bool IsMouseButtonPressed(const uint8_t buttonCode) const;
    bool IsMouseButtonJustPressed(const uint8_t buttonCode) const;
    uint32_t GetMouseUpdateTick() const;
    void SetMouseUpdateTick(const uint32_t ticks);
    int32_t GetMouseXPos() const;
    void SetMouseXPos(const int32_t pos);
    int32_t GetMouseYPos() const;
    void SetMouseYPos(const int32_t pos);
    SDL_Keycode GetFirstKeyPressed() const;
    uint8_t GetFirstMouseButtonPressed() const;
    void SetHasFocus(const bool focus);
    bool HasFocus() const;
    void ClearAll();

private:
    std::map<SDL_Keycode, bool> m_keyPressed;
    std::map<SDL_Keycode, bool> m_keyJustPressed;
    bool m_buttonPressed[6];
    bool m_buttonJustPressed[6];
    uint32_t m_mouseUpdateTick;
    int32_t m_mouseXPos;
    int32_t m_mouseYPos;
    bool m_hasFocus;
};
