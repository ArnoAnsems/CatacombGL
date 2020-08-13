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
#pragma once

#include "Logging.h"
#include "IRenderer.h"
#include "PlayerInput.h"
#include "EgaGraph.h"
#include "Level.h"

class OverheadMap
{
public:
    OverheadMap();
    ~OverheadMap();

    void DrawClassic(IRenderer& renderer, EgaGraph& egaGraph, Level& level, const uint16_t additionalMargin);
    void DrawIso(
        IRenderer& renderer,
        EgaGraph& egaGraph,
        Level& level,
        const float aspectRatio,
        const ViewPorts::ViewPortRect3D original3DViewArea);
    void DrawTopDown(IRenderer& renderer,
        EgaGraph& egaGraph,
        Level& level,
        const float aspectRatio,
        const ViewPorts::ViewPortRect3D original3DViewArea);
    void ProcessInput(PlayerInput& playerInput, Level& level, const uint32_t timestamp);
    void UpdateLocationNamesBestPositions(Level& level);

private:
    struct locationNameBestPos
    {
        uint16_t score;
        uint16_t x;
        uint16_t y;
    };

    uint16_t GetScore(Level& level, uint16_t x, uint16_t y) const;

    uint16_t m_originX;
    uint16_t m_originY;
    uint32_t m_lastActionTimestamp;
    OverheadType m_overheadType;
    std::map<uint8_t, locationNameBestPos> m_locationNameBestPositions;
};

