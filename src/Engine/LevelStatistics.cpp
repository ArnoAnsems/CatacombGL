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