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

#include "../Engine/IRenderer.h"
#include "../Engine/EgaGraph.h"
#include "../Engine/PlayerInput.h"
#include "../Engine/AudioPlayer.h"
#include "../Engine/GuiElementBase.h"

class SkullNBones: public GuiElementBase
{
public:
    SkullNBones(
        const PlayerInput& playerInput,
        AudioPlayer& audioPlayer,
        EgaGraph& egaGraph,
        uint32_t& timeStamp,
        RenderableText& renderableText);
    virtual const GuiEvent& ProcessInput() override;
    virtual void Draw(IRenderer& renderer) const override;

private:
    void DrawScore(RenderableText& renderableText) const;
    void UpdateFrame();
    void ResetForNextSkull();
    void Reset();

    AudioPlayer& m_audioPlayer;
    uint16_t m_playerScore;
    uint16_t m_computerScore;
    uint32_t& m_timeStamp;
    const GuiEvent m_closeEvent = {GuiActionClose, 0 };
    RenderableText& m_renderableText;

    float m_playerX;
    float m_computerX;
    float m_skullX;
    float m_skullY;
    int32_t m_skullDeltaX;
    int32_t m_skullDeltaY;
    uint32_t m_timeStampOfCurrentFrame;
    uint32_t m_timeStampOfPreviousFrame;
    uint32_t m_timeStampOfComputer;
    uint32_t m_timeStampSkullGone;
    bool m_skullIsGone;
    bool m_playerMovesLeft;
    bool m_playerMovesRight;
    int16_t m_speedup;
    bool m_lastScore;

    const Picture* m_paddleWarPic;
    const Picture* m_paddleSprite;
    const Picture* m_ballSprite;
    const Picture* m_menuMaskPic;
};
