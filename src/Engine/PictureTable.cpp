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

#include "PictureTable.h"

PictureTable::PictureTable(FileChunk* decompressedChunk) :
    m_count(0),
    m_width(nullptr),
    m_height(nullptr)
{
    m_count = (uint16_t)decompressedChunk->GetSize() / 4;

    if (m_count > 0)
    {
        m_width = new uint16_t[m_count];
        m_height = new uint16_t[m_count];
        uint8_t* chunk = decompressedChunk->GetChunk();
        for (uint16_t i = 0; i < m_count; i++)
        {
            m_width[i] = *(uint16_t*)&chunk[i * 4] * 8;
            m_height[i] = *(uint16_t*)&chunk[(i * 4) + 2];
        }
    }
}

PictureTable::~PictureTable()
{
    delete[] m_width;
    delete[] m_height;
}

uint16_t PictureTable::GetWidth(const uint16_t index)
{
    if (index < m_count)
    {
        return m_width[index];
    }

    return 0;
}

uint16_t PictureTable::GetHeight(const uint16_t index)
{
    if (index < m_count)
    {
        return m_height[index];
    }

    return 0;
}

uint16_t PictureTable::GetCount()
{
    return m_count;
}