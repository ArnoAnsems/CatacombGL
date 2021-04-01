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
#include "AudioRepositoryCatacomb3D.h"

const float SkullMinX = 76.0f;
const float SkullMaxX = 228.0f;
const float SkullMinY = 62.0f;
const float SkullMaxY = 137.0f;
const uint16_t WinningScore = 21;

SkullNBones::SkullNBones(
    const PlayerInput& playerInput,
    AudioPlayer& audioPlayer,
    EgaGraph& egaGraph,
    uint32_t& timeStamp,
    RenderableText& renderableText) :
    GuiElementBase(playerInput),
    m_audioPlayer(audioPlayer),
    m_renderableText(renderableText),
    m_timeStamp(timeStamp),
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
    m_timeStampSkullGone(0),
    m_skullIsGone(true),
    m_playerMovesLeft(false),
    m_playerMovesRight(false),
    m_speedup(10),
    m_lastScore(false),
    m_paddleWarPic(egaGraph.GetPicture(CP_PADDLEWARPIC)),
    m_paddleSprite(egaGraph.GetSprite(PADDLESPR)),
    m_ballSprite(egaGraph.GetSprite(BALLSPR)),
    m_menuMaskPic(egaGraph.GetMaskedPicture(CP_MENUMASKPICM))
{
    ResetForNextSkull();
}

const GuiEvent& SkullNBones::ProcessInput()
{
    if (m_playerInput.IsKeyJustPressed(SDLK_ESCAPE))
    {
        Reset();
        return m_closeEvent;
    }

    if (m_playerScore == WinningScore || m_computerScore == WinningScore)
    {
        if (m_playerInput.IsAnyKeyPressed())
        {
            Reset();
            return m_closeEvent;
        }
    }
    else
    {
        m_playerMovesLeft = m_playerInput.IsKeyPressed(SDLK_LEFT);
        m_playerMovesRight = m_playerInput.IsKeyPressed(SDLK_RIGHT);
    }

    if (m_timeStampOfPreviousFrame == 0)
    {
        // First input, wait 1 sec for the skull appears
        m_timeStampSkullGone = m_timeStamp + 1000;
    }

    m_timeStampOfPreviousFrame = m_timeStampOfCurrentFrame;
    m_timeStampOfCurrentFrame = m_timeStamp;
    UpdateFrame();

    return GetEvent();
}

void SkullNBones::DrawScore(RenderableText& renderableText) const
{
    renderableText.LeftAligned("YOU:", EgaBrightRed, 80, 52);
    const std::string playerScoreStr = std::to_string(m_playerScore);
    renderableText.LeftAligned(playerScoreStr, EgaBrightRed, 102, 52);
    renderableText.LeftAligned("COMP:", EgaBrightRed, 182, 52);
    const std::string computerScoreStr = std::to_string(m_computerScore);
    renderableText.LeftAligned(computerScoreStr, EgaBrightRed, 218, 52);
}

void SkullNBones::Draw(IRenderer& renderer) const
{
    renderer.Render2DBar(77, 60, 154, 1, EgaBrightRed);
    renderer.Render2DBar(77, 143, 154, 1, EgaBrightRed);
    renderer.Render2DPicture(m_paddleWarPic, 130, 48);
    DrawScore(m_renderableText);

    renderer.Render2DPicture(m_paddleSprite, (uint16_t)m_computerX, 66);
    renderer.Render2DPicture(m_paddleSprite, (uint16_t)m_playerX, 135);

    if (!m_skullIsGone)
    {
        renderer.Render2DPicture(m_ballSprite, (uint16_t)m_skullX, (uint16_t)m_skullY);
    }

    if (m_playerScore == WinningScore || m_computerScore == WinningScore)
    {
        const int width = 120;
        const int offsetX = 154 - (width / 2);
        renderer.Render2DPicture(m_menuMaskPic, 74, 48);
        renderer.Render2DBar(offsetX + 1, 81, width - 2, 36, EgaBlack);
        renderer.Render2DBar(offsetX, 80, width, 1, EgaRed);
        renderer.Render2DBar(offsetX, 117, width, 1, EgaRed);
        renderer.Render2DBar(offsetX, 81, 1, 36, EgaRed);
        renderer.Render2DBar(offsetX + width - 1, 81, 1, 36, EgaRed);

        const char* message = (m_playerScore == WinningScore) ? "You won!" : "You lost!";
        m_renderableText.Centered(message, EgaBrightRed, 154, 89);

        renderer.Render2DBar(offsetX + 4, 102, width - 8, 1, EgaRed);
        m_renderableText.Centered("Press any key", EgaRed, 154, 104);
    }
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

    if (m_playerScore == WinningScore || m_computerScore == WinningScore)
    {
        return;
    }

    if (m_skullIsGone && m_timeStampSkullGone <= m_timeStampOfCurrentFrame)
    {
        m_skullIsGone = false;
        ResetForNextSkull();
    }

    if (!m_skullIsGone && m_timeStampOfComputer + 14 < m_timeStampOfCurrentFrame)
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
    constexpr float distanceInPixelsPerMilliSec = distanceInPixelsPerTic * 70.0f / 1000.0f;
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

    if (!m_skullIsGone)
    {
        const float skullDistanceInFrame = distanceInFrame / 8;
        float skullDistanceX = m_skullDeltaX * skullDistanceInFrame;
        float skullDistanceY = m_skullDeltaY * skullDistanceInFrame;
        if (m_skullX + skullDistanceX < SkullMinX || m_skullX + skullDistanceX > SkullMaxX)
        {
            m_skullDeltaX = -m_skullDeltaX;
            skullDistanceX = -skullDistanceX;
            m_audioPlayer.Play(BALLBOUNCESND);
        }

        if (m_skullY + skullDistanceY < SkullMinY)
        {
            m_timeStampSkullGone = m_timeStampOfCurrentFrame + 1000;
            m_skullIsGone = true;
            m_playerScore++;
            m_lastScore = true;
            m_audioPlayer.Play(KEENSCOREDSND);
        }
        else if (m_skullY + skullDistanceY > SkullMaxY)
        {
            m_timeStampSkullGone = m_timeStampOfCurrentFrame + 1000;
            m_skullIsGone = true;
            m_computerScore++;
            m_lastScore = false;
            m_audioPlayer.Play(COMPSCOREDSND);
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
            if (!m_skullDeltaX)
            {
                m_skullDeltaX--;
            }
            m_audioPlayer.Play(COMPPADDLESND);
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
            m_audioPlayer.Play(KEENPADDLESND);
        }
    }
}

void SkullNBones::ResetForNextSkull()
{
    m_skullX = SkullMinX + ((SkullMaxX - SkullMinX) / 2);
    m_skullY = SkullMinY + ((SkullMaxY - SkullMinY) / 2);
    m_skullDeltaX = (1 - (rand() % 3)) << 1;
    m_skullDeltaY = m_lastScore ? -2 : 2;
    m_speedup = 10;
}

void SkullNBones::Reset()
{
    m_timeStampOfCurrentFrame = 0;
    m_timeStampOfPreviousFrame = 0;
    m_timeStampSkullGone = 0;
    m_skullIsGone = true;
    m_playerScore = 0;
    m_computerScore = 0;
    m_playerX = 148.0f;
    m_computerX = 148.0f;
    ResetForNextSkull();
}
