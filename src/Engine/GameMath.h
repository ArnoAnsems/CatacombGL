// Copyright (C) 2026 Arno Ansems
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

#include <stdint.h>


namespace gamemath
{
    static constexpr uint32_t TicksPerSecond = 70u;
    static constexpr uint32_t MillisecondsPerSecond = 1000u;

    constexpr uint32_t TicksToMilliseconds(const uint32_t ticks)
    {
        return (ticks * MillisecondsPerSecond) / TicksPerSecond;
    }

    constexpr float MillisecondsToTicks(const uint32_t milliseconds)
    {
        return static_cast<float>(milliseconds * TicksPerSecond) / static_cast<float>(MillisecondsPerSecond);
    }

    constexpr float DegreesToRadians(const float degrees)
    {
        constexpr float pi = 3.141592653589793;
        return (degrees * pi) / 180.0f;
    }

};
