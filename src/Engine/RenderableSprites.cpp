// Copyright (C) 2020 Arno Ansems
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

#include "RenderableSprites.h"
#include <cmath>

const uint8_t MaxSpritesToRender = 100;

RenderableSprites::RenderableSprites() :
    m_playerPosX(1.0f),
    m_playerPosY(1.0f),
    m_angle(0.0f),
    m_sprites()
{

}

void RenderableSprites::Reset(const float playerPosX, const float playerPosY, const float angle)
{
    m_playerPosX = playerPosX;
    m_playerPosY = playerPosY;
    m_angle = angle;

    // Determine a second point inside the player view screen, at a distance of 1.0 from the player position.
    // This helps later on in calculating the distance between the player view screen and each of the sprites.
    m_pointInPlayerViewScreenX = m_playerPosX - (float)std::sin((m_angle + 90.f) * 3.14159265f / 180.0f);
    m_pointInPlayerViewScreenY = m_playerPosY + (float)std::cos((m_angle + 90.f) * 3.14159265f / 180.0f);

    m_sprites.clear();
}

void RenderableSprites::AddSprite(const Picture* picture, const float offsetX, const float offsetY, const SpriteOrientation orientation)
{
    if (m_sprites.size() == MaxSpritesToRender)
    {
        return;
    }

    const float distance =
        std::abs(
            ((m_pointInPlayerViewScreenX - m_playerPosX) * (m_playerPosY - offsetY)) -
            ((m_playerPosX - offsetX) * (m_pointInPlayerViewScreenY - m_playerPosY))
        );
    m_sprites.push_back({ picture, offsetX, offsetY, distance, orientation });
}

void RenderableSprites::SortSpritesBackToFront()
{
    QuickSort(0, (uint16_t)(m_sprites.size()));
}

const std::vector<RenderableSprites::RenderableSprite>& RenderableSprites::GetSprites() const
{
    return m_sprites;
}

const float RenderableSprites::GetAngle() const
{
    return m_angle;
}

void RenderableSprites::QuickSort(uint16_t p, uint16_t q)
{
    if (p < q)
    {
        int r = Partition(p, q);
        QuickSort(p, r);
        QuickSort(r + 1, q);
    }
}


uint16_t RenderableSprites::Partition(uint16_t p, uint16_t q)
{
    const float x = m_sprites.at(p).distanceToPlayerViewScreen;
    uint16_t i = p;

    for (uint16_t j = p + 1; j < q; j++)
    {
        if (m_sprites.at(j).distanceToPlayerViewScreen >= x)
        {
            i = i + 1;
            Swap(i, j);
        }
    }

    Swap(i, p);
    return i;
}

void RenderableSprites::Swap(uint16_t p, uint16_t q)
{
    std::swap(m_sprites.at(p).picture, m_sprites.at(q).picture);
    std::swap(m_sprites.at(p).offsetX, m_sprites.at(q).offsetX);
    std::swap(m_sprites.at(p).offsetY, m_sprites.at(q).offsetY);
    std::swap(m_sprites.at(p).distanceToPlayerViewScreen, m_sprites.at(q).distanceToPlayerViewScreen);
    std::swap(m_sprites.at(p).orientation, m_sprites.at(q).orientation);
}
