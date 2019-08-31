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

IntroViewCatacomb3D::IntroViewCatacomb3D(IRenderer& renderer, EgaGraph* egagraph, const std::string& path) :
    IIntroView(renderer),
    m_egaGraph(egagraph)
{

}

IntroViewCatacomb3D::~IntroViewCatacomb3D()
{

}

void IntroViewCatacomb3D::DrawIntroduction(const uint32_t timeStamp)
{
    m_renderer.Render2DPicture(m_egaGraph->GetPicture(TITLEPIC), 0, 0);
}

void IntroViewCatacomb3D::DrawRequestDifficultyLevel()
{
    m_renderer.RenderTextLeftAligned("Select N or W [placeholder]", DefaultFont::Get(m_renderer), EgaBrightWhite, 10, 10);
}

void IntroViewCatacomb3D::DrawNoviceSelected()
{
    m_renderer.RenderTextLeftAligned("Novice selected [placeholder]", DefaultFont::Get(m_renderer), EgaBrightWhite, 10, 10);
}

void IntroViewCatacomb3D::DrawWarriorSelected()
{
    m_renderer.RenderTextLeftAligned("Warrior selected [placeholder]", DefaultFont::Get(m_renderer), EgaBrightWhite, 10, 10);
}

void IntroViewCatacomb3D::DrawStandBeforeGate()
{
    m_renderer.RenderTextLeftAligned("About to enter the game... [placeholder]", DefaultFont::Get(m_renderer), EgaBrightWhite, 10, 10);
}