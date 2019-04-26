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

#include "AudioRepository.h"
#include <fstream>
#include "AdlibSound.h"
#include "PCSound.h"

AudioRepository::AudioRepository(const audioRepositoryStaticData& staticData, const std::string& path) :
    m_staticData(staticData)
{
    // Initialize Huffman table
    m_huffman = new Huffman(staticData.table);

    // Read the entire audio repository file into memory
    uint32_t fileSize = staticData.offsets.back();
    m_rawData = new FileChunk(fileSize);
    std::ifstream file;
    const std::string fullPath = path + staticData.filename;
    file.open(fullPath, std::ifstream::in | std::ifstream::binary);
    if (file.is_open())
    {
        file.read((char*)m_rawData->GetChunk(), fileSize);
        file.close();
    }

    // Initialize PC and Adlib sounds
    m_pcSounds = new PCSound*[staticData.lastSound];
    m_adlibSounds = new AdlibSound*[staticData.lastSound];
    for (uint16_t i = 0; i < staticData.lastSound; i++)
    {
        m_pcSounds[i] = NULL;
        m_adlibSounds[i] = NULL;
    }
}

AudioRepository::~AudioRepository()
{
    for (uint16_t i = 0; i < m_staticData.lastSound; i++)
    {
        if (m_pcSounds[i] != NULL)
        {
            delete m_pcSounds[i];
        }
        if (m_adlibSounds[i] != NULL)
        {
            delete m_adlibSounds[i];
        }
    }
    delete[] m_pcSounds;
    delete[] m_adlibSounds;

    if (m_huffman != NULL)
    {
        delete m_huffman;
    }
    if (m_rawData != NULL)
    {
        delete m_rawData;
    }
}

PCSound* AudioRepository::GetPCSound(const uint16_t index)
{
    if (index >= m_staticData.lastSound)
    {
        return NULL;
    }

    if (m_pcSounds[index] == NULL)
    {
        uint8_t* compressedSound = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(index)];
        uint32_t compressedSize = GetChunkSize(index) - sizeof(uint32_t);
        uint32_t uncompressedSize = *(uint32_t*)compressedSound;
        FileChunk* soundChunk = m_huffman->Decompress(&compressedSound[sizeof(uint32_t)], compressedSize, uncompressedSize);
        m_pcSounds[index] = new PCSound(soundChunk);
        delete soundChunk;
    }

    return m_pcSounds[index]; 
}

AdlibSound* AudioRepository::GetAdlibSound(const uint16_t index)
{
    if (index >= m_staticData.lastSound)
    {
        return NULL;
    }

    if (m_adlibSounds[index] == NULL)
    {
        uint8_t* compressedSound = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(index + m_staticData.lastSound)];
        uint32_t compressedSize = GetChunkSize(index + m_staticData.lastSound) - sizeof(uint32_t);
        uint32_t uncompressedSize = *(uint32_t*)compressedSound;
        FileChunk* soundChunk = m_huffman->Decompress(&compressedSound[sizeof(uint32_t)], compressedSize, uncompressedSize);
        m_adlibSounds[index] = new AdlibSound(soundChunk);
        delete soundChunk;
    }

    return m_adlibSounds[index]; 
}

uint32_t AudioRepository::GetChunkSize(const uint16_t index)
{
    if (index > m_staticData.offsets.size() - 1)
    {
        return 0;
    }

    int32_t pos = m_staticData.offsets.at(index);
    if (pos<0)
    {
        return 0;
    }

    uint16_t next = index + 1;
    while (m_staticData.offsets.at(index) == -1)		// skip past any sparse tiles
    {
        next++;
    }

    return m_staticData.offsets.at(next) - pos;
}