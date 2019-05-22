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
// GameMapsApocalypse
//
// Data structures required for reading the Catacomb Apocalypse maps repository.
//
#pragma once

#include "..\Engine\GameMaps.h"
#include "EgaGraphApocalypse.h"

const std::vector<int32_t> gameMapsOffsetsApocalypse =
{
    1036,  2086,  2605,  3437,  4393,  5504,  6188,  6979,  9571, 10520,
    11497, 12498, 15059, 15536, 16488, 17179, 18002, 18813, 18855
};

const std::vector<LevelInfo> gameMapsInfoApocalypse =
{
    {
        "", // EGYPT_CEMETERY_MAP (0)
        EgaBlue,
        EgaGreen,
        false,
        false
    },
    {
        "", // CEMETERY_EGYPT_MAP (1)
        EgaBlack,
        EgaGreen,
        false,
        false
    },
    {
        "", // WIZARDS_ROOM_MAP (2)
        EgaGreen,
        EgaBrightRed,
        false,
        false
    },
    {
        "", // AXIS_TIME_LORDS_MAP (3)
        EgaBrightRed,
        EgaBlue,
        false,
        false
    },
    {
        "", // FUTURE_NODE_MAP (4)
        EgaBlue,
        EgaCyan,
        false,
        false
    },
    {
        "", // FIRE_NODE_MAP (5)
        EgaBlack,
        EgaBrightWhite,
        false,
        false
    },
    {
        "", // ANCIENT_NODE_MAP (6)
        EgaBrightCyan,
        EgaGreen,
        false,
        false
    },
    {
        "", // WATER_NODE_MAP (7)
        EgaBrightCyan,
        EgaBlue,
        false,
        false
    },
    {
        "", // BIG_WIZARD_MAZE_MAP (8)
        EgaBlack,
        EgaGreen,
        true,
        false
    },
    {
        "", // RAMB_MANTA_MAP (9)
        EgaBlack,
        EgaBlue,
        false,
        false
    },
    {
        "", // FLOODED_CITY_MAP (10)
        EgaBlack,
        EgaBlue,
        true,
        false
    },
    {
        "", // COMPUTER_CORE_MAP (11)
        EgaBrightRed,
        EgaCyan,
        false,
        false
    },
    {
        "", // BIG_SPACE_MAP_MAP (12)
        EgaRed,
        EgaBlack,
        false,
        false
    },
    {
        "", // ENCOUNTER_MAP (13)
        EgaBlack,
        EgaBlack,
        false,
        false
    },
    {
        "", // NEMESIS_FINAL_MAP (14)
        EgaBlack,
        EgaBlack,
        false,
        false
    },
    {
        "", // PASSAGE_HOME_MAP (15)
        EgaBlack,
        EgaBlack,
        false,
        false
    },
    {
        "", // EGYPT_1_MAP (16)
        EgaBlack,
        EgaRed,
        false,
        false
    },
    {
        "", // WIZARD_GARDEN_MAP (17)
        EgaBlack,
        EgaGreen,
        true,
        false
    }
};

