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

#include "AutoMap.h"
#include "LevelLocationNames.h"

AutoMap::AutoMap() :
    m_originX(0),
    m_originY(0),
    m_lastActionTimestamp(0),
    m_autoMapType(ActorAtView),
    m_cheat(false)
{

}

AutoMap::~AutoMap()
{

}

void AutoMap::SetCheat(const bool enabled)
{
    m_cheat = enabled;
}

void AutoMap::DrawClassic(IRenderer& renderer, EgaGraph& egaGraph, Level& level, const uint16_t additionalMargin)
{
    level.DrawAutoMap(renderer, egaGraph, additionalMargin, m_originX, m_originY, m_autoMapType, m_cheat);
}

void AutoMap::DrawIso(
    IRenderer& renderer,
    EgaGraph& egaGraph,
    Level& level,
    const float aspectRatio,
    const ViewPorts::ViewPortRect3D original3DViewArea)
{
    level.DrawAutoMapIso(renderer, egaGraph, aspectRatio, original3DViewArea, m_originX, m_originY, m_cheat);
}

void AutoMap::DrawTopDown(IRenderer& renderer,
    EgaGraph& egaGraph,
    Level& level,
    const float aspectRatio,
    const ViewPorts::ViewPortRect3D original3DViewArea)
{
    level.DrawAutoMapTopDown(renderer, egaGraph, aspectRatio, original3DViewArea, m_originX, m_originY, m_cheat);
}

void AutoMap::ProcessInput(PlayerInput& playerInput, Level& level, const uint32_t timestamp, const AutoMapMode autoMapMode)
{
    const uint16_t maxOriginX = level.GetLevelWidth() - 20;
    const uint16_t maxOriginY = (autoMapMode == TopDown) ? level.GetLevelHeight() - 7 : level.GetLevelHeight() - 9;
    if (timestamp > m_lastActionTimestamp + 200)
    {
        if (playerInput.IsKeyPressed(SDLK_RIGHT) && m_originX < maxOriginX)
        {
            m_originX++;
            m_lastActionTimestamp = timestamp;
        }
        if (playerInput.IsKeyPressed(SDLK_LEFT) && m_originX > 0)
        {
            m_originX--;
            m_lastActionTimestamp = timestamp;
        }
        if (playerInput.IsKeyPressed(SDLK_DOWN) && m_originY < maxOriginY)
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
            m_autoMapType = (AutoMapType)(m_autoMapType + 1);
            if (m_autoMapType == MaxAutoMapType)
            {
                m_autoMapType = MapView;
            }
        }
    }
}

void AutoMap::ResetOrigin(Level& level, const AutoMapMode autoMapMode)
{
    if (autoMapMode == ClassicDebug)
    {
        m_originX = 0;
        m_originY = 0;
    }
    else
    {
        // Put the origin at the player position
        const uint16_t maxOriginX = level.GetLevelWidth() - 20;
        const uint16_t maxOriginY = (autoMapMode == TopDown) ? level.GetLevelHeight() - 7 : level.GetLevelHeight() - 9;
        const int16_t bestOriginX = (int16_t)(level.GetPlayerActor()->GetX()) - 10;
        const int16_t bestOriginY = (int16_t)(level.GetPlayerActor()->GetY()) - 4;
        m_originX = (bestOriginX < 0) ? 0 :
            (bestOriginX > maxOriginX) ? maxOriginX : bestOriginX;
        m_originY = (bestOriginY < 0) ? 0 :
            (bestOriginY > maxOriginY) ? maxOriginY : bestOriginY;
    }
}
