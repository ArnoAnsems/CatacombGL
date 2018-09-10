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
// GameMapsAbyss
//
// Data structures required for reading the Catacomb Abyss maps repository.
// Catacomb Abyss v1.13 and v1.24 share the same maps repository.
//
#pragma once

#include "..\Engine\GameMaps.h"
#include "EgaGraphAbyss.h"

const std::vector<int32_t> gameMapsOffsetsAbyss =
{
    1048,  2036,  3056,  4040,  5000,  6034,  7128,  8300,  9260,  10669,
    11880, 12961, 13595, 14813, 15703, 16108, 16759, 17942, 18935, 19144,
    19372, 19648, 20391, 20433
};

const std::vector<LevelInfo> gameMapsInfoAbyss =
{
    {
        "The Towne Cemetery",
        EgaBlack,
        EgaGreen,
        true,
        true        
    },
    {
        "The Garden of Tears",
        EgaBlack,
        EgaGreen,
        true,
        false
    },
    {
        // "The Den of Zombies" is stored in the third slot of the gamemaps file, but it is not the third map
        // that the player encounters in the game. It is in fact played inbetween "The Crypt of Nemesis the Undead"
        // and "The Subterranean Vault". The black ceiling in this map is therefore not a sky. Which also explains
        // why there is no lightning. See C4_PLAY.C, line 1032.
        "The Den of Zombies",
        EgaBlack,
        EgaGreen,
        false,
        false
    },
    {
        "The Mausoleum Grounds",
        EgaBlack,
        EgaGreen,
        true,
        false
    },
    {
        "The Main Floor of the Mausoleum",
        EgaDarkGray,
        EgaLightGray,
        false,
        false
    },
    {
        // Map 5 is a secret map and only accessible via the warp cheat
        "Mike's Blastable Passage",
        EgaRed,
        EgaMagenta,
        false,
        false
    },
    {
        "The Crypt of Nemesis the Undead",
        EgaBlack,
        EgaDarkGray,
        false,
        false
    },
    {
        "The Subterranean Vault",
        EgaBlack,
        EgaBrown,
        false,
        false       
    },
    {
        "The Ancient Aqueduct",
        EgaBlack,
        EgaBlue,
        false,
        false      
    },
    {
        "The Orc Mines",
        EgaBlack,
        EgaDarkGray,
        false,
        false       
    },
    {
        "The Lair of the Troll",
        EgaBlack,
        EgaBrown,
        false,
        false       
    },
    {
        "The Demon's Inferno",
        EgaBlack,
        EgaRed,
        false,
        false      
    },
    {
        "The Battleground of the Titans",
        EgaBlack,
        EgaDarkGray,
        false,
        false      
    },
    {
        "The Coven of Mages",
        EgaBlack,
        EgaBrightRed,
        false,
        false     
    },
    {
        "The Inner Sanctum",
        EgaBrown,
        EgaBrightYellow,
        false,
        false        
    },
    {
        "The Haunt of Nemesis",
        EgaBlack,
        EgaDarkGray,
        false,
        false        
    },
    {
        "The Passage to the Surface",
        EgaBlack,
        EgaDarkGray,
        false,
        false     
    },
    {
        // Map 17 is a secret map and only accessible via the warp cheat
        "Big Jim's Domain",
        EgaBlack,
        EgaBrightRed,
        false,
        false    
    },
    {
        // Map 18 is a secret map and only accessible via the warp cheat
        "Nolan's Nasty",
        EgaBlack,
        EgaBlack,
        false,
        false        
    },
    {
        // Maps 19 - 22 are maps that were only created for development purposes, but which are
        // still accessible via the warp cheat. The map names are based on the enumerations in MAPSABS.H.
        // Map 19: TILE_TESTER_3_MAP
        "Tile Tester 3",
        EgaBlack,
        EgaLightGray,
        false,
        false         
    },
    {
        // Map 20: TILE_TESTER_1_MAP
        "Tile Tester 1",
        EgaBlack,
        EgaDarkGray,
        false,
        false         
    },
    {
        // Map 21: TILE_TESTER_2_MAP
        "Tile Tester 2",
        EgaBlack,
        EgaDarkGray,
        false,
        false         
    },
    {
        // Map 22: GOOD_MAP_TO_USE_MAP
        "Good Map To Use",
        EgaBlack,
        EgaBrown,
        false,
        false  
    }
};

