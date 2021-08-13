// Copyright (C) 2021 Arno Ansems
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
// LevelStatistics
//
// Keeps track of the number of monsters, secrets and items in the level,
// which can be displayed on the automap.
//
#pragma once

#include "Level.h"

class LevelStatistics
{
public:
    LevelStatistics();
    ~LevelStatistics();

    void SetCountersAtStartOfLevel(const Level& level);

    uint32_t GetTotalMonsters() const;
    uint32_t GetTotalSecrets() const;
    uint32_t GetTotalItems() const;
    uint32_t GetMonstersKilled() const;
    uint32_t GetSecretsRevealed() const;
    uint32_t GetItemsTaken() const;

private:
    static void FloodFillSecret(const Level& level, bool* secretsMap, const uint16_t x, const uint16_t y);

    uint32_t m_totalMonsters;
    uint32_t m_totalSecrets;
    uint32_t m_totalItems;
    uint32_t m_remainingMonsters;
    uint32_t m_remainingSecrets;
    uint32_t m_remainingItems;
};
