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
#include "LevelLocationNames.h"

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

void OverheadMap::DrawClassic(IRenderer& renderer, EgaGraph& egaGraph, Level& level, const uint16_t additionalMargin)
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

void OverheadMap::DrawTopDown(IRenderer& renderer,
    EgaGraph& egaGraph,
    Level& level,
    const float aspectRatio,
    const ViewPorts::ViewPortRect3D original3DViewArea)
{
    const uint16_t additionalMargin = renderer.GetAdditionalMarginDueToWideScreen(aspectRatio) * 4;
    renderer.PrepareTopDownRendering(aspectRatio, original3DViewArea, 4);
    level.DrawOverheadMapTopDown(renderer, egaGraph, additionalMargin, m_originX, m_originY);

    renderer.PrepareTopDownRendering(aspectRatio, original3DViewArea, 2);

    RenderableText locationNames(*egaGraph.GetFont(3));
    
    for (std::pair<uint8_t, locationNameBestPos> pair : m_locationNameBestPositions)
    {
        int16_t x = ((pair.second.x - m_originX) * 32) + 16;
        int16_t y = ((pair.second.y - m_originY) * 32);

        const std::string& locationMessage = egaGraph.GetWorldLocationNames(level.GetLevelIndex())->GetLocationName(pair.first);
        std::vector<std::string> subStrings;
        locationNames.SplitTextInTwo(locationMessage, subStrings);
        if (subStrings.size() == 1)
        {
            locationNames.Centered(subStrings.at(0), EgaBrightWhite, x, y + 11);
        }
        else
        {
            locationNames.Centered(subStrings.at(0), EgaBrightWhite, x, y + 6);
            locationNames.Centered(subStrings.at(1), EgaBrightWhite, x, y + 16);
        }

    }
    renderer.RenderText(locationNames);
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

void OverheadMap::UpdateLocationNamesBestPositions(Level& level)
{
    m_locationNameBestPositions.clear();

    for (uint16_t y = 0; y < level.GetLevelHeight(); y++)
    {
        for (uint16_t x = 0; x < level.GetLevelWidth(); x++)
        {
            const uint16_t wallTile = level.GetWallTile(x, y);
            if (wallTile > 180)
            {
                const uint8_t locationNameIndex = (uint8_t)(wallTile - 180);
                const uint16_t score = GetScore(level, x, y);
                const locationNameBestPos currentPos = { score, x, y };
                if (m_locationNameBestPositions.find(locationNameIndex) == m_locationNameBestPositions.end())
                {
                    m_locationNameBestPositions.insert(std::make_pair(locationNameIndex, currentPos));
                }
                else if (score > m_locationNameBestPositions.at(locationNameIndex).score)
                {
                    m_locationNameBestPositions.at(locationNameIndex) = currentPos;
                }
            }
        }
    }
}

uint16_t OverheadMap::GetScore(Level& level, uint16_t x, uint16_t y) const
{
    const uint16_t floorTile = level.GetWallTile(x, y);
    uint16_t tilesToTheLeft = 0;
    while (x - tilesToTheLeft > 0 && level.GetWallTile(x - tilesToTheLeft - 1, y) == floorTile)
    {
        tilesToTheLeft++;
    }
    uint16_t tilesToTheRight = 0;
    while (x + tilesToTheRight + 1 < level.GetLevelWidth() && level.GetWallTile(x + tilesToTheRight + 1, y) == floorTile)
    {
        tilesToTheRight++;
    }
    uint16_t tilesAbove = 0;
    while (y - tilesAbove > 0 && level.GetWallTile(x, y - tilesAbove - 1) == floorTile)
    {
        tilesAbove++;
    }
    uint16_t tilesBelow = 0;
    while (y + tilesBelow + 1 < level.GetLevelHeight() && level.GetWallTile(x, y + tilesBelow + 1) == floorTile)
    {
        tilesBelow++;
    }
    uint16_t verticalScore =
        (tilesToTheLeft > tilesToTheRight) ? (tilesToTheRight * 100) + 50 :
        (tilesToTheLeft < tilesToTheRight) ? (tilesToTheLeft * 100) + 50 :
        (tilesToTheLeft * 100);
    uint16_t horizontalScore =
        (tilesAbove > tilesBelow) ? (tilesBelow * 2) + 1 :
        (tilesAbove < tilesBelow) ? (tilesAbove * 2) + 1 :
        (tilesAbove * 2);

    return verticalScore + horizontalScore;
}