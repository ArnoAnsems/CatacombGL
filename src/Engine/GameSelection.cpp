// Copyright (C) 2019 Arno Ansems
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
#pragma once

#include "GameSelection.h"
#include "DefaultFont.h"

GameSelection::GameSelection(IRenderer& renderer) :
    m_renderer(renderer)
{

}

GameSelection::~GameSelection()
{

}

void GameSelection::DrawBox(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height)
{
    m_renderer.Render2DBar(x, y, width, 1, EgaBrightWhite);
    m_renderer.Render2DBar(x + width - 1, y + 1, 1, height - 1, EgaBrightWhite);
    m_renderer.Render2DBar(x, y + 1, 1, height - 1, EgaDarkGray);
    m_renderer.Render2DBar(x + 1, y + height - 1, width - 2, 1, EgaDarkGray);

    m_renderer.Render2DBar(x + 1, y + 1, width - 2, 4, EgaLightGray);
    m_renderer.Render2DBar(x + 1, y + height - 5, width - 2, 4, EgaLightGray);
    m_renderer.Render2DBar(x + 1, y + 5, 4, height - 10, EgaLightGray);
    m_renderer.Render2DBar(x + width - 5, y + 5, 4, height - 10, EgaLightGray);

    m_renderer.Render2DBar(x + 5, y + 5, width - 10, 1, EgaBlue);
    m_renderer.Render2DBar(x + width - 6, y + 6, 1, height - 11, EgaBlue);
    m_renderer.Render2DBar(x + 5, y + 6, 1, height - 12, EgaBrightBlue);
    m_renderer.Render2DBar(x + 5, y + height - 6, width - 11, 1, EgaBrightBlue);

    m_renderer.Render2DBar(x + 6, y + 6, width - 12, height - 12, EgaDarkGray);
}

void GameSelection::Draw(const GameSelectionPresentation& presentation)
{
    m_renderer.Prepare3DRendering(false, 1.0f, 25);
    m_renderer.Prepare2DRendering(true);
    DrawBox(2, 2, 636, 80);
    const Font* defaultFont = DefaultFont::Get(m_renderer);
    for (uint8_t gameIndex = 0; gameIndex < presentation.gameList.size(); gameIndex++)
    {
        const char* gameName = presentation.gameList.at(gameIndex).first.c_str();
        m_renderer.RenderTextLeftAligned(gameName, defaultFont, EgaBrightWhite, 16, 15 + (15 * gameIndex));
        const GameDetectionState detectionState = presentation.gameList.at(gameIndex).second;
        const char* detectionStateStr =
            (detectionState == Detected) ? "Detected" :
            (detectionState == NotDetected) ? "Not Detected" :
            "Not Supported";
        const egaColor detectionStateColor =
            (detectionState == Detected) ? EgaGreen :
            (detectionState == NotDetected) ? EgaRed :
            EgaLightGray;
        m_renderer.RenderTextCentered(detectionStateStr, defaultFont, detectionStateColor, 300, 15 + (15 * gameIndex));
    }
    m_renderer.Unprepare2DRendering();
}
