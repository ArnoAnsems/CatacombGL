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
    Logging::Instance().AddLogMessage("Loading " + m_staticData.filename);

    // Check that there are sufficient entries for both PC sounds and Adlib sounds
    if (m_staticData.offsets.size() < (uint32_t)(m_staticData.lastSound * 2) + 1)
    {
        Logging::Instance().FatalError("Insufficient entries (" + std::to_string(m_staticData.offsets.size()) + ") in " +
            m_staticData.filename + " for " + std::to_string(m_staticData.lastSound) + " PC and adlib sounds");
    }

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
        if (file.fail())
        {
            Logging::Instance().FatalError("Failed to read " + std::to_string(fileSize) + " bytes from " + m_staticData.filename);
        }
        file.close();
    }
    else
    {
        Logging::Instance().FatalError("Failed to open " + fullPath);
    }

    // Initialize PC and Adlib sounds
    m_pcSounds = new PCSound*[staticData.lastSound];
    m_adlibSounds = new AdlibSound*[staticData.lastSound];
    m_musicTracks = new FileChunk*[staticData.lastSound];
    for (uint16_t i = 0; i < staticData.lastSound; i++)
    {
        m_pcSounds[i] = NULL;
        m_adlibSounds[i] = NULL;
        m_musicTracks[i] = NULL;
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
        if (m_musicTracks[i] != NULL)
        {
            delete m_musicTracks[i];
        }
    }
    delete[] m_pcSounds;
    delete[] m_adlibSounds;
    delete[] m_musicTracks;

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

FileChunk* AudioRepository::GetMusicTrack(const uint16_t index)
{
    if (index >= m_staticData.lastSound)
    {
        return NULL;
    }

    if (m_musicTracks[index] == NULL)
    {
        uint8_t* compressedSound = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(index + (m_staticData.lastSound * 3))];
        uint32_t compressedSize = GetChunkSize(index + (m_staticData.lastSound * 3)) - sizeof(uint32_t);
        uint32_t uncompressedSize = *(uint32_t*)compressedSound;
        FileChunk* soundChunk = m_huffman->Decompress(&compressedSound[sizeof(uint32_t)], compressedSize, uncompressedSize);
        const uint16_t musicTrackLength = *(uint16_t*)soundChunk->GetChunk();
        m_musicTracks[index] = new FileChunk(musicTrackLength);
        memcpy(m_musicTracks[index]->GetChunk(), soundChunk->GetChunk() + sizeof(uint16_t), musicTrackLength);
        delete soundChunk;
    }

    return m_musicTracks[index];
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