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

#include "../Engine/IIntroView.h"
#include "../Engine/Shape.h"
#include "../Engine/GameId.h"
#include "../Engine/FadeEffect.h"
#include <filesystem>
#include <string>

class IntroViewAbyss : public IIntroView
{
public:
    IntroViewAbyss(GameId gameId, IRenderer& renderer, const std::filesystem::path& path);
    ~IntroViewAbyss();
    void DrawIntroduction(const uint32_t timeStamp);
    void DrawRequestDifficultyLevel();
    void DrawNoviceSelected();
    void DrawWarriorSelected();
    void DrawStandBeforeGate();

private:
    const GameId m_gameId;
    Shape* m_shapeEntering = nullptr;
    Shape* m_shapePresents = nullptr;
    Shape* m_shapeSoftdisk = nullptr;
    Shape* m_shapeTitle = nullptr;
    Shape* m_shapeCredits = nullptr;
    Shape* m_shapeTrilogy = nullptr;
    Shape* m_shapeSelectDifficulty = nullptr;
    Shape* m_shapeConfirmDifficulty = nullptr;
    Shape* m_shapeNovice = nullptr;
    Shape* m_shapeWarrior = nullptr;
    Shape* m_shapeStandBeforeGate = nullptr;
    FadeEffect m_fadeEffect;
};

