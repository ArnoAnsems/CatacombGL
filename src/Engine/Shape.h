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
#include "Logging.h"

class Shape
{
public:
    Shape(const IRenderer& renderer);
    ~Shape();
    bool LoadFromFile(const char* filename);

    uint16_t GetOffsetX() const;
    uint16_t GetOffsetY() const;
    Picture* GetPicture() const;

private:
    unsigned int LoadFileChunkIntoTexture(
        const FileChunk* decompressedChunk,
        const uint16_t imageWidth,
        const uint16_t imageHeight,
        const uint16_t textureWidth,
        const uint16_t textureHeight,
        const bool transparent);

    uint16_t m_offsetX;
    uint16_t m_offsetY;
    Picture* m_picture;
    const IRenderer& m_renderer;
};
