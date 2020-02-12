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
    m_highScores(highScores),
    m_fadeEffect(renderer)
{

}

IntroViewCatacomb3D::~IntroViewCatacomb3D()
{

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
        m_highScores.Draw(m_renderer, *m_egaGraph, timeStamp, HIGHSCORESPIC);
        break;
    }
    }

    if ((timeStamp % 5000) > 4000)
    {
        if (m_fadeEffect.OverlayActive())
        {
            m_fadeEffect.SetOverlay();
        }
    }

    if ((timeStamp % 5000) < 1000)
    {
        const uint32_t milliSec = timeStamp % 5000;
        m_fadeEffect.DrawOverlay(milliSec);
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