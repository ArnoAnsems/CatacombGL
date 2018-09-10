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
#pragma once

#include "FileChunk.h"

class AdlibSound
{
public:
    AdlibSound();
    AdlibSound(const FileChunk* decompressedChunk);
    ~AdlibSound();

    uint32_t GetLength() const;
    uint16_t GetPriority() const;
    uint8_t* GetInstruments() const;
    uint8_t GetOctave() const;
    uint8_t* GetData() const;

    uint8_t GetmChar() const;
    uint8_t GetcChar() const;
    uint8_t GetmScale() const;
    uint8_t GetcScale() const;
    uint8_t GetmAttack() const;
    uint8_t GetcAttack() const;
    uint8_t GetmSus() const;
    uint8_t GetcSus() const;
    uint8_t GetmWave() const;
    uint8_t GetcWave() const;

private:
    uint32_t m_length;
    uint16_t m_priority;
    uint8_t m_instruments[16];
    uint8_t m_octave;
    uint8_t* m_data;
};

