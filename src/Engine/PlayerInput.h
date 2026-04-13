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
#include <SDL_gamecontroller.h>
#include <cstdint>
#include <map>

class PlayerInput
{
public:
    PlayerInput();
    ~PlayerInput() = default;
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
    SDL_GameControllerButton GetFirstGameControllerButtonPressed() const;
    SDL_GameControllerAxis GetFirstGameControllerAxisPressed() const;
    void SetHasFocus(const bool focus);
    bool HasFocus() const;
    void ClearAll();
    void SetGameControllerButtonPressed(const SDL_GameControllerButton gameControllerButton, const bool pressed);
    bool IsGameControllerButtonJustPressed(const SDL_GameControllerButton gameControllerButton) const;
    bool IsGameControllerButtonPressed(const SDL_GameControllerButton gameControllerButton) const;
    int16_t GetGameControllerAxisPressed(const SDL_GameControllerAxis gameControllerAxis) const;
    void SetGameControllerAxisPressed(const SDL_GameControllerAxis gameControllerAxis, const int16_t value);
    bool GetGameControllerAxisJustPressedTowardsNegative(const SDL_GameControllerAxis gameControllerAxis) const;
    bool GetGameControllerAxisJustPressedTowardsPositive(const SDL_GameControllerAxis gameControllerAxis) const;
    void SetGameControllerDetected(const bool detected);
    bool IsGameControllerDetected() const;

private:
    std::map<SDL_Keycode, bool> m_keyPressed;
    std::map<SDL_Keycode, bool> m_keyJustPressed;
    std::map<SDL_GameControllerButton, bool> m_gameControllerButtonPressed;
    std::map<SDL_GameControllerButton, bool> m_gameControllerButtonJustPressed;
    bool m_buttonPressed[6];
    bool m_buttonJustPressed[6];
    uint32_t m_mouseUpdateTick;
    int32_t m_mouseXPos;
    int32_t m_mouseYPos;
    bool m_hasFocus;
    std::map<SDL_GameControllerAxis, int16_t> m_gameControllerAxisPressed;
    std::map<SDL_GameControllerAxis, bool> m_gameControllerAxisJustPressedTowardsNegative;
    std::map<SDL_GameControllerAxis, bool> m_gameControllerAxisJustPressedTowardsPositive;
    bool m_isGameControllerDetected = false;
};
