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
#include "AdlibSound.h"

AdlibSound::AdlibSound() :
    m_length(0),
    m_priority(0),
    m_data(nullptr)
{
    for (uint8_t j = 0; j < 16; j++)
    {
        m_instruments[j] = 0;
    }
}

AdlibSound::AdlibSound(const FileChunk* decompressedChunk) :
    m_length(0),
    m_priority(0),
    m_data(nullptr)
{
    const uint32_t headerSize = sizeof(m_length) + sizeof(m_priority) + sizeof(m_instruments) + sizeof(m_octave);
    if (decompressedChunk->GetSize() > headerSize)
    {
        m_length = *(uint32_t*)&decompressedChunk->GetChunk()[0];
        m_length = (m_length > decompressedChunk->GetSize() - headerSize) ? decompressedChunk->GetSize() - headerSize : m_length;
        m_priority = *(uint16_t*)&decompressedChunk->GetChunk()[4];
        for (uint8_t j = 0; j < 16; j++)
        {
            m_instruments[j] = *(uint8_t*)&decompressedChunk->GetChunk()[6 + j];
        }
        m_octave = *(uint8_t*)&decompressedChunk->GetChunk()[22];
        m_data = new uint8_t[m_length];
        for (uint32_t i = 0; i < m_length; i++)
        {
            m_data[i] = decompressedChunk->GetChunk()[i + headerSize];
        }
    }
}

AdlibSound::~AdlibSound()
{
    delete[] m_data;
}

uint32_t AdlibSound::GetLength() const
{
    return m_length;
}

uint16_t AdlibSound::GetPriority() const
{
    return m_priority;
}

uint8_t* AdlibSound::GetInstruments() const
{
    return (uint8_t*)&m_instruments;
}

uint8_t AdlibSound::GetOctave() const
{
    return m_octave;
}

uint8_t* AdlibSound::GetData() const
{
    return m_data;
}

uint8_t AdlibSound::GetmChar() const
{
    return m_instruments[0];
}

uint8_t AdlibSound::GetcChar() const
{
    return m_instruments[1];
}

uint8_t AdlibSound::GetmScale() const
{
    return m_instruments[2];
}

uint8_t AdlibSound::GetcScale() const
{
    return m_instruments[3];
}

uint8_t AdlibSound::GetmAttack() const
{
    return m_instruments[4];
}

uint8_t AdlibSound::GetcAttack() const
{
    return m_instruments[5];
}

uint8_t AdlibSound::GetmSus() const
{
    return m_instruments[6];
}

uint8_t AdlibSound::GetcSus() const
{
    return m_instruments[7];
}

uint8_t AdlibSound::GetmWave() const
{
    return m_instruments[8];
}

uint8_t AdlibSound::GetcWave() const
{
    return m_instruments[9];
}
