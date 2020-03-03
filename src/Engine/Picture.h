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
    Picture(const unsigned int textureId, const uint16_t imageWidth, const uint16_t imageHeight, const uint16_t textureWidth, const uint16_t textureHeight);
    ~Picture();

    unsigned int GetTextureId() const;
    uint16_t GetImageWidth() const;
    uint16_t GetImageHeight() const;
    uint16_t GetTextureWidth() const;
    uint16_t GetTextureHeight() const;
    static uint16_t GetNearestPowerOfTwo(const uint16_t size);

private:
    uint16_t m_imageWidth;
    uint16_t m_imageHeight;
    uint16_t m_textureWidth;
    uint16_t m_textureHeight;
    unsigned int m_textureId;
};

