// Copyright (C) 2019 Arno Ansems
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

#include "Score.h"

Score::Score() :
    m_points(0),
    m_pointsToAdd(0),
    m_pointsAddedTimestamp(0)
{

}

Score::~Score()
{

}

const uint32_t Score::GetPoints() const
{
    return m_points;
}

void Score::SetPoints(const uint32_t points)
{
    m_points = points;
    m_pointsToAdd = 0;
    m_pointsAddedTimestamp = 0;
}

void Score::AddPoints(const uint32_t points)
{
    m_pointsToAdd += points;
}

bool Score::Update(const uint32_t timestamp)
{
    if (m_pointsToAdd > 0 && timestamp - m_pointsAddedTimestamp > 100)
    {
        const int32_t addedPoints =
            (m_pointsToAdd > 1000) ? 1000 :
            (m_pointsToAdd > 100) ? 100 :
            (m_pointsToAdd < 20) ? m_pointsToAdd : 20;
        m_points += addedPoints;
        m_pointsToAdd -= addedPoints;
        m_pointsAddedTimestamp = timestamp;
        return true;
    }

    return false;
}

void Score::UpdateAll()
{
    m_points += m_pointsToAdd;
    m_pointsToAdd = 0;
}

void Score::Reset()
{
    m_points = 0;
    m_pointsToAdd = 0;
    m_pointsAddedTimestamp = 0;
}
