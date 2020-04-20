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
// RenderableText
//
// Converts text strings into a format that can be processed by the renderer.
//
#pragma once

#include "EgaColor.h"
#include "Font.h"
#include <vector>
#include <string>

class RenderableText
{
public:
    typedef struct
    {
        int16_t offsetX;
        int16_t offsetY;
        uint16_t imageIndex;
        egaColor color;
    } renderableCharacter;

    RenderableText(const Font& font);

    const std::vector<renderableCharacter>& GetText() const;
    const Font& GetFont() const;

    void LeftAligned(
        const std::string& text,
        const egaColor color,
        const int16_t offsetX,
        const int16_t offsetY);
    void LeftAlignedTruncated(
        const std::string& text,
        const egaColor color,
        const int16_t offsetX,
        const int16_t offsetY,
        const uint16_t maxLength);
    uint8_t LeftAlignedMultiLine(
        const std::string& text,
        const egaColor color,
        const int16_t offsetX,
        const int16_t offsetY);
    void Centered(
        const std::string& text,
        const egaColor color,
        const int16_t offsetX,
        const int16_t offsetY);
    void Number(
        const uint16_t value,
        const uint8_t maxDigits,
        const egaColor color,
        const int16_t offsetX,
        const int16_t offsetY);

private:
    std::vector<renderableCharacter> m_text;
    const Font& m_font;
};