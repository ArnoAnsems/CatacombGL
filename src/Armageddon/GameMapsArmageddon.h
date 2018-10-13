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
// GameMapsArmageddon
//
// Data structures required for reading the Catacomb Armageddon maps repository.
//
#pragma once

#include "..\Engine\GameMaps.h"
#include "EgaGraphArmageddon.h"

const std::vector<int32_t> gameMapsOffsetsArmageddon =
{
    1120,  2353,  3366,  4537,  5662,  6754,  7734,  8865,  9788, 10860,
    11812, 12725, 14221, 15502, 16521, 17060, 17611, 17653
};

const std::vector<LevelInfo> gameMapsInfoArmageddon =
{
    {
        "", // TOWN_MORBIDITY_MAP
        EgaBlack,
        EgaBrightRed,
        true,
        true
    },
    {
        "", // DARK_FOREST_MAP
        EgaBlack,
        EgaBlack,
        true,
        false
    },
    {
        "", // GARDEN_OF_SOULS_MAP
        EgaBlue,
        EgaGreen,
        false,
        false
    },
    {
        "", // LOST_CITY_DAMND_MAP
        EgaBrightBlue,
        EgaGreen,
        false,
        false
    },
    {
        "", // TEMPLE_OF_VIPER_MAP
        EgaBlack,
        EgaRed,
        false,
        false
    },
    {
        "", // TORTURE_CHAMBER_MAP
        EgaBlue,
        EgaBrightRed,
        false,
        false
    },
    {
        "", // DEMONS_HOLD_MAP
        EgaBrightRed,
        EgaRed,
        false,
        false
    },
    {
        "", // COLONY_FIRE_ANT_MAP
        EgaBlack,
        EgaBrown,
        false,
        false
    },
    {
        "", // HALL_WRETCH_POX_MAP
        EgaBlue,
        EgaBrown,
        false,
        false
    },
    {
        "", // LAIR_OF_SUCUBUS_MAP
        EgaBrightBlue,
        EgaRed,
        false,
        false
    },
    {
        "", // BLOOD_CHAMB_EYE_MAP
        EgaBlack,
        EgaRed,
        false,
        false
    },
    {
        "", // FLAMING_INFERNO_MAP
        EgaBlack,
        EgaBrightWhite,
        false,
        false
    },
    {
        "", // SUBTERR_RIVER_MAP
        EgaBlack,
        EgaBlue,
        false,
        false
    },
    {
        "", // CRYSTAL_MAZE_MG_MAP
        EgaBlue,
        EgaBrightBlue,
        false,
        false
    },
    {
        "", // RAMPARTS_OF_NEM_MAP
        EgaBlack,
        EgaRed,
        false,
        false
    },
    {
        "", // FORTRESS_OF_NEM_MAP
        EgaBlack,
        EgaRed,
        false,
        false
    },
    {
        "", // PASSAGE_TO_SURF_MAP
        EgaBlue,
        EgaGreen,
        false,
        false
    }
};

