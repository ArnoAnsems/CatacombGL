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
#include "SavedGameInDosFormat.h"

static const uint16_t MaxMapWidth = 128;
static const uint16_t MaxMapHeight = 128;

GameMaps::GameMaps(const gameMapsStaticData& staticData, const std::string& path) :
    m_staticData(staticData)
{
    Logging::Instance().AddLogMessage("Loading " + m_staticData.filename);

    // Read the entire GameMaps file into memory
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
}

GameMaps::~GameMaps()
{
    delete m_rawData;
}

Level* GameMaps::GetLevelFromStart(const uint8_t mapIndex) const
{
    Logging::Instance().AddLogMessage("Loading map " + std::to_string(mapIndex) + " from start");

    const uint16_t rlewTag = 0xABCD;    
    const uint8_t* headerStart = m_rawData->GetChunk() + m_staticData.offsets.at(mapIndex);
    const uint32_t plane0Offset = *(uint32_t*)(headerStart);
    const uint32_t plane2Offset = *(uint32_t*)(headerStart + 8);
    const uint16_t plane0Length = *(uint16_t*)(headerStart + 12);
    const uint16_t plane2Length = *(uint16_t*)(headerStart + 16);

    // Sanity check on plane 0 and 2
    if (plane0Offset + plane0Length > m_rawData->GetSize())
    {
        Logging::Instance().FatalError("Corrupt plane 0 info for level " + std::to_string(mapIndex) + " in " + m_staticData.filename +
            " (plane0Offset: " + std::to_string(plane0Offset) +
            " ,plane0Length: " + std::to_string(plane0Length) +
            " ,total size: " + std::to_string(m_rawData->GetSize()) + ")");
    }
    if (plane2Offset + plane2Length > m_rawData->GetSize())
    {
        Logging::Instance().FatalError("Corrupt plane 2 info for level " + std::to_string(mapIndex) + " in " + m_staticData.filename +
            " (plane2Offset: " + std::to_string(plane0Offset) +
            " ,plane2Length: " + std::to_string(plane0Length) +
            " ,total size: " + std::to_string(m_rawData->GetSize()) + ")");
    }

    const uint16_t mapWidth = *(uint16_t*)(&(headerStart[18]));
    const uint16_t mapHeight = *(uint16_t*)(&(headerStart[20]));

    // Sanity check on map width and height
    if (mapWidth > MaxMapWidth)
    {
        Logging::Instance().FatalError("Map width (" + std::to_string(mapWidth) + ") too large for level " + std::to_string(mapIndex) + " in " + m_staticData.filename);
    }
    if (mapHeight > MaxMapHeight)
    {
        Logging::Instance().FatalError("Map height (" + std::to_string(mapHeight) + ") too large for level " + std::to_string(mapIndex) + " in " + m_staticData.filename);
    }

    uint8_t* plane0Source = &(m_rawData->GetChunk()[plane0Offset]);
    FileChunk* carmackExpandedChunk = Decompressor::CarmackExpand(plane0Source);
    FileChunk* decompressedPlane0 = Decompressor::RLEW_Decompress(carmackExpandedChunk->GetChunk(), rlewTag);

    if ((decompressedPlane0->GetSize() / sizeof(uint16_t)) < (uint32_t)mapWidth * (uint32_t)mapHeight)
    {
        Logging::Instance().FatalError("Plane 0 of level " + std::to_string(mapIndex) + " in " + m_staticData.filename +
            " is " + std::to_string(decompressedPlane0->GetSize()) + " bytes in size, which is too small for a level with a width of " +
            std::to_string(mapWidth) + " and a height of " + std::to_string(mapHeight));
    }

    uint8_t* plane2Source = &(m_rawData->GetChunk()[plane2Offset]);
    FileChunk* carmackExpandedChunk2 = Decompressor::CarmackExpand(plane2Source);
    FileChunk* decompressedPlane2 = Decompressor::RLEW_Decompress(carmackExpandedChunk2->GetChunk(), rlewTag);

    if ((decompressedPlane2->GetSize() / sizeof(uint16_t)) < (uint32_t)mapWidth * (uint32_t)mapHeight)
    {
        Logging::Instance().FatalError("Plane 2 of level " + std::to_string(mapIndex) + " in " + m_staticData.filename +
            " is " + std::to_string(decompressedPlane2->GetSize()) + " bytes in size, which is too small for a level with a width of " +
            std::to_string(mapWidth) + " and a height of " + std::to_string(mapHeight));
    }

    Level* level = new Level(mapIndex, mapWidth, mapHeight, (uint16_t*)(decompressedPlane0->GetChunk()), (uint16_t*)(decompressedPlane2->GetChunk()), m_staticData.mapsInfo.at(mapIndex), m_staticData.wallsInfo);

    delete carmackExpandedChunk;
    delete decompressedPlane0;
    delete carmackExpandedChunk2;
    delete decompressedPlane2;

    return level;
}

