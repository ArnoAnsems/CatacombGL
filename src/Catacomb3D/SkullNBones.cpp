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

#include "SkullNBones.h"
#include "EgaGraphCatacomb3D.h"

SkullNBones::SkullNBones() :
    m_playerScore(0),
    m_computerScore(0),
    m_playerX(148.0f),
    m_computerX(148.0f),
    m_timeStampOfCurrentFrame(0),
    m_timeStampOfPreviousFrame(0),
    m_playerMovesLeft(false),
    m_playerMovesRight(false)
{

}

void SkullNBones::ProcessInput(const PlayerInput& playerInput)
{
    m_playerMovesLeft = playerInput.IsKeyPressed(SDLK_LEFT);
    m_playerMovesRight = playerInput.IsKeyPressed(SDLK_RIGHT);
}

void SkullNBones::DrawScore(IRenderer& renderer, EgaGraph& egaGraph) const
{
    renderer.RenderTextLeftAligned("YOU:", egaGraph.GetFont(4), EgaBrightRed, 80, 52);
    const std::string playerScoreStr = std::to_string(m_playerScore);
    renderer.RenderTextLeftAligned(playerScoreStr.c_str(), egaGraph.GetFont(4), EgaBrightRed, 102, 52);
    renderer.RenderTextLeftAligned("COMP:", egaGraph.GetFont(4), EgaBrightRed, 182, 52);
    const std::string computerScoreStr = std::to_string(m_computerScore);
    renderer.RenderTextLeftAligned(computerScoreStr.c_str(), egaGraph.GetFont(4), EgaBrightRed, 218, 52);
}

void SkullNBones::Draw(IRenderer& renderer, EgaGraph& egaGraph, const uint32_t timeStamp)
{
    m_timeStampOfPreviousFrame = m_timeStampOfCurrentFrame;
    m_timeStampOfCurrentFrame = timeStamp;

    if (m_timeStampOfCurrentFrame > m_timeStampOfPreviousFrame)
    {
        UpdateFrame(m_timeStampOfCurrentFrame - m_timeStampOfPreviousFrame);
    }

    renderer.Render2DBar(77, 60, 154, 1, EgaBrightRed);
    renderer.Render2DBar(77, 143, 154, 1, EgaBrightRed);
    renderer.Render2DPicture(egaGraph.GetPicture(CP_PADDLEWARPIC), 130, 48);
    DrawScore(renderer, egaGraph);

    renderer.Render2DPicture(egaGraph.GetSprite(PADDLESPR), (uint16_t)m_computerX, 66);
    renderer.Render2DPicture(egaGraph.GetSprite(PADDLESPR), (uint16_t)m_playerX, 135);
    renderer.Render2DPicture(egaGraph.GetSprite(BALLSPR), 120, 100);
}

void SkullNBones::UpdateFrame(const uint32_t deltaTime)
{
    constexpr float distanceInPixelsPerTic = 2.0f;
    constexpr float distanceInPixelsPerMilliSec = distanceInPixelsPerTic * 60.0f / 1000.0f;
    const float distanceInFrame = deltaTime * distanceInPixelsPerMilliSec;
    if (m_playerMovesLeft && !m_playerMovesRight)
    {
        m_playerX -= distanceInFrame;
        if (m_playerX < 77.0f)
        {
            m_playerX = 77.0f;
        }
    }
    else if (!m_playerMovesLeft && m_playerMovesRight)
    {
        m_playerX += distanceInFrame;
        if (m_playerX > 219.0f)
        {
            m_playerX = 219.0f;
        }
    }
}
