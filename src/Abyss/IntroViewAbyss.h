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

//
// IntroViewAbyss
//
// Introduction screens for the Catacomb Abyss
//
#pragma once

#include "..\Engine\IIntroView.h"
#include "..\Engine\Shape.h"
#include <string>

class IntroViewAbyss : public IIntroView
{
public:
    IntroViewAbyss(IRenderer& renderer, const std::string& path, Logging* logging);
    ~IntroViewAbyss();
    void DrawIntroduction(const uint32_t timeStamp);
    void DrawRequestDifficultyLevel();
    void DrawNoviceSelected();
    void DrawWarriorSelected();
    void DrawStandBeforeGate();

private:
    Shape* m_shapeEntering;
    Shape* m_shapePresents;
    Shape* m_shapeSoftdisk;
    Shape* m_shapeTitle;
    Shape* m_shapeCredits;
    Shape* m_shapeTrilogy;
    Shape* m_shapeSelectDifficulty;
    Shape* m_shapeConfirmDifficulty;
    Shape* m_shapeNovice;
    Shape* m_shapeWarrior;
    Shape* m_shapeStandBeforeGate;
};

