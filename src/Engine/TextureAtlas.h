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
// TextureAtlas
//
// Combines multiple images of the same size into a single texture
//
#pragma once

#include <stdint.h>

class TextureAtlas
{
public:
    TextureAtlas(
        const uint16_t imageWidth,
        const uint16_t imageHeight,
        const uint16_t numberOfColumns,
        const uint16_t numberOfRows,
        const uint16_t imageSpacingX,
        const uint16_t imageSpacingY);
    ~TextureAtlas();

    void StoreImage(const uint16_t imageIndex, const uint8_t* const pixelData);

    float GetImageRelativeOffsetX(const uint16_t imageIndex) const;
    float GetImageRelativeOffsetY(const uint16_t imageIndex) const;
    float GetImageRelativeWidth() const;
    float GetImageRelativeHeight() const;

    uint16_t GetTextureWidth() const;
    uint16_t GetTextureHeight() const;
    uint8_t* GetTexturePixelData() const;

private:
    static uint16_t GetNearestPowerOfTwo(const uint16_t size);
    static uint16_t CalculateTextureWidth(const uint16_t imageWidth, const uint16_t numberOfColumns, const uint16_t imageSpacingX);
    static uint16_t CalculateTextureHeight(const uint16_t imageHeight, const uint16_t numberOfRows, const uint16_t imageSpacingY);
    uint16_t CalculateOffsetInPixelsX(const uint16_t imageIndex) const;
    uint16_t CalculateOffsetInPixelsY(const uint16_t imageIndex) const;

    const uint16_t m_imageWidth;
    const uint16_t m_imageHeight;
    const uint16_t m_numberOfColumns;
    const uint16_t m_numberOfRows;
    const uint16_t m_imageSpacingX;
    const uint16_t m_imageSpacingY;
    const uint16_t m_textureWidth;
    const uint16_t m_textureHeight;
    const uint16_t m_bytesPerPixel;
    const float m_imageRelativeWidth;
    const float m_imageRelativeHeight;
    uint8_t* m_texturePixelData;
};