const std::vector<WallInfo> wallsInfoArmageddon =
{
    // 0: No wall
    { {0}, {0}, WTOpen },
    { {CRYSTAL_LIGHT_1PIC, CRYSTAL_LIGHT_2PIC, CRYSTAL_LIGHT_3PIC, CRYSTAL_LIGHT_4PIC}, {CRYSTAL_DARK_1PIC, CRYSTAL_DARK_2PIC, CRYSTAL_DARK_3PIC, CRYSTAL_DARK_4PIC}, WTSolid },
    { {CRYSTAL_LIGHT_2PIC, CRYSTAL_LIGHT_3PIC, CRYSTAL_LIGHT_4PIC, CRYSTAL_LIGHT_1PIC}, {CRYSTAL_DARK_2PIC, CRYSTAL_DARK_3PIC, CRYSTAL_DARK_4PIC, CRYSTAL_DARK_1PIC}, WTSolid },
    { {CRYSTAL_LIGHT_3PIC, CRYSTAL_LIGHT_4PIC, CRYSTAL_LIGHT_1PIC, CRYSTAL_LIGHT_2PIC}, {CRYSTAL_DARK_3PIC, CRYSTAL_DARK_4PIC, CRYSTAL_DARK_1PIC, CRYSTAL_DARK_2PIC}, WTSolid },
    { {CRYSTAL_LIGHT_4PIC, CRYSTAL_LIGHT_1PIC, CRYSTAL_LIGHT_2PIC, CRYSTAL_LIGHT_3PIC}, {CRYSTAL_DARK_4PIC, CRYSTAL_DARK_1PIC, CRYSTAL_DARK_2PIC, CRYSTAL_DARK_3PIC}, WTSolid },

    { {FIRE_WALL_1PIC, FIRE_WALL_2PIC, FIRE_WALL_3PIC, FIRE_WALL_4PIC}, {FIRE_WALL_1PIC, FIRE_WALL_2PIC, FIRE_WALL_3PIC, FIRE_WALL_4PIC}, WTSolid },
    { {FIRE_WALL_2PIC, FIRE_WALL_3PIC, FIRE_WALL_4PIC, FIRE_WALL_1PIC}, {FIRE_WALL_2PIC, FIRE_WALL_3PIC, FIRE_WALL_4PIC, FIRE_WALL_1PIC}, WTSolid },
    { {FIRE_WALL_3PIC, FIRE_WALL_4PIC, FIRE_WALL_1PIC, FIRE_WALL_2PIC}, {FIRE_WALL_3PIC, FIRE_WALL_4PIC, FIRE_WALL_1PIC, FIRE_WALL_2PIC}, WTSolid },
    { {FIRE_WALL_4PIC, FIRE_WALL_1PIC, FIRE_WALL_2PIC, FIRE_WALL_3PIC}, {FIRE_WALL_4PIC, FIRE_WALL_1PIC, FIRE_WALL_2PIC, FIRE_WALL_3PIC}, WTSolid },

    { {BRN_STONE_GATEPIC}, {BRN_STONE_GATEPIC}, WTDoor },
    { {BRN_STONE_WALL_1PIC}, {BRN_STONE_WALL_2PIC}, WTSolid },
    { {KUDZU_WEAK_LIGHTPIC}, {KUDZU_WEAK_DARKPIC}, WTSolid },
    { {KUDZU_LIGHT_WALLPIC}, {KUDZU_DARK_WALLPIC}, WTSolid },
    { {HEDGE_WALLPIC}, {HEDGE_WALLPIC}, WTSolid },
    { {HEDGE_EYESPIC}, {HEDGE_EYESPIC}, WTSolid },

    { {W_GEN_DOOR1PIC}, {W_GEN_DOOR1PIC}, WTDoor },
    { {BRN_WINDOW_LIGHTPIC}, {BRN_WINDOW_DARKPIC}, WTSolid },

    { {ALTAR_LEFTPIC}, {ALTAR_LEFTPIC}, WTSolid },
    { {ALTAR_RIGHTPIC}, {ALTAR_RIGHTPIC}, WTSolid },
    { {GRAY_LIGHT_WALLPIC}, {GRAY_DARK_WALLPIC}, WTSolid },
    { {GRAY_LIGHT_SIGNPIC}, {GRAY_DARK_SIGNPIC}, WTSolid },

    { {MANICLE_LIGHT_WALLPIC}, {MANICLE_DARK_WALLPIC}, WTSolid },
    { {MANICLE_LIGHT_BLOODYPIC}, {MANICLE_DARK_BLOODYPIC}, WTSolid },

    { {LIGHT_CURTAIN_WINDOWPIC}, {DARK_CURTAIN_WINDOWPIC}, WTSolid },
    { {LIGHT_CURTAIN_WALLPIC}, {DARK_CURTAIN_WALLPIC}, WTSolid },
    { {BRN_LIGHT_SIGNPIC}, {BRN_DARK_SIGNPIC}, WTSolid },

    { {LIGHT_STONE_WALLPIC}, {DARK_STONE_WALLPIC}, WTSolid },

    { {W_GEN_DOOR2PIC}, {W_GEN_DOOR2PIC}, WTDoor },

    { {TROLL_LIGHT_STONEPIC}, {TROLL_DARK_STONEPIC}, WTSolid },

    { {BRN_FLAGSTONE_LIGHT_2PIC}, {BRN_FLAGSTONE_DARK_2PIC}, WTSolid },

    { {W_CRYSTAL_DOORPIC}, {W_CRYSTAL_DOORPIC}, WTDoor },

    { {DMG_BRN_FSTN_LTPIC}, {DMG_BRN_FSTN_DKPIC}, WTSolid },

    { {RUST_METAL_LIGHTPIC}, {RUST_METAL_DARKPIC}, WTSolid },
    { {GRAY_METAL_LIGHTPIC}, {GRAY_METAL_DARKPIC}, WTSolid },

    { {WEAK_STONE_LIGHTPIC}, {WEAK_STONE_DARKPIC},  WTSolid },

    { {DMG_FIN_FSTN_LTPIC}, {DMG_FIN_FSTN_DKPIC}, WTSolid },

    { {WEAK_GRAY_RFGSTN_LIGHTPIC}, {WEAK_GRAY_RFGSTN_DARKPIC}, WTSolid },
    { {0}, {0}, WTOpen },

    { {WEAK_CRYSTAL_LIGHTPIC}, {WEAK_CRYSTAL_DARKPIC}, WTSolid },

    { {RED_MUD_LIGHTPIC}, {BRN_MUD_DARKPIC}, WTSolid },

    { {STEEL_DOOR1PIC}, {STEEL_DOOR1PIC}, WTDoor },

    { {RED_MUD_WEAK_LIGHTPIC}, {BRN_MUD_WEAK_DARKPIC}, WTSolid },

    { {STEEL_DOOR2PIC}, {STEEL_DOOR2PIC}, WTDoor },

    { {HORN_DOORPIC}, {HORN_DOORPIC}, WTDoor },
    { {TROLL_BLOODY_LT_STONEPIC}, {TROLL_BLOODY_DK_STONEPIC}, WTSolid },
    { {CLOSED_DOOR_1PIC}, {CLOSED_DOOR_1PIC}, WTDoor },

    { {GRY_DOOR_LTPIC}, {GRY_DOOR_DKPIC}, WTDoor },

    { {BRN_DOOR_LTPIC}, {BRN_DOOR_DKPIC}, WTDoor },

    { {GRY_FGSTN_LTPIC}, {GRY_FGSTN_DKPIC}, WTSolid },
    { {DOOR_2PIC}, {DOOR_2PIC}, WTDoor },

    { {WATER_LIGHT_WEAK_1PIC, WATER_LIGHT_WEAK_2PIC, WATER_LIGHT_WEAK_3PIC}, {WATER_DARK_WEAK_1PIC, WATER_DARK_WEAK_2PIC, WATER_DARK_WEAK_3PIC}, WTSolid },
    { {WATER_LIGHT_WEAK_2PIC, WATER_LIGHT_WEAK_3PIC, WATER_LIGHT_WEAK_1PIC},{ WATER_DARK_WEAK_2PIC, WATER_DARK_WEAK_3PIC, WATER_DARK_WEAK_1PIC }, WTSolid },
    { {WATER_LIGHT_WEAK_3PIC, WATER_LIGHT_WEAK_1PIC, WATER_LIGHT_WEAK_2PIC},{ WATER_DARK_WEAK_3PIC, WATER_DARK_WEAK_1PIC, WATER_DARK_WEAK_2PIC }, WTSolid },

    { {WATER_LIGHT_1PIC, WATER_LIGHT_2PIC, WATER_LIGHT_3PIC}, {WATER_DARK_1PIC, WATER_DARK_2PIC, WATER_DARK_3PIC}, WTSolid },
    { {WATER_LIGHT_2PIC, WATER_LIGHT_3PIC, WATER_LIGHT_1PIC}, {WATER_DARK_2PIC, WATER_DARK_3PIC, WATER_DARK_1PIC}, WTSolid },
    { {WATER_LIGHT_3PIC, WATER_LIGHT_1PIC, WATER_LIGHT_2PIC}, {WATER_DARK_3PIC, WATER_DARK_1PIC, WATER_DARK_2PIC}, WTSolid },

    { {LIGHT_BREATH_1PIC, LIGHT_BREATH_2PIC, LIGHT_BREATH_3PIC}, {DARK_BREATH_1PIC, DARK_BREATH_2PIC, DARK_BREATH_3PIC}, WTSolid },   // tf_SPECIAL ???
    { {LIGHT_BREATH_2PIC, LIGHT_BREATH_3PIC, LIGHT_BREATH_1PIC },{DARK_BREATH_2PIC, DARK_BREATH_3PIC, DARK_BREATH_1PIC}, WTSolid },
    { {LIGHT_BREATH_3PIC, LIGHT_BREATH_1PIC, LIGHT_BREATH_2PIC },{DARK_BREATH_3PIC, DARK_BREATH_1PIC, DARK_BREATH_2PIC}, WTSolid },

    { {EXP_WALL_1PIC, EXP_WALL_2PIC, EXP_WALL_3PIC}, {EXP_WALL_1PIC, EXP_WALL_2PIC, EXP_WALL_3PIC}, WTSolid },
    { {EXP_WALL_2PIC, EXP_WALL_3PIC, EXP_WALL_1PIC}, {EXP_WALL_2PIC, EXP_WALL_3PIC, EXP_WALL_1PIC}, WTSolid },
    { {EXP_WALL_3PIC, EXP_WALL_1PIC, EXP_WALL_2PIC}, {EXP_WALL_3PIC, EXP_WALL_1PIC, EXP_WALL_2PIC}, WTSolid },

    { {WATER_EXP_WALL_1PIC, WATER_EXP_WALL_2PIC, WATER_EXP_WALL_3PIC}, {WATER_EXP_WALL_1PIC, WATER_EXP_WALL_2PIC, WATER_EXP_WALL_3PIC}, WTSolid },
    { {WATER_EXP_WALL_2PIC, WATER_EXP_WALL_3PIC, WATER_EXP_WALL_1PIC}, {WATER_EXP_WALL_2PIC, WATER_EXP_WALL_3PIC, WATER_EXP_WALL_1PIC}, WTSolid },
    { {WATER_EXP_WALL_3PIC, WATER_EXP_WALL_1PIC, WATER_EXP_WALL_2PIC}, {WATER_EXP_WALL_3PIC, WATER_EXP_WALL_1PIC, WATER_EXP_WALL_2PIC}, WTSolid },

    { {FINALWALLPIC}, {FINALWALLPIC}, WTVictory },

    { {LT_SKEL1PIC}, {LT_SKEL1PIC}, WTSolid },
    { {DK_SKEL1PIC}, {DK_SKEL1PIC}, WTSolid },
    { {LT_SKEL2PIC}, {LT_SKEL2PIC}, WTSolid },
    { {DK_SKEL2PIC}, {DK_SKEL2PIC}, WTSolid },

    { {0}, {0}, WTOpen }, // tf_INVISIBLE_WALL

    { {TAP_1PIC}, {TAP_1PIC}, WTSolid },
    { {TAP_2PIC}, {TAP_2PIC}, WTSolid },
    { {TAP_3PIC}, {TAP_3PIC}, WTSolid },
    { {TAP_4PIC}, {TAP_4PIC}, WTSolid },
    { {TAP_5PIC}, {TAP_5PIC}, WTSolid },

    { {WATER_DOOR1_PIC, WATER_DOOR2_PIC}, {WATER_DOOR1_PIC, WATER_DOOR2_PIC}, WTDoor },
    { {WATER_DOOR2_PIC, WATER_DOOR1_PIC}, {WATER_DOOR2_PIC, WATER_DOOR1_PIC}, WTDoor }
};

const gameMapsStaticData gameMapsArmageddon = { "GAMEMAPS.ARM", gameMapsOffsetsArmageddon, gameMapsInfoArmageddon, wallsInfoArmageddon, 59, 62 };