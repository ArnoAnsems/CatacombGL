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
    m_originX(0.0f),
    m_originY(0.0f),
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
    level.DrawAutoMap(renderer, egaGraph, additionalMargin, (uint16_t)m_originX, (uint16_t)m_originY, m_autoMapType, m_cheat);
}

void AutoMap::SetupIso(
    RenderableAutoMapIso& renderableAutoMapIso,
    EgaGraph& egaGraph,
    Level& level,
    const float aspectRatio)
{
    level.SetupAutoMapIso(renderableAutoMapIso, egaGraph, aspectRatio, m_originX, m_originY, m_cheat);
}

void AutoMap::SetupTopDown(
    RenderableAutoMapTopDown& rendererAutoMapTopDown,
    EgaGraph& egaGraph,
    Level& level,
    const float aspectRatio,
    const uint16_t tileSize,
    const uint16_t additionalMargin)
{
    level.SetupAutoMapTopDown(rendererAutoMapTopDown, egaGraph, aspectRatio, tileSize, additionalMargin, m_originX, m_originY, m_cheat);
}

void AutoMap::ProcessInput(PlayerInput& playerInput, Level& level, const uint32_t timestamp, const AutoMapMode autoMapMode)
{
    const float maxOriginX = level.GetLevelWidth() - 20.0f;
    const float maxOriginY = (autoMapMode == TopDown || autoMapMode == TopDownHD) ? level.GetLevelHeight() - 7.0f : level.GetLevelHeight() - 9.0f;
    const float stepSize = (autoMapMode == ClassicDebug) ? 1.0f : 0.125f;
    const uint32_t timeInterval = (autoMapMode == ClassicDebug) ? 200 : 25;
    if (timestamp > m_lastActionTimestamp + timeInterval)
    {
        if (playerInput.IsKeyPressed(SDLK_RIGHT))
        {
            m_originX += stepSize;
            if (autoMapMode == Isometric)
            {
                m_originY -= stepSize;
            }
            m_lastActionTimestamp = timestamp;
        }
        if (playerInput.IsKeyPressed(SDLK_LEFT))
        {
            m_originX -= stepSize;
            if (autoMapMode == Isometric)
            {
                m_originY += stepSize;
            }
            m_lastActionTimestamp = timestamp;
        }
        if (playerInput.IsKeyPressed(SDLK_DOWN))
        {
            m_originY += stepSize;
            if (autoMapMode == Isometric)
            {
                m_originX += stepSize;
            }
            m_lastActionTimestamp = timestamp;
        }
        if (playerInput.IsKeyPressed(SDLK_UP))
        {
            m_originY -= stepSize;
            if (autoMapMode == Isometric)
            {
                m_originX -= stepSize;
            }
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
        if (m_originX < 0.0f)
        {
            m_originX = 0.0f;
        }
        if (m_originX > maxOriginX)
        {
            m_originX = maxOriginX;
        }
        if (m_originY < 0.0f)
        {
            m_originY = 0.0f;
        }
        if (m_originY > maxOriginY)
        {
            m_originY = maxOriginY;
        }
    }
}

void AutoMap::ResetOrigin(Level& level, const AutoMapMode autoMapMode)
{
    if (autoMapMode == ClassicDebug)
    {
        m_originX = 0.0f;
        m_originY = 0.0f;
    }
    else
    {
        // Put the origin at the player position
        const float maxOriginX = level.GetLevelWidth() - 20.0f;
        const float maxOriginY = (autoMapMode == TopDown) ? level.GetLevelHeight() - 7.0f : level.GetLevelHeight() - 9.0f;
        const float bestOriginX = level.GetPlayerActor()->GetX() - 10.0f;
        const float bestOriginY = level.GetPlayerActor()->GetY() - 4.0f;
        m_originX = (bestOriginX < 0.0f) ? 0.0f :
            (bestOriginX > maxOriginX) ? maxOriginX : bestOriginX;
        m_originY = (bestOriginY < 0.0f) ? 0.0f :
            (bestOriginY > maxOriginY) ? maxOriginY : bestOriginY;
    }
}
