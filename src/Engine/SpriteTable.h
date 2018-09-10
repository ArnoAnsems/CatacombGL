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
// SpriteTable
//
// Table with the properties of a number of sprites, as read from a chunk of the EGAGRAPH repository file.
//
#pragma once

#include "FileChunk.h"

class SpriteTable
{
public:
    SpriteTable(FileChunk* decompressedChunk);
    ~SpriteTable();

    uint16_t GetWidth(const uint16_t index);
    uint16_t GetHeight(const uint16_t index);
    uint16_t GetOffsetX(const uint16_t index);
    uint16_t GetOffsetY(const uint16_t index);
    uint16_t GetClippingLeft(const uint16_t index);
    uint16_t GetClippingTop(const uint16_t index);
    uint16_t GetClippingRight(const uint16_t index);
    uint16_t GetClippingBottom(const uint16_t index);
    uint16_t GetCount();

private:
    uint16_t* m_width;
    uint16_t* m_height;
    uint16_t* m_offsetX;
    uint16_t* m_offsetY;
    uint16_t* m_clippingLeft;
    uint16_t* m_clippingTop;
    uint16_t* m_clippingRight;
    uint16_t* m_clippingBottom;
    uint16_t m_count;
};

