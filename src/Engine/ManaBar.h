// Copyright (C) 2020 Arno Ansems
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
// ManaBar
//
// The mana bar can be enabled to restrict the use of the primary magic missiles.
//
#pragma once

#include <stdint.h>
#include "IRenderer.h"

class ManaBar
{
public:
    struct ManaBarConfig
    {
        int16_t offsetY;
        egaColor frameColor;
        egaColor frameShadowColor;
    };
    ManaBar(const ManaBarConfig config);
    ~ManaBar();

    void Reset(const bool enabled);
    bool IsEnabled();
    bool FireShot();
    void Update(const uint32_t timestamp);
    void Draw(IRenderer& renderer);

private:
    const ManaBarConfig m_config;
    bool m_enabled;
    uint32_t m_mana;
    uint32_t m_previousTimestamp;
};