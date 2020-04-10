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

#include "TextureAtlas.h"

TextureAtlas::TextureAtlas(
    const unsigned int textureId,
    const uint16_t imageWidth,
    const uint16_t imageHeight,
    const uint16_t numberOfColumns,
    const uint16_t numberOfRows,
    const uint16_t imageSpacingX,
    const uint16_t imageSpacingY) :
    m_textureId(textureId),
    m_imageWidth(imageWidth),
    m_imageHeight(imageHeight),
    m_numberOfColumns(numberOfColumns),
    m_numberOfRows(numberOfRows),
    m_imageSpacingX(imageSpacingX),
    m_imageSpacingY(imageSpacingY),
    m_textureWidth(CalculateTextureWidth(imageWidth, numberOfColumns, imageSpacingX)),
    m_textureHeight(CalculateTextureHeight(imageHeight, numberOfRows, imageSpacingY)),
    m_bytesPerPixel(4),
    m_imageRelativeWidth((float)m_imageWidth / (float)m_textureWidth),
    m_imageRelativeHeight((float)m_imageHeight / (float)m_textureHeight)
{
    const size_t textureSize = m_textureWidth * m_textureHeight * m_bytesPerPixel;
    m_texturePixelData = new uint8_t[textureSize];

    // Fill the texture with zero's, which amounts to black and fully transparent.
    for (size_t i = 0; i < textureSize; i++)
    {
        m_texturePixelData[i] = 0;
    }
}

TextureAtlas::~TextureAtlas()
{
    delete m_texturePixelData;
}

unsigned int TextureAtlas::GetTextureId() const
{
    return m_textureId;
}

uint16_t TextureAtlas::GetNearestPowerOfTwo(const uint16_t size)
{
    // In order to support OpenGL 1.4, the texture width and height need to be a power of two.
    uint16_t powerOfTwo = 1;
    while (powerOfTwo < size)
    {
        powerOfTwo *= 2;
    }

    return powerOfTwo;
}

uint16_t TextureAtlas::CalculateTextureWidth(const uint16_t imageWidth, const uint16_t numberOfColumns, const uint16_t imageSpacingX)
{
    const uint16_t totalRequiredWidth = (imageWidth * numberOfColumns) + (imageSpacingX * (numberOfColumns - 1));
    return GetNearestPowerOfTwo(totalRequiredWidth);
}

uint16_t TextureAtlas::CalculateTextureHeight(const uint16_t imageHeight, const uint16_t numberOfRows, const uint16_t imageSpacingY)
{
    const uint16_t totalRequiredHeight = (imageHeight * numberOfRows) + (imageSpacingY * (numberOfRows - 1));
    return GetNearestPowerOfTwo(totalRequiredHeight);
}

uint16_t TextureAtlas::CalculateOffsetInPixelsX(const uint16_t imageIndex) const
{
    const uint16_t column = imageIndex % m_numberOfColumns;
    return (column * (m_imageWidth + m_imageSpacingX));
}

uint16_t TextureAtlas::CalculateOffsetInPixelsY(const uint16_t imageIndex) const
{
    const uint16_t row = imageIndex / m_numberOfColumns;
    return (row * (m_imageHeight + m_imageSpacingY));
}

void TextureAtlas::StoreImage(const uint16_t imageIndex, const uint8_t* const pixelData)
{
    const uint16_t offsetInPixelsX = CalculateOffsetInPixelsX(imageIndex);
    const uint16_t offsetInPixelsY = CalculateOffsetInPixelsY(imageIndex);
    for (uint16_t rowIndex = 0; rowIndex < m_imageHeight; rowIndex++)
    {
        const uint8_t* sourceRowPtr = pixelData + (rowIndex * m_imageWidth * m_bytesPerPixel);
        uint8_t* destinationRowPtr = m_texturePixelData + ((((offsetInPixelsY + rowIndex) * m_textureWidth) + offsetInPixelsX) * m_bytesPerPixel);
        for (uint16_t p = 0; p < m_imageWidth * m_bytesPerPixel; p++)
        {
            destinationRowPtr[0] = *sourceRowPtr;
            sourceRowPtr++;
            destinationRowPtr++;
        }
    }
}

uint16_t TextureAtlas::GetImageWidth() const
{
    return m_imageWidth;
}

uint16_t TextureAtlas::GetImageHeight() const
{
    return m_imageHeight;
}

float TextureAtlas::GetImageRelativeOffsetX(const uint16_t imageIndex) const
{
    const uint16_t offsetInPixelsX = CalculateOffsetInPixelsX(imageIndex);
    return ((float)offsetInPixelsX / (float)m_textureWidth);
}

float TextureAtlas::GetImageRelativeOffsetY(const uint16_t imageIndex) const
{
    const uint16_t offsetInPixelsY = CalculateOffsetInPixelsY(imageIndex);
    return ((float)offsetInPixelsY / (float)m_textureHeight);
}

float TextureAtlas::GetImageRelativeWidth() const
{
    return m_imageRelativeWidth;
}

float TextureAtlas::GetImageRelativeHeight() const
{
    return m_imageRelativeHeight;
}

uint16_t TextureAtlas::GetTextureWidth() const
{
    return m_textureWidth;
}

uint16_t TextureAtlas::GetTextureHeight() const
{
    return m_textureHeight;
}

uint8_t* TextureAtlas::GetTexturePixelData() const
{
    return m_texturePixelData;
}