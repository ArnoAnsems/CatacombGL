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
// Shape
//
// A picture used in the introduction screens, which is read from a SHP??.ABS file.
//
#pragma once

#include "Picture.h"
#include "IRenderer.h"

class Shape
{
public:
    Shape(IRenderer& renderer);
    ~Shape();
    bool LoadFromFile(const char* filename);

    uint16_t GetOffsetX() const;
    uint16_t GetOffsetY() const;
    Picture* GetPicture() const;

private:
    uint8_t* m_data;
    int32_t m_dataSize;
    uint16_t m_bytesPerRow;
    uint16_t m_width;
    uint16_t m_height;
    uint16_t m_offsetX;
    uint16_t m_offsetY;
    uint8_t m_numberOfPlanes;
    uint8_t m_transparent;
    uint8_t m_compressed;
    uint8_t m_pad;
    Picture* m_picture;
    IRenderer& m_renderer;
};
