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
// Radar
//
// Visualizes the positions of nearby actors as small blips on the crystal ball.
//
#pragma once

#include "EgaColor.h"
#include <stdint.h>
#include "PlayerInventory.h"

class Actor;

typedef struct radarBlipStruct
{
    float offsetX;
    float offsetY;
    egaColor color;
} radarBlip;

static const uint16_t MaxBlips = 60;

class Radar
{

public:
    Radar();
    ~Radar();

    void ResetRadar(const Actor* player, const PlayerInventory& playerInventory, const uint32_t timeStamp);
    void AddActors(const Actor** actors, const uint16_t numberOfActors);
    radarBlip GetRadarBlip(const uint16_t index) const;
    uint16_t GetNumberOfBlips() const;

private:
    radarBlip m_radarBlips[MaxBlips];
    uint16_t m_numberOfBlips;
    float m_playerPosX;
    float m_playerPosY;
    float m_playerAngle;
    egaColor m_flickeringColor;
    bool m_gemPresent[5];
};
