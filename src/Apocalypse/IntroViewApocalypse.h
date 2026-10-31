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
// IntroViewApocalypse
//
// Introduction screens for the Catacomb Apocalypse
//
#pragma once

#include "../Engine/IIntroView.h"
#include "../Engine/GameControllerHint.h"
#include <filesystem>
#include <string>
#include <memory>

class Shape;

class IntroViewApocalypse : public IIntroView
{
public:
    explicit IntroViewApocalypse(IRenderer& renderer, EgaGraph& egaGraph, const std::filesystem::path& path);
    ~IntroViewApocalypse();
    void DrawIntroduction(const uint32_t timeStamp);
    void DrawRequestDifficultyLevel(bool showGameControllerHint);
    void DrawNoviceSelected();
    void DrawWarriorSelected();
    void DrawStandBeforeGate();
    void DrawBackForMore();
    bool CanShowBackForMore();

private:
    std::unique_ptr<Shape> m_shapeEntering;
    std::unique_ptr<Shape> m_shapePresents;
    std::unique_ptr<Shape> m_shapeSoftdisk;
    std::unique_ptr<Shape> m_shapeTitle;
    std::unique_ptr<Shape> m_shapeCreditsProgramming;
    std::unique_ptr<Shape> m_shapeCreditsArt;
    std::unique_ptr<Shape> m_shapeCreditsQA;
    std::unique_ptr<Shape> m_shapeCreditsDesign;
    std::unique_ptr<Shape> m_shapeSelectDifficulty;
    std::unique_ptr<Shape> m_shapeConfirmDifficulty;
    std::unique_ptr<Shape> m_shapeNovice;
    std::unique_ptr<Shape> m_shapeWarrior;
    std::unique_ptr<Shape> m_shapeStandBeforeGate;
    std::unique_ptr<Shape> m_shapeBackForMore;
    GameControllerHint m_gameControllerHint;
};

