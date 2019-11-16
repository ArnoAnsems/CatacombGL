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

#include "IntroViewCatacomb3D.h"
#include "..\Engine\DefaultFont.h"
#include "EgaGraphCatacomb3D.h"

IntroViewCatacomb3D::IntroViewCatacomb3D(IRenderer& renderer, EgaGraph* egagraph, const std::string& path, HighScores& highScores) :
    IIntroView(renderer),
    m_egaGraph(egagraph),
    m_highScores(highScores)
{

}

IntroViewCatacomb3D::~IntroViewCatacomb3D()
{

}

void IntroViewCatacomb3D::ApplyEqualSpacingToNumbers(std::string& str)
{
    const uint16_t offsetInFontOfEqualSpacedNumbers = 129;
    for (uint16_t i = 0; i < str.length(); i++)
    {
        str.at(i) += (offsetInFontOfEqualSpacedNumbers - '0');
    }
}

void IntroViewCatacomb3D::DrawIntroduction(const uint32_t timeStamp)
{
    m_renderer.Render2DPicture(m_egaGraph->GetPicture(TITLEPIC), 0, 0);
    const uint8_t pictureIndex = (timeStamp / 5000) % 3;
    switch (pictureIndex)
    {
    case 0:
    {
        m_renderer.Render2DPicture(m_egaGraph->GetPicture(TITLEPIC), 0, 0);
        break;
    }
    case 1:
    {
        m_renderer.Render2DPicture(m_egaGraph->GetPicture(CREDITSPIC), 0, 0);
        break;
    }
    case 2:
    {
        m_renderer.Render2DPicture(m_egaGraph->GetPicture(HIGHSCORESPIC), 0, 0);
        uint16_t y = 68;
        for (const auto highScore : m_highScores.Get())
        {
            m_renderer.RenderTextLeftAligned(highScore.name.c_str(), m_egaGraph->GetFont(3), EgaBlue, 60, y);
            std::string levelStr = std::to_string(highScore.level);
            ApplyEqualSpacingToNumbers(levelStr);
            m_renderer.RenderTextLeftAligned(levelStr.c_str(), m_egaGraph->GetFont(3), EgaBlue, 192 - (8 * (uint16_t)levelStr.length()), y);
            std::string scoreStr = std::to_string(highScore.score);
            ApplyEqualSpacingToNumbers(scoreStr);
            m_renderer.RenderTextLeftAligned(scoreStr.c_str(), m_egaGraph->GetFont(3), EgaBlue, 264 - (8 * (uint16_t)scoreStr.length()), y);
            y += 16;
        }
        break;
    }
    }
}

void IntroViewCatacomb3D::DrawRequestDifficultyLevel()
{
    // Not applicable
}

void IntroViewCatacomb3D::DrawNoviceSelected()
{
    // Not applicable
}

void IntroViewCatacomb3D::DrawWarriorSelected()
{
    // Not applicable
}

void IntroViewCatacomb3D::DrawStandBeforeGate()
{
    // Not applicable
}