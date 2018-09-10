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
// Currently used as a way to transfer the input from the OS towards the OS-indepenent engine library.
// TODO: Get the input directly from the SDL2 library.
//
#pragma once

#include <stdint.h>
#include <map>
#include "..\..\ThirdParty\SDL\include\SDL_keycode.h"

const SDL_Keycode asciiKeyToSDL[256] =
{
    SDLK_UNKNOWN, // 0x00
    SDLK_UNKNOWN,
    SDLK_UNKNOWN,
    SDLK_UNKNOWN,
    SDLK_UNKNOWN, // 0x04
    SDLK_UNKNOWN,
    SDLK_UNKNOWN,
    SDLK_UNKNOWN,
    SDLK_BACKSPACE, // 0x08
    SDLK_TAB,
    SDLK_UNKNOWN,
    SDLK_UNKNOWN,
    SDLK_CLEAR,
    SDLK_RETURN, // 0x0D
    SDLK_UNKNOWN,
    SDLK_UNKNOWN,
    SDLK_RSHIFT, // 0x10
    SDLK_RCTRL, // 0x11
    SDLK_RALT, // 0x12
    SDLK_PAUSE, // 0x13
    SDLK_UNKNOWN, // 0x14
    SDLK_UNKNOWN, // 0x15
    SDLK_UNKNOWN, // 0x16
    SDLK_UNKNOWN, // 0x17
    SDLK_UNKNOWN, // 0x18
    SDLK_UNKNOWN, // 0x19
    SDLK_UNKNOWN, // 0x1a
    SDLK_ESCAPE, // 0x1b
    SDLK_UNKNOWN, // 0x1c
    SDLK_UNKNOWN, // 0x1d
    SDLK_UNKNOWN, // 0x1e
    SDLK_UNKNOWN, // 0x1f
    SDLK_SPACE, // 0x20
    SDLK_UNKNOWN, // 0x21
    SDLK_UNKNOWN, // 0x22
    SDLK_END, // 0x23
    SDLK_HOME, // 0x24
    SDLK_LEFT, // 0x25
    SDLK_UP, // 0x26
    SDLK_RIGHT, // 0x27
    SDLK_DOWN, // 0x28
    SDLK_UNKNOWN, // 0x29
    SDLK_UNKNOWN, // 0x2a
    SDLK_UNKNOWN, // 0x2b
    SDLK_UNKNOWN, // 0x2c
    SDLK_INSERT, // 0x2d
    SDLK_DELETE, // 0x2e
    SDLK_UNKNOWN, // 0x2f
    SDLK_0, // 0x30
    SDLK_1, // 0x31
    SDLK_2, // 0x32
    SDLK_3, // 0x33
    SDLK_4, // 0x34
    SDLK_5, // 0x35
    SDLK_6, // 0x36
    SDLK_7, // 0x37
    SDLK_8, // 0x38
    SDLK_9, // 0x39
    SDLK_UNKNOWN, // 0x3a
    SDLK_UNKNOWN, // 0x3b
    SDLK_UNKNOWN, // 0x3c
    SDLK_UNKNOWN, // 0x3d
    SDLK_UNKNOWN, // 0x3e
    SDLK_UNKNOWN, // 0x3f
    SDLK_UNKNOWN, // 0x40
    SDLK_a, // 0x41
    SDLK_b, // 0x42
    SDLK_c, // 0x43
    SDLK_d, // 0x44
    SDLK_e, // 0x45
    SDLK_f, // 0x46
    SDLK_g, // 0x47
    SDLK_h, // 0x48
    SDLK_i, // 0x49
    SDLK_j, // 0x4a
    SDLK_k, // 0x4b
    SDLK_l, // 0x4c
    SDLK_m, // 0x4d
    SDLK_n, // 0x4e
    SDLK_o, // 0x4f
    SDLK_p, // 0x50
    SDLK_q, // 0x51
    SDLK_r, // 0x52
    SDLK_s, // 0x53
    SDLK_t, // 0x54
    SDLK_u, // 0x55
    SDLK_v, // 0x56
    SDLK_w, // 0x57
    SDLK_x, // 0x58
    SDLK_y, // 0x59
    SDLK_z, // 0x5a
    SDLK_UNKNOWN, // 0x5b
    SDLK_UNKNOWN, // 0x5c
    SDLK_UNKNOWN, // 0x5d
    SDLK_UNKNOWN, // 0x5e
    SDLK_UNKNOWN, // 0x5f
    SDLK_KP_0, // 0x60
    SDLK_KP_1, // 0x61
    SDLK_KP_2, // 0x62
    SDLK_KP_3, // 0x63
    SDLK_KP_4, // 0x64
    SDLK_KP_5, // 0x65
    SDLK_KP_6, // 0x66
    SDLK_KP_7, // 0x67
    SDLK_KP_8, // 0x68
    SDLK_KP_9, // 0x69
    SDLK_KP_MULTIPLY, // 0x6a
    SDLK_KP_PLUS, // 0x6b
    SDLK_UNKNOWN, // 0x6c
    SDLK_KP_MINUS, // 0x6d
    SDLK_UNKNOWN, // 0x6e
    SDLK_KP_DIVIDE, // 0x6f
    SDLK_F1, // 0x70
    SDLK_F2, // 0x71
    SDLK_F3, // 0x72
    SDLK_F4, // 0x73
    SDLK_F5, // 0x74
    SDLK_F6, // 0x75
    SDLK_F7, // 0x76
    SDLK_F8, // 0x77
    SDLK_F9, // 0x78
    SDLK_F10, // 0x79
    SDLK_F11, // 0x7a
    SDLK_F12, // 0x7b
    SDLK_F13, // 0x7c
    SDLK_F14, // 0x7d
    SDLK_F15, // 0x7e
    SDLK_F16, // 0x7f
    SDLK_F17, // 0x80
    SDLK_F18, // 0x81
    SDLK_F19, // 0x82
    SDLK_F20, // 0x83
    SDLK_F21, // 0x84
    SDLK_F22, // 0x85
    SDLK_F23, // 0x86
    SDLK_F24, // 0x87
    SDLK_UNKNOWN, // 0x88
    SDLK_UNKNOWN, // 0x89
    SDLK_UNKNOWN, // 0x8a
    SDLK_UNKNOWN, // 0x8b
    SDLK_UNKNOWN, // 0x8c
    SDLK_UNKNOWN, // 0x8d
    SDLK_UNKNOWN, // 0x8e
    SDLK_UNKNOWN, // 0x8f
    SDLK_NUMLOCKCLEAR, // 0x90
    SDLK_SCROLLLOCK, // 0x91
    SDLK_UNKNOWN, // 0x92
    SDLK_UNKNOWN, // 0x93
    SDLK_UNKNOWN, // 0x94
    SDLK_UNKNOWN, // 0x95
    SDLK_UNKNOWN, // 0x96
    SDLK_UNKNOWN, // 0x97
    SDLK_UNKNOWN, // 0x98
    SDLK_UNKNOWN, // 0x99
    SDLK_UNKNOWN, // 0x9a
    SDLK_UNKNOWN, // 0x9b
    SDLK_UNKNOWN, // 0x9c
    SDLK_UNKNOWN, // 0x9d
    SDLK_UNKNOWN, // 0x9e
    SDLK_UNKNOWN, // 0x9f
    SDLK_LSHIFT, // 0xa0
    SDLK_RSHIFT, // 0xa1
    SDLK_LCTRL, // 0xa2
    SDLK_RCTRL, // 0xa3
    SDLK_LALT, // 0xa4
    SDLK_RALT, // 0xa5
    SDLK_UNKNOWN, // 0xa6
    SDLK_UNKNOWN, // 0xa7
    SDLK_UNKNOWN, // 0xa8
    SDLK_UNKNOWN, // 0xa9
    SDLK_UNKNOWN, // 0xaa
    SDLK_UNKNOWN, // 0xab
    SDLK_UNKNOWN, // 0xac
    SDLK_UNKNOWN, // 0xad
    SDLK_UNKNOWN, // 0xae
    SDLK_UNKNOWN, // 0xaf
    SDLK_UNKNOWN, // 0xa0
    SDLK_UNKNOWN, // 0xb1
    SDLK_UNKNOWN, // 0xb2
    SDLK_UNKNOWN, // 0xb3
    SDLK_UNKNOWN, // 0xb4
    SDLK_UNKNOWN, // 0xb5
    SDLK_UNKNOWN, // 0xb6
    SDLK_UNKNOWN, // 0xb7
    SDLK_UNKNOWN, // 0xb8
    SDLK_UNKNOWN, // 0xb9
    SDLK_UNKNOWN, // 0xba
    SDLK_UNKNOWN, // 0xbb
    SDLK_UNKNOWN, // 0xbc
    SDLK_UNKNOWN, // 0xbd
    SDLK_UNKNOWN, // 0xbe
    SDLK_UNKNOWN, // 0xbf
    SDLK_UNKNOWN, // 0xc0
    SDLK_UNKNOWN, // 0xc1
    SDLK_UNKNOWN, // 0xc2
    SDLK_UNKNOWN, // 0xc3
    SDLK_UNKNOWN, // 0xc4
    SDLK_UNKNOWN, // 0xc5
    SDLK_UNKNOWN, // 0xc6
    SDLK_UNKNOWN, // 0xc7
    SDLK_UNKNOWN, // 0xc8
    SDLK_UNKNOWN, // 0xc9
    SDLK_UNKNOWN, // 0xca
    SDLK_UNKNOWN, // 0xcb
    SDLK_UNKNOWN, // 0xcc
    SDLK_UNKNOWN, // 0xcd
    SDLK_UNKNOWN, // 0xce
    SDLK_UNKNOWN, // 0xcf
    SDLK_UNKNOWN, // 0xd0
    SDLK_UNKNOWN, // 0xd1
    SDLK_UNKNOWN, // 0xd2
    SDLK_UNKNOWN, // 0xd3
    SDLK_UNKNOWN, // 0xd4
    SDLK_UNKNOWN, // 0xd5
    SDLK_UNKNOWN, // 0xd6
    SDLK_UNKNOWN, // 0xd7
    SDLK_UNKNOWN, // 0xd8
    SDLK_UNKNOWN, // 0xd9
    SDLK_UNKNOWN, // 0xda
    SDLK_UNKNOWN, // 0xdb
    SDLK_UNKNOWN, // 0xdc
    SDLK_UNKNOWN, // 0xdd
    SDLK_UNKNOWN, // 0xde
    SDLK_UNKNOWN, // 0xdf
    SDLK_UNKNOWN, // 0xe0
    SDLK_UNKNOWN, // 0xe1
    SDLK_UNKNOWN, // 0xe2
    SDLK_UNKNOWN, // 0xe3
    SDLK_UNKNOWN, // 0xe4
    SDLK_UNKNOWN, // 0xe5
    SDLK_UNKNOWN, // 0xe6
    SDLK_UNKNOWN, // 0xe7
    SDLK_UNKNOWN, // 0xe8
    SDLK_UNKNOWN, // 0xe9
    SDLK_UNKNOWN, // 0xea
    SDLK_UNKNOWN, // 0xeb
    SDLK_UNKNOWN, // 0xec
    SDLK_UNKNOWN, // 0xed
    SDLK_UNKNOWN, // 0xee
    SDLK_UNKNOWN, // 0xef
    SDLK_UNKNOWN, // 0xf0
    SDLK_UNKNOWN, // 0xf1
    SDLK_UNKNOWN, // 0xf2
    SDLK_UNKNOWN, // 0xf3
    SDLK_UNKNOWN, // 0xf4
    SDLK_UNKNOWN, // 0xf5
    SDLK_UNKNOWN, // 0xf6
    SDLK_UNKNOWN, // 0xf7
    SDLK_UNKNOWN, // 0xf8
    SDLK_UNKNOWN, // 0xf9
    SDLK_UNKNOWN, // 0xfa
    SDLK_UNKNOWN, // 0xfb
    SDLK_UNKNOWN, // 0xfc
    SDLK_UNKNOWN, // 0xfd
    SDLK_UNKNOWN, // 0xfe
    SDLK_UNKNOWN  // 0xff
};

class PlayerInput
{
public:
    PlayerInput();
    ~PlayerInput();
    void SetKeyPressed(const uint8_t keyCode, const bool pressed);
    bool IsKeyJustPressed(const SDL_Keycode keyCode) const;
    bool IsKeyPressed(const SDL_Keycode keyCode) const;
    void ClearJustPressed();
    void SetMouseButtonPressed(const uint8_t buttonCode, const bool pressed);
    bool IsMouseButtonPressed(const uint8_t buttonCode) const;
    bool IsMouseButtonJustPressed(const uint8_t buttonCode) const;
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
    int32_t m_mouseXPos;
    int32_t m_mouseYPos;
    bool m_hasFocus;
};