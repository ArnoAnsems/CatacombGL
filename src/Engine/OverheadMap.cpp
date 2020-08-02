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

#include "OverheadMap.h"

OverheadMap::OverheadMap() :
    m_originX(0),
    m_originY(0),
    m_lastActionTimestamp(0),
    m_overheadType(ActorAtView)
{

}

OverheadMap::~OverheadMap()
{

}

void OverheadMap::Draw(IRenderer& renderer, EgaGraph& egaGraph, Level& level, const uint16_t additionalMargin)
{
    level.DrawOverheadMap(renderer, egaGraph, additionalMargin, m_originX, m_originY, m_overheadType);
}

void OverheadMap::DrawIso(
    IRenderer& renderer,
    EgaGraph& egaGraph,
    Level& level,
    const float aspectRatio,
    const ViewPorts::ViewPortRect3D original3DViewArea)
{
    const uint16_t additionalMargin = renderer.GetAdditionalMarginDueToWideScreen(aspectRatio);
    renderer.PrepareIsoRendering(aspectRatio, original3DViewArea, (float)m_originX, (float)m_originY);
    level.DrawOverheadMapIso(renderer, egaGraph, additionalMargin);
}

void OverheadMap::ProcessInput(PlayerInput& playerInput, Level& level, const uint32_t timestamp)
{
    if (timestamp > m_lastActionTimestamp + 200)
    {
        if (playerInput.IsKeyPressed(SDLK_RIGHT) && m_originX + 20 < level.GetLevelWidth())
        {
            m_originX++;
            m_lastActionTimestamp = timestamp;
        }
        if (playerInput.IsKeyPressed(SDLK_LEFT) && m_originX > 0)
        {
            m_originX--;
            m_lastActionTimestamp = timestamp;
        }
        if (playerInput.IsKeyPressed(SDLK_DOWN) && m_originY + 9 < level.GetLevelHeight())
        {
            m_originY++;
            m_lastActionTimestamp = timestamp;
        }
        if (playerInput.IsKeyPressed(SDLK_UP) && m_originY > 0)
        {
            m_originY--;
            m_lastActionTimestamp = timestamp;
        }
        if (playerInput.IsKeyJustPressed(SDLK_LCTRL) || playerInput.IsKeyJustPressed(SDLK_RCTRL))
        {
            m_overheadType = (OverheadType)(m_overheadType + 1);
            if (m_overheadType == MaxOverheadType)
            {
                m_overheadType = MapView;
            }
        }
    }
}