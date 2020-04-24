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
        AlongYAxis
    };

    typedef struct
    {
        const Picture* picture;
        float offsetX;
        float offsetY;
        int32_t squaredDistance;
        SpriteOrientation orientation;
    } RenderableSprite;

    RenderableSprites(const float playerPosX, const float playerPosY);
    void AddSprite(const Picture* picture, const float offsetX, const float offsetY, const SpriteOrientation orientation);

    const std::vector<RenderableSprite>& GetSprites();

private:
    void QuickSort(uint16_t p, uint16_t q);
    uint16_t Partition(uint16_t p, uint16_t q);
    void Swap(uint16_t p, uint16_t q);

    std::vector<RenderableSprite> m_sprites;
    const float m_playerPosX;
    const float m_playerPosY;
};
