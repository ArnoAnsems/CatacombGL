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

#include "LevelStatistics.h"

LevelStatistics::LevelStatistics() :
    m_totalMonsters(0),
    m_totalSecrets(0),
    m_totalItems(0),
    m_remainingMonsters(0),
    m_remainingSecrets(0),
    m_remainingItems(0)
{

}

LevelStatistics::~LevelStatistics()
{

}

void LevelStatistics::SetCountersAtStartOfLevel(const Level& level)
{
    m_totalMonsters = 0;
    m_totalSecrets = 0;
    m_totalItems = 0;

    bool* secretsMap = new bool[level.GetLevelHeight() * level.GetLevelWidth()];
    std::memset(secretsMap, false, level.GetLevelHeight() * level.GetLevelWidth());

    for (uint16_t y = 1; y < level.GetLevelHeight() - 1; y++)
    {
        for (uint16_t x = 1; x < level.GetLevelWidth() - 1; x++)
        {
            const Actor* actor = level.GetBlockingActor(x, y);
            if (actor != nullptr)
            {
                if (actor->IsMonsterAndAlive())
                {
                    m_totalMonsters++;
                }
                if (actor->IsItem())
                {
                    m_totalItems++;
                }
            }

            if (!secretsMap[y * level.GetLevelWidth() + x])
            {
                if (level.IsExplosiveWall(x, y))
                {
                    m_totalSecrets++;
                    FloodFillSecret(level, secretsMap, x, y);
                }
                else
                {
                    secretsMap[y * level.GetLevelWidth() + x] = true;
                }
            }
        }
    }

    m_remainingMonsters = m_totalMonsters;
    m_remainingSecrets = m_totalSecrets;
    m_remainingItems = m_totalItems;

    delete[] secretsMap;
}

void LevelStatistics::FloodFillSecret(const Level& level, bool* secretsMap, const uint16_t x, const uint16_t y)
{
    secretsMap[y * level.GetLevelWidth() + x] = true;
    if (x > 1 && !secretsMap[y * level.GetLevelWidth() + x - 1] && level.IsExplosiveWall(x - 1, y))
    {
        FloodFillSecret(level, secretsMap, x - 1, y);
    }

    if (x < level.GetLevelWidth() - 2 && !secretsMap[y * level.GetLevelWidth() + x + 1] && level.IsExplosiveWall(x + 1, y))
    {
        FloodFillSecret(level, secretsMap, x + 1, y);
    }

    if (y < level.GetLevelHeight() - 2 && !secretsMap[(y + 1) * level.GetLevelWidth() + x] && level.IsExplosiveWall(x, y + 1))
    {
        FloodFillSecret(level, secretsMap, x, y + 1);
    }
}

uint32_t LevelStatistics::GetTotalMonsters() const
{
    return m_totalMonsters;
}

uint32_t LevelStatistics::GetTotalSecrets() const
{
    return m_totalSecrets;
}

uint32_t LevelStatistics::GetTotalItems() const
{
    return m_totalItems;
}

uint32_t LevelStatistics::GetMonstersKilled() const
{
    return m_totalMonsters - m_remainingMonsters;
}

uint32_t LevelStatistics::GetSecretsRevealed() const
{
    return m_totalSecrets - m_remainingSecrets;
}

uint32_t LevelStatistics::GetItemsTaken() const
{
    return m_totalItems - m_remainingItems;
}