const std::vector<WallInfo> wallsInfoApocalypse =
{
    // 0: No wall
    { {0}, {0}, WTOpen },
    { {0}, {0}, WTOpen },
    { {0}, {0}, WTOpen },
    { {EGYPT2LIGHTPIC}, {EGYPT2DARKPIC}, WTSolid },
    { {EGYPT3LIGHTPIC}, {EGYPT3DARKPIC}, WTSolid },
    { {FIREWALL1PIC}, {FIREWALL1PIC}, WTSolid},
    { {FIREWALL2PIC}, {FIREWALL2PIC}, WTSolid},
    { {FIREWALL3PIC}, {FIREWALL3PIC}, WTSolid},
    { {FIREWALL4PIC}, {FIREWALL4PIC}, WTSolid},
    { {NEMESISPIC}, {NEMESISPIC},WTSolid},
    { {ALTARLEFTPIC}, {ALTARLEFTPIC}, WTSolid},
    { {ALTARRIGHTPIC}, {ALTARRIGHTPIC}, WTSolid},
    { {TEMPLEWALLLIGHTPIC}, {TEMPLEWALLDARKPIC}, WTSolid},
    { {TORCHWALL1PIC}, {TORCHWALL1PIC}, WTSolid},
    { {TORCHWALL2PIC}, {TORCHWALL2PIC}, WTSolid},
    { {BRNBRKLIGHTPIC}, {BRNBRKDARKPIC}, WTSolid},
    { {BRNBRKEMLIGHTPIC}, {BRNBRKEMDARKPIC}, WTSolid},
    { {IRONGATEPIC}, {IRONGATEPIC}, WTSolid},
    { {BRNFLGLIGHTPIC}, {BRNFLGDARKPIC}, WTSolid},
    { {BRNFLGWINDOWLIGHTPIC}, {BRNFLGWINDOWDARKPIC}, WTSolid},
    { {BRNFLGVINELIGHTPIC}, {BRNFLGVINEDARKPIC}, WTSolid},
    { {BRNFLGDMGLIGHTPIC}, {BRNFLGDMGDARKPIC}, WTSolid},
    { {SPACEDMG1LIGHTPIC}, {SPACEDMG1DARKPIC}, WTSolid},
    { {SPACEDMG2LIGHTPIC}, {SPACEDMG2DARKPIC}, WTSolid},
    { {SPACE1LIGHTPIC}, {SPACE1DARKPIC}, WTSolid},
    { {SPACE2LIGHTPIC}, {SPACE2DARKPIC}, WTSolid},
    { {SPACE3LIGHTPIC}, {SPACE3DARKPIC}, WTSolid},
    { {SPACE4LIGHTPIC}, {SPACE4DARKPIC}, WTSolid},
    { {SPACE5LIGHTPIC}, {SPACE5DARKPIC}, WTSolid},
    { {SPACE6LIGHTPIC}, {SPACE6DARKPIC}, WTSolid},
    { {SPACE7LIGHTPIC}, {SPACE7DARKPIC}, WTSolid},
    { {SPACE8LIGHTPIC}, {SPACE8DARKPIC}, WTSolid},

    { {0}, {0}, WTOpen },//SPACE9LIGHTPIC,
    { {0}, {0}, WTOpen },//SPACEDMG9LIGHTPIC,
    { {SPACE10LIGHTPIC}, {SPACE10DARKPIC}, WTSolid},
    { {RUSTDOORLIGHTPIC}, {RUSTDOORDARKPIC}, WTSolid},

    { {SPACE11LIGHTPIC}, {SPACE11DARKPIC}, WTSolid },
    { {SPACE12LIGHTPIC}, {SPACE12DARKPIC}, WTSolid},
    { {SPACE13LIGHTPIC}, {SPACE13DARKPIC}, WTSolid},
    { {SPACE14LIGHTPIC}, {SPACE14DARKPIC}, WTSolid},

    { {SPACEDMG5LIGHTPIC}, {SPACEDMG5DARKPIC}, WTSolid},
    { {SPACEDMG6LIGHTPIC}, {SPACEDMG6DARKPIC}, WTSolid},

    { {TAP1PIC}, {TAP1PIC}, WTSolid},
    { {TAP2PIC}, {TAP2PIC}, WTSolid},
    { {ENDPIC}, {ENDPIC}, WTVictory},
    { {0}, {0}, WTOpen}, //SIRONLIGHTPIC,

    { {SPCDOOR1LIGHTPIC}, {SPCDOOR1DARKPIC}, WTSolid},
    { {SPCDOOR2LIGHTPIC}, {SPCDOOR2DARKPIC}, WTSolid},
    { {SPCDOOR3LIGHTPIC}, {SPCDOOR3DARKPIC}, WTSolid},
    { {SPCDOOR4LIGHTPIC}, {SPCDOOR4DARKPIC}, WTSolid},

   { {COLUMNSLIGHTPIC}, {COLUMNSDARKPIC}, WTSolid},

   { {DEMONSTATUELIGHTPIC}, {DEMONSTATUEDARKPIC}, WTSolid},

   { {0}, {0}, WTOpen}, //CRYSTALBWALL1LIGHTPIC,

   { {0}, {0}, WTOpen },//SRUSTLIGHTPIC,

   { {TROLLSTATUELIGHTPIC}, {TROLLSTATUEDARKPIC}, WTSolid},

   { {BRNDMGVINELIGHTPIC}, {BRNDMGVINEDARKPIC}, WTSolid},
   { {TAP3PIC}, {TAP3PIC}, WTSolid},
   { {HORNDOORPIC}, {HORNDOORPIC}, WTSolid},
   { {RUNEDOORPIC}, {RUNEDOORPIC}, WTSolid},

   { {EXP_WALL_1PIC, EXP_WALL_2PIC, EXP_WALL_3PIC}, {EXP_WALL_1PIC, EXP_WALL_2PIC, EXP_WALL_3PIC}, WTSolid },
   { {EXP_WALL_2PIC, EXP_WALL_3PIC, EXP_WALL_1PIC}, {EXP_WALL_2PIC, EXP_WALL_3PIC, EXP_WALL_1PIC}, WTSolid },
   { {EXP_WALL_3PIC, EXP_WALL_1PIC, EXP_WALL_2PIC}, {EXP_WALL_3PIC, EXP_WALL_1PIC, EXP_WALL_2PIC}, WTSolid },
   { {WATER_EXP_WALL_1PIC, WATER_EXP_WALL_2PIC, WATER_EXP_WALL_3PIC}, {WATER_EXP_WALL_1PIC, WATER_EXP_WALL_2PIC, WATER_EXP_WALL_3PIC}, WTSolid },
   { {WATER_EXP_WALL_2PIC, WATER_EXP_WALL_3PIC, WATER_EXP_WALL_1PIC}, {WATER_EXP_WALL_2PIC, WATER_EXP_WALL_3PIC, WATER_EXP_WALL_1PIC}, WTSolid },
   { {WATER_EXP_WALL_3PIC, WATER_EXP_WALL_1PIC, WATER_EXP_WALL_2PIC}, {WATER_EXP_WALL_3PIC, WATER_EXP_WALL_1PIC, WATER_EXP_WALL_2PIC}, WTSolid },

   { {IRONDMGLIGHTPIC}, {IRONDMGDARKPIC}, WTSolid},
   { {IRONLIGHTPIC}, {IRONDARKPIC}, WTSolid},
   { {0}, {0}, WTOpen},
   { {TROLLBLOODYLIGHTPIC}, {TROLLBLOODYDARKPIC}, WTSolid},
   { {TROLLLIGHTPIC}, {TROLLDARKPIC}, WTSolid},

   { {0}, {0}, WTInvisibleWall },

   { {STONEDOORLIGHTPIC}, {STONEDOORDARKPIC}, WTSolid},
   { {0}, {0}, WTOpen },
   { {IRONWTR1LIGHTPIC}, {IRONWTR1DARKPIC}, WTSolid},
   { {IRONWTR2LIGHTPIC}, {IRONWTR2DARKPIC}, WTSolid},
   { {IRONWTR3LIGHTPIC}, {IRONWTR3DARKPIC}, WTSolid},

   { {RUSTWTR1LIGHTPIC}, {RUSTWTR1DARKPIC}, WTSolid},
   { {RUSTWTR2LIGHTPIC}, {RUSTWTR2DARKPIC}, WTSolid},
   { {RUSTWTR3LIGHTPIC}, {RUSTWTR3DARKPIC}, WTSolid},

   { {CEMETARYLIGHTPIC}, {CEMETARYDARKPIC}, WTSolid},
   { {0}, {0}, WTOpen },	//	STAIRDWNLIGHTPIC,

   { {WGRATE1LIGHTPIC}, {WGRATE1DARKPIC}, WTSolid},
   { {WGRATE2LIGHTPIC}, {WGRATE2DARKPIC}, WTSolid},
   { {WGRATE3LIGHTPIC}, {WGRATE3DARKPIC}, WTSolid},

   { {MAS_WIN_LIGHTPIC}, {MAS_WIN_DARKPIC}, WTSolid},
   { {MAS_DOOR_LIGHTPIC}, {MAS_DOOR_DARKPIC}, WTSolid},
   { {MAS_VINE1_LIGHTPIC}, {MAS_VINE1_DARKPIC}, WTSolid},
   { {MAS_VINE2_LIGHTPIC}, {MAS_VINE2_DARKPIC}, WTSolid},

   // Start of non-solid walls
   { {0}, {0}, WTOpen },
   { {0}, {0}, WTOpen },
   { {0}, {0}, WTOpen },
   { {0}, {0}, WTOpen },
   { {0}, {0}, WTOpen },
   { {0}, {0}, WTOpen },

   // solid walls
   { {SGRATEPIC}, {SGRATEPIC}, WTSolid}
};

const gameMapsStaticData gameMapsApocalypse = { "GAMEMAPS.APC", gameMapsOffsetsApocalypse, gameMapsInfoApocalypse, wallsInfoApocalypse, 59, 62 };
