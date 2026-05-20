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

#include "GameControllerHint.h"
#include "../Engine/EgaGraph.h"
#include "../Engine/IRenderer.h"

GameControllerHint::GameControllerHint(IRenderer& renderer, EgaGraph& egaGraph) :
    m_renderer(renderer),
    m_renderableText(*egaGraph.GetFont(3)),
    m_renderableTiles(*egaGraph.GetTilesSize8())
{
}

void GameControllerHint::DrawDifficultySelection()
{
    m_renderableTiles.Reset();
    m_renderableTiles.DrawWindow(28, 15, 9, 3);
    m_renderer.RenderTiles(m_renderableTiles);

    m_renderer.Render2DBar(224, 120, 72, 24, EgaLightGray);

    m_renderableText.Reset();
    m_renderableText.LeftAligned("(B)", EgaRed, 226, 122);
    m_renderableText.LeftAligned("Novice", EgaBrightWhite, 246, 122);
    m_renderableText.LeftAligned("(X)", EgaBlue, 226, 132);
    m_renderableText.LeftAligned("Warrior", EgaBrightWhite, 246, 132);
    m_renderer.RenderText(m_renderableText);
}
