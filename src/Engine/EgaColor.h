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
// EgaColor
//
// Covenient enumeration to translate the default EGA 4 bit palette into colors. 
//
#pragma once

#include <stdint.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} rgbColor;

typedef enum
{
    EgaBlack = 0,
    EgaBlue = 1,
    EgaGreen = 2,
    EgaCyan = 3,
    EgaRed = 4,
    EgaMagenta = 5,
    EgaBrown = 6,
    EgaLightGray = 7,
    EgaDarkGray = 8,
    EgaBrightBlue  = 9,
    EgaBrightGreen = 10,
    EgaBrightCyan = 11,
    EgaBrightRed = 12,
    EgaBrightMagenta = 13,
    EgaBrightYellow = 14,
    EgaBrightWhite = 15,
    EgaRange = 16
} egaColor;

const rgbColor egaToRgbMap[EgaRange] =
{
    { 0, 0, 0 },    // Black
    { 0, 0, 170 },    // Blue
    { 0, 170, 0},    // Green
    { 0, 170, 170 },    // Cyan
    { 170, 0, 0 },    // Red
    { 170, 0, 170 },    // Magenta
    { 170, 85, 0 },    // Brown
    { 170, 170, 170 },    // Light gray
    { 85, 85, 85 },    // Dark gray
    { 85, 85, 255 },    // Bright blue
    { 85, 255, 85 },    // Bright green
    { 85, 255, 255 },    // Bright cyan
    { 255, 85, 85 },    // Bright red
    { 255, 85, 255 },    // Bright magenta
    { 255, 255, 85 },    // Bright yellow
    { 255, 255, 255 }     // Bright white
};

constexpr rgbColor EgaToRgb(const egaColor ega)
{
    if (ega < EgaRange)
    {
        return egaToRgbMap[ega];
    }
    else
    {
        return egaToRgbMap[egaColor::EgaBlack];
    }
};