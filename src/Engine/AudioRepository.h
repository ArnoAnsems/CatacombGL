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

#include <stdint.h>
#include <vector>
#include <string>
#include "Huffman.h"
#include "Logging.h"

class AdlibSound;
class PCSound;

typedef struct audioRepositoryStaticData
{
    const std::string filename;
    const std::vector<int32_t>& offsets;
    const huffmanTable& table;
    const int32_t lastSound;
} audioRepositoryStaticData;

class AudioRepository
{
public:
    AudioRepository(const audioRepositoryStaticData& staticData, const std::string& path);
    ~AudioRepository();

    PCSound* GetPCSound(const uint16_t index);
    AdlibSound* GetAdlibSound(const uint16_t index);
    FileChunk* GetMusicTrack(const uint16_t index);

private:
    uint32_t GetChunkSize(const uint16_t index);

    const audioRepositoryStaticData& m_staticData;

    FileChunk* m_rawData;
    PCSound** m_pcSounds;
    AdlibSound** m_adlibSounds;
    FileChunk** m_musicTracks;
    Huffman* m_huffman;
};

