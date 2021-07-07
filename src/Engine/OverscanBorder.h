// Copyright (C) 2021 Arno Ansems
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
#pragma once

//
// OverscanBorder
//
// The overscan border is the area that surrounds the pixel space on a CRT monitor.
// It can be assigned a color and is for example used in the original game to flash red
// when the player gets hurt.
//

#include "EgaColor.h"

class OverscanBorder
{
public:
    OverscanBorder();
    ~OverscanBorder();

    void SetColor(const uint32_t timeStamp, const egaColor color, const uint32_t durationInMs);
    egaColor GetActiveColor(const uint32_t timeStamp) const;

private:
    egaColor m_color;
    uint32_t m_timeStampWhenColorIsExpired;
};
