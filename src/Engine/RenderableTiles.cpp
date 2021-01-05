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

#include "RenderableTiles.h"
#include <string>

RenderableTiles::RenderableTiles(const TextureAtlas& textureAtlas) :
    m_textureAtlas(textureAtlas),
    m_tiles()
{
}

const std::vector<RenderableTiles::RenderableTile>& RenderableTiles::GetTiles() const
{
    return m_tiles;
}

const TextureAtlas& RenderableTiles::GetTextureAtlas() const
{
    return m_textureAtlas;
}

void RenderableTiles::Reset()
{
    m_tiles.clear();
}

void RenderableTiles::Add(
    const int16_t x,
    const int16_t y,
    const uint16_t tileId)
{
    m_tiles.push_back({ x, y, tileId });
}

void RenderableTiles::DrawWindow(
    const int16_t x,
    const int16_t y,
    const uint16_t width,
    const uint16_t height)
{
    // Based on US_DrawWindow in ID_US.C of the Catacomb Abyss source code.
    const int16_t sx = (x - 1) * 8;
    const int16_t sy = (y - 1) * 8;
    const int16_t sw = (width + 1) * 8;
    const int16_t sh = (height + 1) * 8;

    m_tiles.push_back({ sx, sy, TileIdWindowTopLeft });
    m_tiles.push_back({ sx, sy + sh, TileIdWindowBottomLeft });

    for (int16_t i = sx + 8; i <= sx + sw - 8; i += 8)
    {
        m_tiles.push_back({ i, sy, TileIdWindowTop });
        m_tiles.push_back({ i, sy + sh, TileIdWindowBottom });
    }
    m_tiles.push_back({ sx + sw, sy, TileIdWindowTopRight });
    m_tiles.push_back({ sx + sw, sy + sh, TileIdWindowBottomRight });

    for (int16_t i = sy + 8; i <= sy + sh - 8; i += 8)
    {
        m_tiles.push_back({ sx, i, TileIdWindowLeft });
        m_tiles.push_back({ sx + sw, i, TileIdWindowRight });
    }
}

void RenderableTiles::DrawDialog(
    const int16_t x,
    const int16_t y,
    const uint16_t length)
{
    // Based on CAL_DialogDraw in ID_CA.C
    m_tiles.push_back({ x, y, TileIdWindowTopLeft });
    m_tiles.push_back({ x, y + 8, TileIdWindowLeft });
    m_tiles.push_back({ x, y + 16, TileIdWindowBottomLeft });
    m_tiles.push_back({ x + (length - 1) * 8, y, TileIdWindowTopRight });
    m_tiles.push_back({ x + (length - 1) * 8, y + 8, TileIdWindowRight });
    m_tiles.push_back({ x + (length - 1) * 8, y + 16, TileIdWindowBottomRight });

    for (int16_t x2 = x + 8; x2 < x + (length - 1) * 8; x2 += 8)
    {
        m_tiles.push_back({ x2, y, TileIdWindowTop });
        m_tiles.push_back({ x2, y + 8, TileIdWindowCenter });
        m_tiles.push_back({ x2, y + 16, TileIdWindowBottom });
    }
}

void RenderableTiles::DrawListBullet(
    const int16_t x,
    const int16_t y,
    const bool enable,
    const bool flash)
{
    const uint16_t imageId =
        (enable && flash) ? TileIdListBulletEnabledFlashing :
        (enable && !flash) ? TileIdListBulletEnabled :
        (!enable && flash) ? TileIdListBulletDisabledFlashing :
        TileIdListBulletDisabled;
    m_tiles.push_back({ x, y, imageId });
}

void RenderableTiles::DrawRadioButton(
    const int16_t x,
    const int16_t y,
    const bool selected,
    const bool flash)
{
    const uint16_t imageId =
        (selected && flash) ? TileIdListBulletEnabledFlashing :
        (selected && !flash) ? TileIdListBulletEnabled :
        (!selected && flash) ? TileIdListRadioButtonNotSelectedFlashing :
        TileIdListRadioButtonNotSelected;
    m_tiles.push_back({ x, y, imageId });
}

void RenderableTiles::DrawNumberRightAligned(
    const int16_t x,
    const int16_t y,
    const int16_t number)
{
    const std::string str = std::to_string(number);
    const int16_t length = (int16_t)str.length();
    int16_t tilex = x - (8 * (int16_t)length);
    for (int16_t i = 0; i < length; i++)
    {
        m_tiles.push_back({ tilex, y, (uint16_t)(RenderableTiles::TileIdFirstNumber + str.at(i) - '0') });
        tilex += 8;
    }
}