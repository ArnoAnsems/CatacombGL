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

//
// GameMaps
//
// Class for reading map data from a GAMEMAPS file.
//
#pragma once

#include <stdint.h>
#include <vector>
#include "FileChunk.h"
#include "Level.h"
#include "Logging.h"

const uint16_t wallSolid = 1;
const uint16_t wallSpecial = 2;
const uint16_t wallRequiresKey = 4;

typedef struct gameMapsStaticData
{
    std::string filename;
    std::vector<int32_t> offsets;
    std::vector<LevelInfo> mapsInfo;
    std::vector<WallInfo> wallsInfo;
    uint16_t tileWallExplosion;
    uint16_t tileWaterExplosion;
} gameMapsStaticData;

class GameMaps
{
public:
    GameMaps(const gameMapsStaticData& staticData, const std::string& path, Logging* logging);
    ~GameMaps();

    Level* GetLevelFromStart(const uint8_t mapIndex) const;
    Level* GetLevelFromSavedGame(std::ifstream& file) const;
    uint8_t GetNumberOfLevels() const;
    uint16_t GetTileWallExplosion(const bool isWaterLevel) const;

private:
    uint32_t GetChunkSize(const uint16_t index);

    const gameMapsStaticData& m_staticData;
    FileChunk* m_rawData;
    Logging* m_logging;
};

