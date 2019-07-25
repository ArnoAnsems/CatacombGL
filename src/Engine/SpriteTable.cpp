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

#include "SpriteTable.h"

SpriteTable::SpriteTable(FileChunk* decompressedChunk)
{
    m_count = (uint16_t)decompressedChunk->GetSize() / 16;
    m_width = new uint16_t[m_count];
    m_height = new uint16_t[m_count];
    m_offsetX = new uint16_t[m_count];
    m_offsetY = new uint16_t[m_count];
    m_clippingLeft = new uint16_t[m_count];
    m_clippingTop = new uint16_t[m_count];
    m_clippingRight = new uint16_t[m_count];
    m_clippingBottom = new uint16_t[m_count];
    uint8_t* chunk = decompressedChunk->GetChunk();
    for (uint16_t i = 0; i < m_count; i++)
    {
        m_width[i] = *(uint16_t*)&chunk[i * 16] * 8;
        m_height[i] = *(uint16_t*)&chunk[(i * 16) + 2];
        m_offsetX[i] = *(uint16_t*)&chunk[(i * 16) + 4];
        m_offsetY[i] = *(uint16_t*)&chunk[(i * 16) + 6];
        m_clippingLeft[i] = *(uint16_t*)&chunk[(i * 16) + 8];
        m_clippingTop[i] = *(uint16_t*)&chunk[(i * 16) + 10];
        m_clippingRight[i] = *(uint16_t*)&chunk[(i * 16) + 12];
        m_clippingBottom[i] = *(uint16_t*)&chunk[(i * 16) + 14];
    }
}

SpriteTable::~SpriteTable()
{
    if (m_width != 0)
    {
        delete[] m_width;
    }
    if (m_height != 0)
    {
        delete[] m_height;
    }
    if (m_offsetX != 0)
    {
        delete[] m_offsetX;
    }
    if (m_offsetY != 0)
    {
        delete[] m_offsetY;
    }
    if (m_clippingLeft != 0)
    {
        delete[] m_clippingLeft;
    }
    if (m_clippingTop != 0)
    {
        delete[] m_clippingTop;
    }
    if (m_clippingRight != 0)
    {
        delete[] m_clippingRight;
    }
    if (m_clippingBottom != 0)
    {
        delete[] m_clippingBottom;
    }
}

uint16_t SpriteTable::GetWidth(const uint16_t index)
{
    if (index < m_count)
    {
        return m_width[index];
    }

    return 0;
}

uint16_t SpriteTable::GetHeight(const uint16_t index)
{
    if (index < m_count)
    {
        return m_height[index];
    }

    return 0;
}

uint16_t SpriteTable::GetOffsetX(const uint16_t index)
{
    if (index < m_count)
    {
        return m_offsetX[index];
    }

    return 0;
}

uint16_t SpriteTable::GetOffsetY(const uint16_t index)
{
    if (index < m_count)
    {
        return m_offsetY[index];
    }

    return 0;
}

uint16_t SpriteTable::GetClippingLeft(const uint16_t index)
{
    if (index < m_count)
    {
        return m_clippingLeft[index];
    }

    return 0;
}

uint16_t SpriteTable::GetClippingTop(const uint16_t index)
{
    if (index < m_count)
    {
        return m_clippingTop[index];
    }

    return 0;
}

uint16_t SpriteTable::GetClippingRight(const uint16_t index)
{
    if (index < m_count)
    {
        return m_clippingRight[index];
    }

    return 0;
}

uint16_t SpriteTable::GetClippingBottom(const uint16_t index)
{
    if (index < m_count)
    {
        return m_clippingBottom[index];
    }

    return 0;
}

uint16_t SpriteTable::GetCount()
{
    return m_count;
}