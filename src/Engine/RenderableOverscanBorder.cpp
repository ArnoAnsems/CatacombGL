// Copyright (C) 2021 Arno Ansems
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

#include "RenderableOverscanBorder.h"

RenderableOverscanBorder::RenderableOverscanBorder(const OverscanBorder& overscanBorder) :
    m_overscanBorder(overscanBorder)
{

}

RenderableOverscanBorder::~RenderableOverscanBorder()
{
}

void RenderableOverscanBorder::Draw(IRenderer& renderer, const uint16_t additionalMargin, const uint32_t timeStamp)
{
    const egaColor color = m_overscanBorder.GetActiveColor(timeStamp);
    const uint16_t borderWidth = m_overscanBorder.GetBorderWidth();
    const uint16_t borderHeight = m_overscanBorder.GetBorderHeight();
    const int16_t offsetX = 0 - borderWidth - additionalMargin;
    const int16_t offsetY = 0 - borderHeight;
    const uint16_t totalWidth = 320 + (2 * (borderWidth + additionalMargin));

    // Top border
    renderer.Render2DBar(offsetX, offsetY, totalWidth, borderHeight, color);
    // Bottom border
    renderer.Render2DBar(offsetX, 200, totalWidth, borderHeight, color);
    // Left border
    renderer.Render2DBar(offsetX, 0, borderWidth, 200, color);
    // Right border
    renderer.Render2DBar(320 + additionalMargin, 0, borderWidth, 200, color);
}