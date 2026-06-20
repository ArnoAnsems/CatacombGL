// Copyright (C) 2026 Arno Ansems
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

#include "GuiElementListCat3D.h"
#include "../Engine/IRenderer.h"
#include <cmath>

GuiElementListCat3D::GuiElementListCat3D(
    const PlayerInput& playerInput,
    const uint16_t maxElementsDrawn,
    const uint16_t elementHeight,
    const Picture* cursorPicture,
    const uint16_t soundWhenBrowsing) :
    GuiElementList(playerInput, maxElementsDrawn, elementHeight, cursorPicture, soundWhenBrowsing)
{
    m_scrollbarHeight = 65u;
    m_scrollbarWidth = 8u;
    m_scrollbarOffsetX = 150;
}

void GuiElementListCat3D::DrawScrollbar(IRenderer& renderer) const
{
    if (m_elements.size() > m_maxElementsDrawn)
    {
        const uint16_t scrollbarInnerHeight = m_scrollbarHeight - 6u;
        const int16_t scrollbarOriginX = m_originX + m_scrollbarOffsetX;
        const int16_t scrollbarOriginY = m_originY;

        // Top border
        renderer.Render2DBar(scrollbarOriginX, scrollbarOriginY, m_scrollbarWidth, 1u, EgaRed);

        // Right border
        renderer.Render2DBar(scrollbarOriginX + m_scrollbarWidth - 1u, scrollbarOriginY + 1u, 1u, m_scrollbarHeight - 1u, EgaRed);

        // Left border
        renderer.Render2DBar(scrollbarOriginX, scrollbarOriginY + 1u, 1u, m_scrollbarHeight - 1u, EgaRed);

        // Bottom border
        renderer.Render2DBar(scrollbarOriginX + 1u, scrollbarOriginY + m_scrollbarHeight - 1u, m_scrollbarWidth - 2u, 1u, EgaRed);

        const uint16_t visibleSectionHeight = std::ceil(static_cast<double>(scrollbarInnerHeight * m_maxElementsDrawn) / static_cast<double>(m_elements.size()));
        const uint16_t visibleSectionOffset = scrollbarInnerHeight * m_firstElementDrawn / m_elements.size();
        renderer.Render2DBar(scrollbarOriginX + 3u, scrollbarOriginY + 3u + visibleSectionOffset, m_scrollbarWidth - 6u, visibleSectionHeight, EgaRed);
    }
}
