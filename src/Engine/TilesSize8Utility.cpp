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

#include "TilesSize8Utility.h"

void TilesSize8Utility::DrawWindow(
    std::vector<IRenderer::imageOnTextureAtlas>& outputImages,
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

    outputImages.push_back({ sx, sy, TileIdWindowTopLeft });
    outputImages.push_back({ sx, sy + sh, TileIdWindowBottomLeft });

    for (int16_t i = sx + 8; i <= sx + sw - 8; i += 8)
    {
        outputImages.push_back({ i, sy, TileIdWindowTop });
        outputImages.push_back({ i, sy + sh, TileIdWindowBottom });
    }
    outputImages.push_back({ sx + sw, sy, TileIdWindowTopRight });
    outputImages.push_back({ sx + sw, sy + sh, TileIdWindowBottomRight });

    for (int16_t i = sy + 8; i <= sy + sh - 8; i += 8)
    {
        outputImages.push_back({ sx, i, TileIdWindowLeft });
        outputImages.push_back({ sx + sw, i, TileIdWindowRight });
    }
}

void TilesSize8Utility::DrawDialog(
    std::vector<IRenderer::imageOnTextureAtlas>& outputImages,
    const int16_t x,
    const int16_t y,
    const uint16_t length)
{
    // Based on CAL_DialogDraw in ID_CA.C
    outputImages.push_back({ x, y, TileIdWindowTopLeft });
    outputImages.push_back({ x, y + 8, TileIdWindowLeft });
    outputImages.push_back({ x, y + 16, TileIdWindowBottomLeft });
    outputImages.push_back({ x + (length - 1) * 8, y, TileIdWindowTopRight });
    outputImages.push_back({ x + (length - 1) * 8, y + 8, TileIdWindowRight });
    outputImages.push_back({ x + (length - 1) * 8, y + 16, TileIdWindowBottomRight });

    for (int16_t x2 = x + 8; x2 < x + (length - 1) * 8; x2 += 8)
    {
        outputImages.push_back({ x2, y, TileIdWindowTop });
        outputImages.push_back({ x2, y + 8, TileIdWindowCenter });
        outputImages.push_back({ x2, y + 16, TileIdWindowBottom });
    }
}

void TilesSize8Utility::DrawListBullet(
    std::vector<IRenderer::imageOnTextureAtlas>& outputImages,
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
    outputImages.push_back({ x, y, imageId });
}

void TilesSize8Utility::DrawRadioButton(
    std::vector<IRenderer::imageOnTextureAtlas>& outputImages,
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
    outputImages.push_back({ x, y, imageId });
}