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
// IntroViewCatacomb3D
//
// Introduction screens for the Catacomb 3-D
//
#pragma once

#include "../Engine/IIntroView.h"
#include "../Engine/EgaGraph.h"
#include "../Engine/HighScores.h"
#include "../Engine/FadeEffect.h"
#include <filesystem>
#include <string>

class IntroViewCatacomb3D : public IIntroView
{
public:
    IntroViewCatacomb3D(IRenderer& renderer, EgaGraph* egagraph, const std::filesystem::path& path, HighScores& highScores);
    ~IntroViewCatacomb3D();
    void DrawIntroduction(const uint32_t timeStamp);
    void DrawRequestDifficultyLevel();
    void DrawNoviceSelected();
    void DrawWarriorSelected();
    void DrawStandBeforeGate();

private:
    EgaGraph* m_egaGraph;
    HighScores& m_highScores;
    FadeEffect m_fadeEffect;
};
