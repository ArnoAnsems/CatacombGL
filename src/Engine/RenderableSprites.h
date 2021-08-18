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

//
// RenderableSprites
//
// Converts sprites into a format that can be processed by the renderer.
//
#pragma once

#include "Picture.h"
#include <vector>
#include <string>

class RenderableSprites
{
public:
    enum SpriteOrientation
    {
        RotatedTowardsPlayer,
        AlongXAxis,
        AlongYAxis,
        Isometric
    };

    typedef struct
    {
        const Picture* picture;
        float offsetX;
        float offsetY;
        float distanceToPlayerViewScreen;
        SpriteOrientation orientation;
    } RenderableSprite;

    RenderableSprites();
    void Reset(const float playerPosX, const float playerPosY, const float angle);
    void AddSprite(const Picture* picture, const float offsetX, const float offsetY, const SpriteOrientation orientation);
    void SortSpritesBackToFront();

    const std::vector<RenderableSprite>& GetSprites() const;
    const float GetAngle() const;

private:
    void QuickSort(uint16_t p, uint16_t q);
    uint16_t Partition(uint16_t p, uint16_t q);
    void Swap(uint16_t p, uint16_t q);

    std::vector<RenderableSprite> m_sprites;
    float m_playerPosX;
    float m_playerPosY;
    float m_angle;
    float m_pointInPlayerViewScreenX;
    float m_pointInPlayerViewScreenY;
};
