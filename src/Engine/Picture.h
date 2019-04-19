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
// Picture
//
// Contains a single picture (wall texture, sprite texture, etc...)
//
#pragma once

#include "FileChunk.h"

class Picture
{
public:
    Picture(const unsigned int textureId, const uint16_t width, const uint16_t height);
    ~Picture();

    unsigned int GetTextureId() const;
    uint16_t GetWidth() const;
    uint16_t GetHeight() const;

private:
    uint16_t m_width;
    uint16_t m_height;
    unsigned int m_textureId;
};

