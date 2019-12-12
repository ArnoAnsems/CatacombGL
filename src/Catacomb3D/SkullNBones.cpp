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

const float SkullMinX = 76.0f;
const float SkullMaxX = 228.0f;
const float SkullMinY = 62.0f;
const float SkullMaxY = 137.0f;

SkullNBones::SkullNBones() :
    m_playerScore(0),
    m_computerScore(0),
    m_playerX(148.0f),
    m_computerX(148.0f),
    m_skullX(0.0f),
    m_skullY(0.0f),
    m_skullDeltaX(0),
    m_skullDeltaY(0),
    m_timeStampOfCurrentFrame(0),
    m_timeStampOfPreviousFrame(0),
    m_timeStampOfComputer(0),
    m_playerMovesLeft(false),
    m_playerMovesRight(false),
    m_speedup(10)
{
    ResetForNextSkull();
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
    UpdateFrame();

    renderer.Render2DBar(77, 60, 154, 1, EgaBrightRed);
    renderer.Render2DBar(77, 143, 154, 1, EgaBrightRed);
    renderer.Render2DPicture(egaGraph.GetPicture(CP_PADDLEWARPIC), 130, 48);
    DrawScore(renderer, egaGraph);

    renderer.Render2DPicture(egaGraph.GetSprite(PADDLESPR), (uint16_t)m_computerX, 66);
    renderer.Render2DPicture(egaGraph.GetSprite(PADDLESPR), (uint16_t)m_playerX, 135);
    renderer.Render2DPicture(egaGraph.GetSprite(BALLSPR), (uint16_t)m_skullX, (uint16_t)m_skullY);
}

void SkullNBones::UpdateFrame()
{
    if (m_timeStampOfCurrentFrame <= m_timeStampOfPreviousFrame)
    {
        return;
    }
    const uint32_t deltaTime = m_timeStampOfCurrentFrame - m_timeStampOfPreviousFrame;
    if (deltaTime > 100)
    {
        return;
    }

    if (m_timeStampOfComputer + 25 < m_timeStampOfCurrentFrame)
    {
        if (((uint32_t)m_computerX + 6 < (uint32_t)m_skullX) && ((uint32_t)m_computerX < 219))
        {
            m_computerX += 1.0f;
        }
        else if (((uint32_t)m_computerX + 6 > (uint32_t)m_skullX) && ((uint32_t)m_computerX > 77))
        {
            m_computerX -= 1.0f;
        }
        m_timeStampOfComputer = m_timeStampOfCurrentFrame;
    }

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
    const float skullDistanceInFrame = distanceInFrame / 8;
    float skullDistanceX = m_skullDeltaX * skullDistanceInFrame;
    float skullDistanceY = m_skullDeltaY * skullDistanceInFrame;
    if (m_skullX + skullDistanceX < SkullMinX || m_skullX + skullDistanceX > SkullMaxX)
    {
        m_skullDeltaX = -m_skullDeltaX;
        skullDistanceX = -skullDistanceX;
    }

    if (m_skullY + skullDistanceY < SkullMinY || m_skullY + skullDistanceY > SkullMaxY)
    {
        m_skullDeltaY = -m_skullDeltaY;
        skullDistanceY = -skullDistanceY;
    }

    m_skullX += skullDistanceX;
    m_skullY += skullDistanceY;

    if
        (
        (m_skullDeltaY < 0)
            && ((m_skullY >= 66) && (m_skullY < 66 + 3))
            && (((int32_t)m_skullX >= ((int32_t)m_computerX - 5)) && ((int32_t)m_skullX < ((int32_t)m_computerX + 11)))
            )
    {
        m_skullDeltaY = -m_skullDeltaY;
        m_skullDeltaX = ((int32_t)(m_skullX + 5 - m_computerX) >> 1) - 4;
        //if (!m_skullDeltaX)
        //    m_skullDeltaX--;
        //SD_PlaySound(COMPPADDLESND);
    }
    else if
        (
        (m_skullDeltaY > 0)
            && ((m_skullY >= (135 - 3)) && (m_skullY < 135))
            && (((int32_t)m_skullX >= ((int32_t)m_playerX - 5)) && ((int32_t)m_skullX < ((int32_t)m_playerX + 11)))
            )
    {
        if (((m_skullDeltaY >> 2) < 3) && !(--m_speedup))
        {
            m_skullDeltaY++;
            m_speedup = 10;
        }
        m_skullDeltaY = -m_skullDeltaY;
        m_skullDeltaX = ((int32_t)(m_skullX + 5 - m_playerX) >> 1) - 4;
        //SD_PlaySound(KEENPADDLESND);


    }
}

void SkullNBones::ResetForNextSkull()
{
    m_skullX = SkullMinX + ((SkullMaxX - SkullMinX) / 2);
    m_skullY = SkullMinY + ((SkullMaxY - SkullMinY) / 2);
    m_skullDeltaX = (1 - (rand() % 3)) << 1;
    m_skullDeltaY = 2;
    m_speedup = 10;
}
