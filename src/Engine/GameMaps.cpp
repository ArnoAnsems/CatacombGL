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

#include "GameMaps.h"
#include <fstream>
#include "Decompressor.h"


GameMaps::GameMaps(const gameMapsStaticData& staticData, const std::string& path) :
    m_staticData(staticData)
{
    // Read the entire GameMaps file into memory
    uint32_t fileSize = staticData.offsets.back();
    m_rawData = new FileChunk(fileSize);

    std::ifstream file;
    const std::string fullPath = path + staticData.filename;
    file.open(fullPath, std::ifstream::binary);
    if (file.is_open())
    {
        file.read((char*)m_rawData->GetChunk(), fileSize);
        file.close();
    }

}

GameMaps::~GameMaps()
{
    delete m_rawData;
}

Level* GameMaps::GetLevelFromStart(const uint8_t mapIndex) const
{
    uint16_t rlewTag = 0xABCD; //*(uint16_t*)(m_rawData->GetChunk());
    uint8_t* headerStart = &(m_rawData->GetChunk()[m_staticData.offsets.at(mapIndex)]);
    const uint32_t plane0Offset = *(uint32_t*)(headerStart);
    const uint32_t plane2Offset = *(uint32_t*)(&(headerStart[8]));
    const uint16_t plane0Length = *(uint16_t*)(&(headerStart[12]));
    const uint16_t plane2Length = *(uint16_t*)(&(headerStart[16]));
    const uint16_t mapWidth = *(uint16_t*)(&(headerStart[18]));
    const uint16_t mapHeight = *(uint16_t*)(&(headerStart[20]));

    uint8_t* plane0Source = &(m_rawData->GetChunk()[plane0Offset]);
    FileChunk* carmackExpandedChunk = Decompressor::CarmackExpand(plane0Source);
    FileChunk* decompressedPlane0 = Decompressor::RLEW_Decompress(carmackExpandedChunk->GetChunk(), rlewTag);

    uint8_t* plane2Source = &(m_rawData->GetChunk()[plane2Offset]);
    FileChunk* carmackExpandedChunk2 = Decompressor::CarmackExpand(plane2Source);
    FileChunk* decompressedPlane2 = Decompressor::RLEW_Decompress(carmackExpandedChunk2->GetChunk(), rlewTag);

    return new Level(mapIndex, mapWidth, mapHeight, (uint16_t*)(decompressedPlane0->GetChunk()), (uint16_t*)(decompressedPlane2->GetChunk()), m_staticData.mapsInfo.at(mapIndex), m_staticData.wallsInfo);
}

Level* GameMaps::GetLevelFromSavedGame(std::ifstream& file) const
{
    uint8_t mapIndex = 0;
    file.read((char*)&mapIndex, sizeof(mapIndex));
    uint16_t mapWidth = 0;
    file.read((char*)&mapWidth, sizeof(mapWidth));
    uint16_t mapHeight = 0;
    file.read((char*)&mapHeight, sizeof(mapHeight));

    const uint16_t mapSize = mapWidth * mapHeight;
    uint16_t* plane0 = new uint16_t[mapSize];
    uint16_t* plane2 = new uint16_t[mapSize];

    file.read((char*)plane0, mapSize * sizeof(plane0[0]));
    file.read((char*)plane2, mapSize * sizeof(plane2[0]));
    uint32_t lightningStartTimestamp = 0;
    file.read((char*)&lightningStartTimestamp, sizeof(lightningStartTimestamp));
    Level* level = new Level(mapIndex, mapWidth, mapHeight, plane0, plane2, m_staticData.mapsInfo.at(mapIndex), m_staticData.wallsInfo);
    delete plane0;
    delete plane2;

    return level;
}

uint8_t GameMaps::GetNumberOfLevels() const
{
    return (uint8_t)m_staticData.mapsInfo.size();
}

uint32_t GameMaps::GetChunkSize(const uint16_t index)
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