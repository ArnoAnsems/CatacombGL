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
// IIntroView
//
// Abstract interface for drawing the (2D) introduction screens.
// The concrete implementation can differ per game (Abyss, Armageddon, etc).
//
#pragma once

#include "IRenderer.h"
#include "Logging.h"

class IIntroView
{
public:
    IIntroView(IRenderer& renderer);
    virtual void DrawIntroduction(const uint32_t timeStamp) = 0;
    virtual void DrawRequestDifficultyLevel() = 0;
    virtual void DrawNoviceSelected() = 0;
    virtual void DrawWarriorSelected() = 0;
    virtual void DrawStandBeforeGate() = 0;

protected:
    IRenderer& m_renderer;
};

