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

//
// SkullNBones
//
#pragma once

#include "..\Engine\IRenderer.h"
#include "..\Engine\EgaGraph.h"
#include "..\Engine\PlayerInput.h"

class SkullNBones
{
public:
    SkullNBones();
    void ProcessInput(const PlayerInput& playerInput);
    void Draw(IRenderer& renderer, EgaGraph& egaGraph, const uint32_t timeStamp);

private:
    void DrawScore(IRenderer& renderer, EgaGraph& egaGraph) const;
    void UpdateFrame();
    void ResetForNextSkull();

    uint16_t m_playerScore;
    uint16_t m_computerScore;

    float m_playerX;
    float m_computerX;
    float m_skullX;
    float m_skullY;
    int32_t m_skullDeltaX;
    int32_t m_skullDeltaY;
    uint32_t m_timeStampOfCurrentFrame;
    uint32_t m_timeStampOfPreviousFrame;
    uint32_t m_timeStampOfComputer;
    bool m_playerMovesLeft;
    bool m_playerMovesRight;
    int16_t m_speedup;
};