Level* GameMaps::GetLevelFromSavedGame(std::ifstream& file) const
{
    uint8_t mapIndex = 0;
    file.read((char*)&mapIndex, sizeof(mapIndex));

    if (file.fail())
    {
        Logging::Instance().FatalError("Failed to read level index from saved game");
    }

    const uint8_t numberOfLevels = GetNumberOfLevels();
    if (mapIndex >= numberOfLevels)
    {
        Logging::Instance().FatalError("Read level index " + std::to_string(mapIndex) + " from saved game, but " +
            m_staticData.filename + " contains only " + std::to_string(numberOfLevels) + " levels");
    }

    Logging::Instance().AddLogMessage("Loading map " + std::to_string(mapIndex) + " from saved game");

    uint16_t mapWidth = 0;
    file.read((char*)&mapWidth, sizeof(mapWidth));
    uint16_t mapHeight = 0;
    file.read((char*)&mapHeight, sizeof(mapHeight));

    if (file.fail())
    {
        Logging::Instance().FatalError("Failed to read level width and height from saved game");
    }

    // Sanity check on map width and height
    if (mapWidth > MaxMapWidth)
    {
        Logging::Instance().FatalError("Map width (" + std::to_string(mapWidth) + ") too large for saved game");
    }
    if (mapHeight > MaxMapHeight)
    {
        Logging::Instance().FatalError("Map height (" + std::to_string(mapHeight) + ") too large for saved game");
    }

    const uint16_t mapSize = mapWidth * mapHeight;
    uint16_t* plane0 = new uint16_t[mapSize];
    uint16_t* plane2 = new uint16_t[mapSize];

    file.read((char*)plane0, mapSize * sizeof(plane0[0]));
    file.read((char*)plane2, mapSize * sizeof(plane2[0]));

    if (file.fail())
    {
        Logging::Instance().FatalError("Failed to read plane info from saved game");
    }

    uint32_t lightningStartTimestamp = 0;
    file.read((char*)&lightningStartTimestamp, sizeof(lightningStartTimestamp));

    if (file.fail())
    {
        Logging::Instance().FatalError("Failed to lightningStartTimestamp from saved game");
    }

    Level* level = new Level(mapIndex, mapWidth, mapHeight, plane0, plane2, m_staticData.mapsInfo.at(mapIndex), m_staticData.wallsInfo);
    delete[] plane0;
    delete[] plane2;

    return level;
}

Level* GameMaps::GetLevelFromDosSavedGame(const SavedGameInDosFormat* savedGameInDosFormat) const
{
    Logging::Instance().AddLogMessage("Loading map " + savedGameInDosFormat->GetName() + " from DOS saved game");

    const uint8_t mapIndex = (uint8_t)savedGameInDosFormat->GetMapOn();

    // The dimensions of the map are retrieved from the GameMaps data
    const uint8_t* headerStart = m_rawData->GetChunk() + m_staticData.offsets.at(mapIndex);
    const uint16_t mapWidth = *(uint16_t*)(&(headerStart[18]));
    const uint16_t mapHeight = *(uint16_t*)(&(headerStart[20]));

    const uint16_t* plane0 = (const uint16_t*)savedGameInDosFormat->GetPlane0()->GetChunk();
    const uint16_t* plane2 = (const uint16_t*)savedGameInDosFormat->GetPlane2()->GetChunk();

    Level* level = new Level(mapIndex, mapWidth, mapHeight, plane0, plane2, m_staticData.mapsInfo.at(mapIndex), m_staticData.wallsInfo);

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

uint16_t GameMaps::GetTileWallExplosion(const bool isWaterLevel) const
{
    return isWaterLevel ? m_staticData.tileWaterExplosion : m_staticData.tileWallExplosion;
}