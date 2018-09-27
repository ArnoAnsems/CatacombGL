// Copyright (C) 2018 Arno Ansems
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

#include "IntroViewArmageddon.h"

IntroViewArmageddon::IntroViewArmageddon(IRenderer& renderer, const std::string& path) :
    IIntroView(renderer)
{
    m_shapeEntering = new Shape(renderer);
    const std::string shp05 = path + "SHP1.ARM";
    m_shapeEntering->LoadFromFile(shp05.c_str());
}

IntroViewArmageddon::~IntroViewArmageddon()
{
    if (m_shapeEntering != NULL)
    {
        delete m_shapeEntering;
        m_shapeEntering = NULL;
    }
}

void IntroViewArmageddon::DrawIntroduction(const uint32_t timeStamp)
{
    m_renderer.Render2DPicture(m_shapeEntering->GetPicture(), 20, 72);
}

void IntroViewArmageddon::DrawRequestDifficultyLevel()
{

}

void IntroViewArmageddon::DrawNoviceSelected()
{

}

void IntroViewArmageddon::DrawWarriorSelected()
{

}

void IntroViewArmageddon::DrawStandBeforeGate()
{

}