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

#include "stdlib.h"
#include "math.h"
#include "PCSound.h"

PCSound::PCSound(const FileChunk* decompressedChunk) :
    m_length(0),
    m_priority(0),
    m_inverseFrequency(NULL)
{
    const uint32_t headerSize = sizeof(m_length) + sizeof(m_priority);
    if (decompressedChunk->GetSize() > headerSize)
    {
        m_length = *(uint32_t*)&decompressedChunk->GetChunk()[0];
        m_priority = *(uint16_t*)&decompressedChunk->GetChunk()[4];
        m_length = (m_length > decompressedChunk->GetSize() - headerSize) ? decompressedChunk->GetSize() - headerSize : m_length;
        m_inverseFrequency = new uint8_t[m_length];
        for (uint32_t i = 0; i < m_length; i++)
        {
            m_inverseFrequency[i] = decompressedChunk->GetChunk()[i + headerSize];
        }
    }
}

PCSound::~PCSound()
{
    if (m_inverseFrequency != NULL)
    {
        delete m_inverseFrequency;
    }
}

uint32_t PCSound::GetLength() const
{
    return m_length;
}

uint16_t PCSound::GetPriority() const
{
    return m_priority;
}

uint8_t PCSound::GetInverseFrequency(const uint32_t index) const
{
    return (m_inverseFrequency != NULL && index < m_length) ? m_inverseFrequency[index] : 0;
}

uint8_t* PCSound::GetData()
{
    return m_inverseFrequency;
}
