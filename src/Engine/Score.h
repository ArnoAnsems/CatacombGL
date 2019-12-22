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

//
// Score
//
// Keeps track of the score for Catacomb 3D
//
#pragma once

#include <stdint.h>

class Score
{
public:
    Score();
    ~Score();

    const uint32_t GetPoints() const;
    void SetPoints(const uint32_t points);
    void AddPoints(const uint32_t points);
    bool Update(const uint32_t timestamp);
    void UpdateAll();

    void Reset();

private:
    uint32_t m_points;
    uint32_t m_pointsToAdd;
    uint32_t m_pointsAddedTimestamp;
};

