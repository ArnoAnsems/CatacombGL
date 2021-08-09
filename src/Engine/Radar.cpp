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

#include "Radar.h"
#include "Actor.h"
#include <math.h>

enum gemTypes
{
    RedGem = 0,
    GreenGem = 1,
    BlueGem = 2,
    YellowGem = 3,
    PurpleGem = 4
};

Radar::Radar() :
    m_numberOfBlips(0),
    m_playerPosX(0.0f),
    m_playerPosY(0.0f)
{

}

Radar::~Radar()
{

}

void Radar::ResetRadar(const Actor* player, const PlayerInventory& playerInventory, const uint32_t timeStamp)
{
    m_playerPosX = player->GetX();
    m_playerPosY = player->GetY();
    m_playerAngle = player->GetAngle();
    m_numberOfBlips = 0;
    const egaColor flickeringColors[3] = {EgaRed, EgaBrightBlue, EgaBrightYellow};
    const int8_t flickeringColorIndex = (uint32_t)((timeStamp % 1000) / 14.3) % 3;
    m_flickeringColor = flickeringColors[flickeringColorIndex];
    m_gemPresent[BlueGem] = playerInventory.GetGem(BlueGem);
    m_gemPresent[RedGem] = playerInventory.GetGem(RedGem);
    m_gemPresent[YellowGem] = playerInventory.GetGem(YellowGem);
    m_gemPresent[GreenGem] = playerInventory.GetGem(GreenGem);
    m_gemPresent[PurpleGem] = playerInventory.GetGem(PurpleGem);
}

void Radar::AddActors(const Actor** actors, const uint16_t numberOfActors)
{
    for (uint16_t i = 0; i < numberOfActors; i++)
    {
        if (m_numberOfBlips == MaxBlips)
        {
            break;
        }

        const Actor* actor = actors[i];
        if (actor == nullptr)
        {
            continue;
        }

        if (!actor->IsActive() || actor->IsDead())
        {
            continue;
        }

        const actorRadarVisibility visibility = actor->GetDecorateActor().radarVisibility;
        if ((visibility == Never) ||
            (visibility == WithBlueGem && !m_gemPresent[BlueGem]) ||
            (visibility == WithRedGem && !m_gemPresent[RedGem]) ||
            (visibility == WithGreenGem && !m_gemPresent[GreenGem]) ||
            (visibility == WithPurpleGem && !m_gemPresent[PurpleGem]) ||
            (visibility == WithYellowGem && !m_gemPresent[YellowGem]) ||
            (visibility == WithAllGems && (!m_gemPresent[BlueGem] || !m_gemPresent[RedGem] || !m_gemPresent[YellowGem] || !m_gemPresent[GreenGem] || !m_gemPresent[PurpleGem]))
            )
        {
            continue;
        }

        const float psin = (float)sin(m_playerAngle * 3.14159265 / 180.0);
        const float pcos = (float)cos(m_playerAngle * 3.14159265 / 180.0);
        const float radarHalfWidth = 25.5f;

        const float xl = m_playerPosX - radarHalfWidth;
        const float xh = m_playerPosX + radarHalfWidth;
        const float yl = m_playerPosY - radarHalfWidth;
        const float yh = m_playerPosY + radarHalfWidth;

        const float oX = actor->GetX();
        const float oY = actor->GetY();

        if ((oX >= xl) && (oX <= xh) && (oY >= yl) && (oY <= yh))
        {
            const float dx = m_playerPosX - oX;
            const float dY = m_playerPosY - oY;
            const float oRadius = sqrt((dx * dx) + (dY * dY));
            const float radarRadius = 17.0f;

            if (oRadius < radarRadius)
            {
                m_radarBlips[m_numberOfBlips].offsetY =  - (dY * pcos) + (dx * psin);
                m_radarBlips[m_numberOfBlips].offsetX =  - (dY * psin) - (dx * pcos);
                m_radarBlips[m_numberOfBlips].color = (actor->GetDecorateActor().radarVisibility == Flickering) ? m_flickeringColor : actor->GetDecorateActor().radarColor;
                m_numberOfBlips++;
            }
        }
    }
}

radarBlip Radar::GetRadarBlip(const uint16_t index) const
{
    if (index >= 0 && index < m_numberOfBlips)
    {
        return m_radarBlips[index];
    }
    else
    {
        return radarBlip();
    }
}

uint16_t Radar::GetNumberOfBlips() const
{
    return m_numberOfBlips;
}
