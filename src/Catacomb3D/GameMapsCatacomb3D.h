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
// GameMapsCatacomb3D
//
// Data structures required for reading the Catacomb 3-D maps repository.
//
#pragma once

#include "..\Engine\GameMaps.h"
#include "EgaGraphCatacomb3D.h"

const std::vector<int32_t> gameMapsOffsetsCatacomb3D =
{
    389,
    851,
    1568,
    2159,
    2829,
    3490,
    4271,
    5023,
    5316,
    6018,
    6753,
    7416,
    8331,
    9151,
    9848,
    10558,
    11353,
    11948,
    12682,
    14246,
    14288
};

const std::vector<LevelInfo> gameMapsInfoCatacomb3D =
{
    {
        "",
        EgaBlack,
        EgaBrightRed,
        false,
        false
    }
};

const std::vector<WallInfo> wallsInfoCatacomb3D =
{
    // 0: No wall
    { {0}, {0}, WTOpen },
};

const gameMapsStaticData gameMapsCatacomb3D = { "GAMEMAPS.C3D", gameMapsOffsetsCatacomb3D, gameMapsInfoCatacomb3D, wallsInfoCatacomb3D, 0, 0 };
