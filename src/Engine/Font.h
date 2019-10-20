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
// Font
//
// Contains the font that was read from an EGAGRAPH file. Required for printing text to the screen.
//
#pragma once

#include "FileChunk.h"

class Font
{
public:
    Font(const uint16_t height, const uint8_t width[256], const unsigned int textureId);
    ~Font();

    unsigned int GetTextureId() const;
    uint16_t GetCharacterWidth(const uint16_t charIndex) const;
    uint16_t GetCharacterHeight() const;

private:
    static const uint16_t NumChar = 256;
    uint16_t m_height;
    uint8_t m_width[256];
    unsigned int m_textureId;
};
