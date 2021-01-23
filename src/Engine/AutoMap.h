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
#include "ConfigurationSettings.h"
#include "RenderableAutoMapIso.h"

class AutoMap
{
public:
    AutoMap();
    ~AutoMap();

    void SetCheat(const bool enabled);
    void DrawClassic(IRenderer& renderer, EgaGraph& egaGraph, Level& level, const uint16_t additionalMargin);
    void SetupIso(
        RenderableAutoMapIso& renderableAutoMapIso,
        EgaGraph& egaGraph,
        Level& level,
        const float aspectRatio);
    void SetupTopDown(
        RenderableAutoMapTopDown& rendererAutoMapTopDown,
        EgaGraph& egaGraph,
        Level& level,
        const float aspectRatio,
        const uint16_t tileSize,
        const uint16_t additionalMargin);
    void ProcessInput(
        const PlayerInput& playerInput,
        const float mouseSensitivity,
        Level& level,
        const uint32_t timestamp,
        const uint8_t autoMapMode,
        const ViewPorts::ViewPortRect3D original3DViewArea);
    void ResetOrigin(Level& level, const uint8_t autoMapMode);

private:
    float m_originX;
    float m_originY;
    uint32_t m_lastActionTimestamp;
    AutoMapType m_autoMapType;
    bool m_cheat;
    int32_t m_accumulatedMouseX;
    int32_t m_accumulatedMouseY;

};

