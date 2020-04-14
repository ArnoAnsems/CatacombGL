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

    outputImages.push_back({ sx, sy, 0 });
    outputImages.push_back({ sx, sy + sh, 6 });

    for (int16_t i = sx + 8; i <= sx + sw - 8; i += 8)
    {
        outputImages.push_back({ i, sy, 1 });
        outputImages.push_back({ i, sy + sh, 7 });
    }
    outputImages.push_back({ sx + sw, sy, 2 });
    outputImages.push_back({ sx + sw, sy + sh, 8 });

    for (int16_t i = sy + 8; i <= sy + sh - 8; i += 8)
    {
        outputImages.push_back({ sx, i, 3 });
        outputImages.push_back({ sx + sw, i, 5 });
    }
}