const std::vector<WallInfo> wallsInfoAbyss =
{
    // 0: No wall
    { 0, 0, 0, 0, WTOpen },
    { W_WARP1EWPIC, W_WARP2EWPIC, W_WARP1NSPIC, W_WARP2NSPIC, WTSolid },
    { W_NEMPICEWPIC, W_NEMPICEWPIC, W_NEMPICEWPIC, W_NEMPICEWPIC, WTSolid },
    { W_PENTAEWPIC, W_PENTAEWPIC, W_PENTANSPIC, W_PENTANSPIC, WTSolid },
    // 4 and 5: Bloody altar
    { W_ALTER_LFPIC, W_ALTER_LFPIC, W_ALTER_LFPIC, W_ALTER_LFPIC, WTSolid },
    { W_ALTER_RTPIC, W_ALTER_RTPIC, W_ALTER_RTPIC, W_ALTER_RTPIC, WTSolid },
    // 6, 7 and 8: Wall skeleton
    { W_SUB1EWPIC, W_SUB1EWPIC, W_SUB1NSPIC, W_SUB1NSPIC, WTSolid },
    { W_SUB2EWPIC, W_SUB2EWPIC, W_SUB2NSPIC, W_SUB2NSPIC, WTSolid },
    { W_SUB3EWPIC, W_SUB3EWPIC, W_SUB3NSPIC, W_SUB3NSPIC, WTSolid },
    // 9 and 10: Burning torch
    { W_TORCH1PIC, W_TORCH2PIC, W_TORCH1PIC, W_TORCH2PIC, WTSolid },
    { W_TORCH2PIC, W_TORCH1PIC, W_TORCH2PIC, W_TORCH1PIC, WTSolid },
    { W_LSUB_STONEPIC, W_LSUB_STONEPIC, W_DSUB_STONEPIC, W_DSUB_STONEPIC, WTSolid },
    { W_BLOODY_LSUB_STONEPIC, W_BLOODY_LSUB_STONEPIC, W_BLOODY_DSUB_STONEPIC, W_BLOODY_DSUB_STONEPIC , WTSolid },
    { W_BREATH_LWALL1PIC, W_BREATH_LWALL2PIC, W_BREATH_DWALL1PIC, W_BREATH_DWALL2PIC, WTSolid },
    { W_BREATH_LWALL2PIC, W_BREATH_LWALL1PIC, W_BREATH_DWALL2PIC, W_BREATH_DWALL1PIC, WTSolid },
    // 15, 16 and 17: Exploding walls
    { EXPWALL1PIC, EXPWALL2PIC, EXPWALL1PIC, EXPWALL2PIC, WTSolid },
    { EXPWALL2PIC, EXPWALL1PIC, EXPWALL2PIC, EXPWALL1PIC, WTSolid },
    { EXPWALL3PIC, EXPWALL3PIC, EXPWALL3PIC, EXPWALL3PIC, WTSolid },
    // 18 and 19: Wooden doorways; red key required
    { W_WOOD_DOORWAYPIC, W_WOOD_DOORWAYPIC, W_WOOD_DOORWAYPIC, W_WOOD_DOORWAYPIC, WTDoor /*WTDoorRedKeyRequired*/ },
    { W_WOOD_DOORWAY_GLOWPIC, W_WOOD_DOORWAY_GLOWPIC, W_WOOD_DOORWAY_GLOWPIC, W_WOOD_DOORWAY_GLOWPIC, WTDoorRedKeyRequired },
    // Ancient aqueduct walls
    { W_WATER1EWPIC, W_WATER2EWPIC, W_WATER1NSPIC, W_WATER2NSPIC, WTSolid },
    { W_DRAIN1EWPIC, W_DRAIN2EWPIC, W_DRAIN1NSPIC, W_DRAIN2NSPIC, WTSolid },
    { W_WATER2EWPIC, W_WATER1EWPIC, W_WATER2NSPIC, W_WATER1NSPIC, WTSolid },
    { W_DRAIN2EWPIC, W_DRAIN1EWPIC, W_DRAIN2NSPIC, W_DRAIN1NSPIC, WTSolid },
    // 24, 25, 26 and 27: Doors
    { W_WOODEN_DOORPIC, W_WOODEN_DOORPIC, W_WOODEN_DOORPIC, W_WOODEN_DOORPIC, WTDoor },
    { W_WOOD_DOOREWPIC, W_WOOD_DOOREWPIC, W_WOOD_DOORNSPIC, W_WOOD_DOORNSPIC, WTDoor },
    { W_METAL_DOORPIC, W_METAL_DOORPIC, W_METAL_DOORPIC, W_METAL_DOORPIC, WTDoor },
    { W_GLOW_DOORPIC, W_GLOW_DOORPIC, W_GLOW_DOORPIC, W_GLOW_DOORPIC, WTDoor },
    // 28: Final exit gate to the surface
    { W_FINALEXITPIC, W_FINALEXITPIC, W_FINALEXITPIC, W_FINALEXITPIC, WTVictory },
    // 29, 30 and 31: Ancient aqueduct exploding walls
    { W_WATER_EXP1PIC, W_WATER_EXP2PIC, W_WATER_EXP1PIC, W_WATER_EXP2PIC, WTSolid },
    { W_WATER_EXP2PIC, W_WATER_EXP1PIC, W_WATER_EXP2PIC, W_WATER_EXP1PIC, WTSolid },
    { W_WATER_EXP3PIC, W_WATER_EXP1PIC, W_WATER_EXP3PIC, W_WATER_EXP1PIC, WTSolid },
    { W_PRE_CHEATSPIC, W_PRE_CHEATSPIC, W_CHEAT_GODPIC, W_CHEAT_GODPIC, WTSolid },
    { W_CHEAT_WARPPIC, W_CHEAT_WARPPIC, W_CHEAT_ITEMSPIC, W_CHEAT_ITEMSPIC, WTSolid },
    { W_CHEAT_FREEZEPIC, W_CHEAT_FREEZEPIC, W_POST_CHEATPIC, W_POST_CHEATPIC, WTSolid },
    { W_SURFACE_PLAQPIC, W_SURFACE_PLAQPIC, W_SURFACE_PLAQPIC, W_SURFACE_PLAQPIC, WTSolid },
    // 36: Water gate
    { W_WATER_GATEEW1PIC, W_WATER_GATEEW2PIC, W_WATER_GATENS1PIC, W_WATER_GATENS2PIC, WTDoor },
    { WALL8LPIC, WALL8LPIC, WALL8DPIC, WALL8DPIC, WTSolid },
    { WALL9LPIC, WALL9LPIC, WALL9DPIC, WALL9DPIC, WTSolid },
    { WALL10DPIC, WALL10DPIC, WALL10LPIC, WALL10LPIC, WTSolid },
    { WALL11LPIC, WALL11LPIC, WALL11DPIC, WALL11DPIC, WTSolid },
    // 41, 42, 43 and 44: Wall skeleton
    { WALL12LPIC, WALL12LPIC, WALL12DPIC, WALL12DPIC, WTSolid },
    { WALL13LPIC, WALL13LPIC, WALL13DPIC, WALL13DPIC, WTSolid },
    { WALL14LPIC, WALL14LPIC, WALL14DPIC, WALL14DPIC, WTSolid },
    { WALL15LPIC, WALL15LPIC, WALL15DPIC, WALL15DPIC, WTSolid },
    { WALL16LPIC, WALL16LPIC, WALL16DPIC, WALL16DPIC, WTSolid },
    { WALL17LPIC, WALL17LPIC, WALL17DPIC, WALL17DPIC, WTSolid },
    { W_WINDOWEWPIC, W_WINDOWEWPIC, W_WINDOWNSPIC, W_WINDOWNSPIC, WTSolid },
    { WALL19LPIC, WALL19LPIC, WALL19DPIC, WALL19DPIC, WTSolid },
    { WALL20LPIC, WALL20LPIC, WALL20DPIC, WALL20DPIC, WTSolid },
    { WALL21LPIC, WALL21LPIC, WALL21DPIC, WALL21DPIC, WTSolid },
    // 51: generic door
    { WALL22LPIC, WALL22LPIC, WALL22DPIC, WALL22DPIC, WTDoor },
    { WALL23LPIC, WALL23LPIC, WALL23DPIC, WALL23DPIC, WTSolid },
    { WALL24LPIC, WALL24LPIC, WALL24DPIC, WALL24DPIC, WTSolid },
    // 54: DOWN stairs
    { WALL25LPIC, WALL25LPIC, WALL25DPIC, WALL25DPIC, WTDoor },
    { WALL26LPIC, WALL26LPIC, WALL26DPIC, WALL26DPIC, WTSolid },
    // 56: UP stairs
    { WALL27LPIC, WALL27LPIC, WALL27DPIC, WALL27DPIC, WTDoor },
    { WALL28LPIC, WALL28LPIC, WALL28DPIC, WALL28DPIC, WTSolid },
    { WALL29LPIC, WALL29LPIC, WALL29DPIC, WALL29DPIC, WTSolid },
    { WALL30LPIC, WALL30LPIC, WALL30DPIC, WALL30DPIC, WTSolid },
    { WALL31LPIC, WALL31LPIC, WALL31DPIC, WALL31DPIC, WTSolid },
    { W_BREATH_LWALL4PIC, W_BREATH_LWALL3PIC, W_BREATH_DWALL4PIC, W_BREATH_DWALL3PIC, WTSolid },
    { W_BREATH_LWALL3PIC, W_BREATH_LWALL4PIC, W_BREATH_DWALL3PIC, W_BREATH_DWALL4PIC, WTSolid },
    // Statues
    { MAGE_STATUEPIC, MAGE_STATUEPIC, MAGE_STATUEPIC, MAGE_STATUEPIC, WTSolid },
    { ZOMBIE_STATUEPIC, ZOMBIE_STATUEPIC, ZOMBIE_STATUEPIC, ZOMBIE_STATUEPIC, WTSolid },
    { EYE_STATUEPIC, EYE_STATUEPIC, EYE_STATUEPIC, EYE_STATUEPIC, WTSolid },
    { NEM_STATUEPIC, NEM_STATUEPIC, NEM_STATUEPIC, NEM_STATUEPIC, WTSolid },
    { SKELETON_STATUEPIC, SKELETON_STATUEPIC, SKELETON_STATUEPIC, SKELETON_STATUEPIC, WTSolid },
    { SPOOK_STATUEPIC, SPOOK_STATUEPIC, SPOOK_STATUEPIC, SPOOK_STATUEPIC, WTSolid },
    { ORCH_STATUEPIC, ORCH_STATUEPIC, ORCH_STATUEPIC, ORCH_STATUEPIC, WTSolid }
};

const gameMapsStaticData gameMapsAbyss = {"GAMEMAPS.ABS", gameMapsOffsetsAbyss, gameMapsInfoAbyss, wallsInfoAbyss};