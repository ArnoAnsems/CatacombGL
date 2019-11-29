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
// SkullNBones
//
#pragma once

#include "..\Engine\IRenderer.h"
#include "..\Engine\EgaGraph.h"

class SkullNBones
{
public:
    SkullNBones();
    void Draw(IRenderer& renderer, EgaGraph& egaGraph) const;

private:
    void DrawScore(IRenderer& renderer, EgaGraph& egaGraph) const;

    uint16_t m_playerScore;
    uint16_t m_computerScore;
};
