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
    m_totalMonsters = CountMonstersOnLevel(level);
    m_totalSecrets = CountSecretsOnLevel(level);
    m_totalItems = CountItemsOnLevel(level);

    m_remainingMonsters = m_totalMonsters;
    m_remainingSecrets = m_totalSecrets;
    m_remainingItems = m_totalItems;
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

    if (y > 1 && !secretsMap[(y - 1) * level.GetLevelWidth() + x] && level.IsExplosiveWall(x, y - 1))
    {
        FloodFillSecret(level, secretsMap, x, y - 1);
    }

    if (y < level.GetLevelHeight() - 2 && !secretsMap[(y + 1) * level.GetLevelWidth() + x] && level.IsExplosiveWall(x, y + 1))
    {
        FloodFillSecret(level, secretsMap, x, y + 1);
    }
}

void LevelStatistics::UpdateMonstersKilled(const Level& level)
{
    m_remainingMonsters = CountMonstersOnLevel(level);
}

void LevelStatistics::UpdateSecrets(const Level& level)
{
    m_remainingSecrets = CountSecretsOnLevel(level);
}

void LevelStatistics::UpdateItems(const Level& level)
{
    m_remainingItems = CountItemsOnLevel(level);
}

uint32_t LevelStatistics::CountMonstersOnLevel(const Level& level)
{
    uint32_t monsterCount = 0;
    for (uint16_t y = 1; y < level.GetLevelHeight() - 1; y++)
    {
        for (uint16_t x = 1; x < level.GetLevelWidth() - 1; x++)
        {
            const Actor* actor = level.GetBlockingActor(x, y);
            if (actor != nullptr)
            {
                if (actor->IsMonsterAndAlive())
                {
                    monsterCount++;
                }
            }
        }
    }

    return monsterCount;
}

uint32_t LevelStatistics::CountSecretsOnLevel(const Level& level)
{
    uint32_t secretsCount = 0;

    bool* secretsMap = new bool[level.GetLevelHeight() * level.GetLevelWidth()];
    std::memset(secretsMap, false, level.GetLevelHeight() * level.GetLevelWidth());

    for (uint16_t y = 1; y < level.GetLevelHeight() - 1; y++)
    {
        for (uint16_t x = 1; x < level.GetLevelWidth() - 1; x++)
        {
            if (!secretsMap[y * level.GetLevelWidth() + x])
            {
                if (level.IsExplosiveWall(x, y))
                {
                    secretsCount++;
                    FloodFillSecret(level, secretsMap, x, y);
                }
                else
                {
                    secretsMap[y * level.GetLevelWidth() + x] = true;
                }
            }
        }
    }

    delete[] secretsMap;

    return secretsCount;
}

uint32_t LevelStatistics::CountItemsOnLevel(const Level& level)
{
    uint32_t itemCount = 0;
    for (uint16_t y = 1; y < level.GetLevelHeight() - 1; y++)
    {
        for (uint16_t x = 1; x < level.GetLevelWidth() - 1; x++)
        {
            const Actor* actor = level.GetBlockingActor(x, y);
            if (actor != nullptr)
            {
                if (actor->IsItem())
                {
                    itemCount++;
                }
            }
        }
    }

    for (uint16_t i = 0; i < level.GetMaxNonBlockingActors(); i++)
    {
        const Actor* actor = level.GetNonBlockingActor(i);
        if (actor != nullptr)
        {
            if (actor->IsItem())
            {
                itemCount++;
            }
        }
    }

    return itemCount;
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