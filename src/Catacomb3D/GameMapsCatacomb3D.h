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

#include "../Engine/GameMaps.h"
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

const std::vector<LevelInfo> gameMapsInfoCatacomb3Dv100 =
{
    {
        "The Approach",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Grelminar's Keep",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Ground Floor",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Second Floor",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Third Floor",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Tower One",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Tower Two",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Secret Halls",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Access Floor",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "The Dungeon",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Lower Dungeon",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Catacomb",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Lower Reaches",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "The Warrens",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Hidden Caverns",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "The Fens of Insanity",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Chaos Corridors",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "The Labyrinth",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Halls of Blood",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Grelminar's Lair",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    }
};

const std::vector<LevelInfo> gameMapsInfoCatacomb3Dv122 =
{
    {
        "The Approach",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Nemesis's Keep",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Ground Floor",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Second Floor",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Third Floor",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Tower One",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Tower Two",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Secret Halls",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Access Floor",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "The Dungeon",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Lower Dungeon",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Catacomb",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Lower Reaches",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "The Warrens",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Hidden Caverns",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "The Fens of Insanity",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Chaos Corridors",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "The Labyrinth",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Halls of Blood",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "Nemesis's Lair",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    }
};

const std::vector<WallInfo> wallsInfoCatacomb3D =
{
    // 0: No wall
    { {0}, {0}, WTOpen },
    {{WALL1LPIC}, {WALL1DPIC}, WTSolid},
    {{WALL2LPIC}, {WALL2DPIC}, WTSolid},
    {{WALL3LPIC}, {WALL3DPIC}, WTSolid},
    {{WALL4LPIC}, {WALL4DPIC}, WTSolid},
    {{WALL5LPIC}, {WALL5DPIC}, WTSolid},
    {{WALL6LPIC}, {WALL6DPIC}, WTSolid},
    {{WALL7LPIC}, {WALL7DPIC}, WTSolid},
    {{WALL1LPIC}, {WALL1DPIC}, WTDestructable},
    {{WALL2LPIC}, {WALL2DPIC}, WTDestructable},
    {{WALL3LPIC}, {WALL3DPIC}, WTDestructable},
    {{WALL4LPIC}, {WALL4DPIC}, WTDestructable},
    {{WALL5LPIC}, {WALL5DPIC}, WTDestructable},
    {{WALL6LPIC}, {WALL6DPIC}, WTDestructable},
    {{WALL7LPIC}, {WALL7DPIC}, WTDestructable},
    {{EXPWALL1PIC}, {EXPWALL1PIC}, WTSolid},
    {{EXPWALL2PIC}, {EXPWALL2PIC}, WTSolid},
    {{EXPWALL3PIC}, {EXPWALL3PIC}, WTSolid},
    {{RDOOR1PIC, RDOOR2PIC}, {RDOOR1PIC, RDOOR2PIC}, WTDoorRedKeyRequired},
    {{RDOOR2PIC, RDOOR1PIC}, {RDOOR2PIC, RDOOR1PIC}, WTDoorRedKeyRequired},
    {{RDOOR1PIC, RDOOR2PIC}, {RDOOR1PIC, RDOOR2PIC}, WTDoorRedKeyRequired},
    {{RDOOR2PIC, RDOOR1PIC}, {RDOOR2PIC, RDOOR1PIC}, WTDoorRedKeyRequired},
    {{YDOOR1PIC, YDOOR2PIC}, {YDOOR1PIC, YDOOR2PIC}, WTDoorYellowKeyRequired},
    {{YDOOR2PIC, YDOOR1PIC}, {YDOOR2PIC, YDOOR1PIC}, WTDoorYellowKeyRequired},
    {{YDOOR1PIC, YDOOR2PIC}, {YDOOR1PIC, YDOOR2PIC}, WTDoorYellowKeyRequired},
    {{YDOOR2PIC, YDOOR1PIC}, {YDOOR2PIC, YDOOR1PIC}, WTDoorYellowKeyRequired},
    {{GDOOR1PIC, GDOOR2PIC}, {GDOOR1PIC, GDOOR2PIC}, WTDoorGreenKeyRequired},
    {{GDOOR2PIC, GDOOR1PIC}, {GDOOR2PIC, GDOOR1PIC}, WTDoorGreenKeyRequired},
    {{GDOOR1PIC, GDOOR2PIC}, {GDOOR1PIC, GDOOR2PIC}, WTDoorGreenKeyRequired},
    {{GDOOR2PIC, GDOOR1PIC}, {GDOOR2PIC, GDOOR1PIC}, WTDoorGreenKeyRequired},
    {{BDOOR1PIC, BDOOR2PIC}, {BDOOR1PIC, BDOOR2PIC}, WTDoorBlueKeyRequired},
    {{BDOOR2PIC, BDOOR1PIC}, {BDOOR2PIC, BDOOR1PIC}, WTDoorBlueKeyRequired},
    {{BDOOR1PIC, BDOOR2PIC}, {BDOOR1PIC, BDOOR2PIC}, WTDoorBlueKeyRequired},
    {{BDOOR2PIC, BDOOR1PIC}, {BDOOR2PIC, BDOOR1PIC}, WTDoorBlueKeyRequired},
};

const gameMapsStaticData gameMapsCatacomb3Dv100 = { "GAMEMAPS.C3D", gameMapsOffsetsCatacomb3D, gameMapsInfoCatacomb3Dv100, wallsInfoCatacomb3D, 15, 15 };
const gameMapsStaticData gameMapsCatacomb3Dv122 = { "GAMEMAPS.C3D", gameMapsOffsetsCatacomb3D, gameMapsInfoCatacomb3Dv122, wallsInfoCatacomb3D, 15, 15 };
