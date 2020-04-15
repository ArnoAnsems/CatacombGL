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

// Supporting class for creating graphics out of the tiles with an 8 x 8 size.
#pragma once

#include "IRenderer.h"

class TilesSize8Utility
{
public:
    static void DrawWindow(
        std::vector<IRenderer::imageOnTextureAtlas>& outputImages,
        const int16_t x,
        const int16_t y,
        const uint16_t width,
        const uint16_t height);
    static void DrawDialog(
        std::vector<IRenderer::imageOnTextureAtlas>& outputImages,
        const int16_t x,
        const int16_t y,
        const uint16_t length);
    static void DrawListBullet(
        std::vector<IRenderer::imageOnTextureAtlas>& outputImages,
        const int16_t x,
        const int16_t y,
        const bool enable,
        const bool flash);
    static void DrawRadioButton(
        std::vector<IRenderer::imageOnTextureAtlas>& outputImages,
        const int16_t x,
        const int16_t y,
        const bool selected,
        const bool flash);

private:

};


