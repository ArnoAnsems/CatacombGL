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

class Uint16Utility
{
public:
    // Based on the function BE_Cross_Swap16 in ReflectionHLE by NY00123.
    static constexpr uint16_t Swap(const uint16_t inputValue)
    {
        return ((uint16_t)((inputValue << 8u) | (inputValue >> 8u)));
    }

    static constexpr uint16_t NearestPowerOfTwo(const uint16_t inputValue)
    {
        // The maximum power of two that can be expressed in a 16 bit unsigned integer.
        constexpr uint16_t maxPowerOfTwo = 1 << 15u;

        uint16_t powerOfTwo = 1;
        while (powerOfTwo < inputValue && powerOfTwo < maxPowerOfTwo)
        {
            powerOfTwo *= 2;
        }

        return powerOfTwo;
    }
};

static_assert(Uint16Utility::Swap(0x0000) == 0x0000);
static_assert(Uint16Utility::Swap(0x1234) == 0x3412);
static_assert(Uint16Utility::Swap(0xFFFF) == 0xFFFF);

static_assert(Uint16Utility::NearestPowerOfTwo(0) == 1);
static_assert(Uint16Utility::NearestPowerOfTwo(1) == 1);
static_assert(Uint16Utility::NearestPowerOfTwo(2) == 2);
static_assert(Uint16Utility::NearestPowerOfTwo(3) == 4);
static_assert(Uint16Utility::NearestPowerOfTwo(5) == 8);
static_assert(Uint16Utility::NearestPowerOfTwo(32769) == 32768);