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
#include "AdlibSound.h"
#include "PCSound.h"
#include <cstring>
#include <fstream>

namespace fs = std::filesystem;

AudioRepository::AudioRepository(const audioRepositoryStaticData& staticData, const fs::path& path) :
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
    const fs::path fullPath = path / staticData.filename;
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
        Logging::Instance().FatalError("Failed to open " + fullPath.string());
    }

    // Initialize PC and Adlib sounds
    m_pcSounds = new PCSound*[staticData.lastSound];
    m_adlibSounds = new AdlibSound*[staticData.lastSound];
    m_musicTracks = new FileChunk*[staticData.lastSound];
    for (uint16_t i = 0; i < staticData.lastSound; i++)
    {
        m_pcSounds[i] = nullptr;
        m_adlibSounds[i] = nullptr;
        m_musicTracks[i] = nullptr;
    }
}

AudioRepository::~AudioRepository()
{
    for (uint16_t i = 0; i < m_staticData.lastSound; i++)
    {
        delete m_pcSounds[i];
        m_pcSounds[i] = nullptr;
        delete m_adlibSounds[i];
        m_adlibSounds[i] = nullptr;
        delete m_musicTracks[i];
        m_musicTracks[i] = nullptr;
    }
    delete[] m_pcSounds;
    delete[] m_adlibSounds;
    delete[] m_musicTracks;

    delete m_huffman;
    delete m_rawData;
}

PCSound* AudioRepository::GetPCSound(const uint16_t index)
{
    if (index >= m_staticData.lastSound)
    {
        return nullptr;
    }

    if (m_pcSounds[index] == nullptr)
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
        return nullptr;
    }

    if (m_adlibSounds[index] == nullptr)
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
        return nullptr;
    }

    const uint16_t chunkIndex = index + (m_staticData.lastSound * 3);
    if (chunkIndex >= m_staticData.offsets.size())
    {
        return nullptr;
    }

    if (m_musicTracks[index] == nullptr)
    {
        uint8_t* compressedSound = (uint8_t*)&m_rawData->GetChunk()[m_staticData.offsets.at(chunkIndex)];
        uint32_t compressedSize = GetChunkSize(index + (m_staticData.lastSound * 3)) - sizeof(uint32_t);
        uint32_t uncompressedSize = *(uint32_t*)compressedSound;
        FileChunk* soundChunk = m_huffman->Decompress(&compressedSound[sizeof(uint32_t)], compressedSize, uncompressedSize);
        const uint16_t musicTrackLength = *(uint16_t*)soundChunk->GetChunk();

        // Music tracks in the Catacomb 3D games are stored as IMF data, which essentially consists of a stream of
        // instructions for the OPL chip. All Catacomb 3-D games contain the music track "Too hot to handle". The music
        // data of this track starts with 9 instructions that alter the first channel. In some games this has the side
        // effect of altering the sound effects. To prevent this, the first nine instructions (36 bytes) are dynamically
        // removed here from the music data. This has no noticeable impact on the music.
        // Credits to NY00123 for suggesting this workaround.
        const uint16_t bytesToRemove = m_staticData.musicShouldNotInterfereWithFirstChannel ? 36u : 0u;
        m_musicTracks[index] = new FileChunk(musicTrackLength - bytesToRemove);
        memcpy(m_musicTracks[index]->GetChunk(), soundChunk->GetChunk() + sizeof(uint16_t) + bytesToRemove, musicTrackLength - bytesToRemove);
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
