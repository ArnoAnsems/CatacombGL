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
#include "../Engine/GameId.h"
#include "../Engine/FadeEffect.h"
#include "../Engine/GameControllerHint.h"
#include <filesystem>
#include <string>
#include <memory>

class Shape;
class EgaGraph;

class IntroViewAbyss : public IIntroView
{
public:
    explicit IntroViewAbyss(GameId gameId, IRenderer& renderer, EgaGraph& egaGraph, const std::filesystem::path& path);
    ~IntroViewAbyss();
    void DrawIntroduction(const uint32_t timeStamp);
    void DrawRequestDifficultyLevel(bool showGameControllerHint);
    void DrawNoviceSelected();
    void DrawWarriorSelected();
    void DrawStandBeforeGate();
    void DrawBackForMore();
    bool CanShowBackForMore();

private:
    const GameId m_gameId;
    EgaGraph& m_egaGraph;
    std::unique_ptr<Shape> m_shapeEntering;
    std::unique_ptr<Shape> m_shapePresents;
    std::unique_ptr<Shape> m_shapeSoftdisk;
    std::unique_ptr<Shape> m_shapeTitle;
    std::unique_ptr<Shape> m_shapeCredits;
    std::unique_ptr<Shape> m_shapeTrilogy;
    std::unique_ptr<Shape> m_shapeSelectDifficulty;
    std::unique_ptr<Shape> m_shapeConfirmDifficulty;
    std::unique_ptr<Shape> m_shapeNovice;
    std::unique_ptr<Shape> m_shapeWarrior;
    std::unique_ptr<Shape> m_shapeStandBeforeGate;
    std::unique_ptr<Shape> m_shapeBackForMore;
    FadeEffect m_fadeEffect;
    GameControllerHint m_gameControllerHint;
};

