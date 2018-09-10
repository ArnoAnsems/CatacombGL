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
// DecorateMisc
//
// Actor definitions for the Catacomb Abyss decorative items, exploding walls, the player, etc.
//
#pragma once

#include "..\Engine\Decorate.h"
#include "EgaGraphAbyss.h"
#include "AudioRepositoryAbyss.h"

const uint16_t actorIdNone = 0;
const uint16_t actorIdTomb1 = 1;
const uint16_t actorIdTomb2 = 2;
const uint16_t actorIdTomb3 = 3;
const uint16_t actorIdExplodingWall = 4;
const uint16_t actorIdWarpPit = 5;
const uint16_t actorIdWarpPortal1 = 6;
const uint16_t actorIdWarpPortal2 = 7;
const uint16_t actorIdWarpPortal3 = 8;
const uint16_t actorIdWarpPortal4 = 9;
const uint16_t actorIdWarpPortal5 = 10;
const uint16_t actorIdPlayer = 11;

//
// TOMBSTONE 1
//

const DecorateAnimation tomb1DecorationAnimation =
{
    { TOMB1PIC, 8, ActionNone }
};

const DecorateState tomb1StateDecoration =
{
    tomb1DecorationAnimation,
    StateIdDecoration // Loop
};

const std::map<DecorateStateId, DecorateState> tomb1States =
{
    std::make_pair(StateIdDecoration, tomb1StateDecoration)
};

const DecorateActor decorateTomb1 =
{
    actorIdTomb1,   // Id
    46, // spawnOnAllDifficulties;
    46, // spawnOnNormalAndHard;
    46,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    tomb1States,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0 // actionParameter
};

//
// TOMBSTONE 2
//

const DecorateAnimation tomb2DecorationAnimation =
{
    { TOMB2PIC, 8, ActionNone }
};

const DecorateState tomb2StateDecoration =
{
    tomb2DecorationAnimation,
    StateIdDecoration // Loop
};

const std::map<DecorateStateId, DecorateState> tomb2States =
{
    std::make_pair(StateIdDecoration, tomb2StateDecoration)
};

const DecorateActor decorateTomb2 =
{
    actorIdTomb2,   // Id
    47, // spawnOnAllDifficulties;
    47, // spawnOnNormalAndHard;
    47,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    tomb2States,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0 // actionParameter
};

//
// TOMBSTONE 3
//

const DecorateAnimation tomb3DecorationAnimation =
{
    { TOMB3PIC, 8, ActionNone }
};

const DecorateState tomb3StateDecoration =
{
    tomb3DecorationAnimation,
    StateIdDecoration // Loop
};

const std::map<DecorateStateId, DecorateState> tomb3States =
{
    std::make_pair(StateIdDecoration, tomb3StateDecoration)
};

const DecorateActor decorateTomb3 =
{
    actorIdTomb3,   // Id
    48, // spawnOnAllDifficulties;
    48, // spawnOnNormalAndHard;
    48,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    tomb3States,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0 // actionParameter
};

//
// EXPLODING WALL
//

const DecorateAnimation explodingWallDeadAnimation =
{
    { 0, 20, ActionExplodeWall1 },
    { 0, 20, ActionExplodeWall2 },
    { 0, 20, ActionExplodeWall3 },
    { 0, 8, ActionExplodeWall4 },
    { 0, 8, ActionRemove }
};

const DecorateState explodingWallStateDead =
{
    explodingWallDeadAnimation,
    StateIdDead // Loop
};

const std::map<DecorateStateId, DecorateState> explodingWallStates =
{
    std::make_pair(StateIdDead, explodingWallStateDead)
};

const DecorateActor decorateExplodingWall =
{
    actorIdExplodingWall,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard
    1,  // initialHealth;
    5 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    explodingWallStates,
    StateIdDead,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0 // actionParameter
};

//
// WARP PIT
//

const DecorateAnimation warpPitWaitForPickupAnimation =
{
    { PITOBJPIC, 8, ActionWaitForPickup }
};

const DecorateState warpPitStateWaitForPickup =
{
    warpPitWaitForPickupAnimation,
    StateIdWaitForPickup // Loop
};

const DecorateAnimation warpPitPickupAnimation =
{
    { PITOBJPIC, 8, ActionWarpToOtherLevel }
};

const DecorateState warpPitStatePickup =
{
    warpPitPickupAnimation,
    StateIdWaitForPickup
};

const std::map<DecorateStateId, DecorateState> warpPitStates =
{
    std::make_pair(StateIdWaitForPickup, warpPitStateWaitForPickup),
    std::make_pair(StateIdPickup, warpPitStatePickup)
};

const DecorateActor decorateWarpPit =
{
    actorIdWarpPit,   // Id
    54, // spawnOnAllDifficulties;
    54, // spawnOnNormalAndHard;
    54,  // spawnOnHard
    1,  // initialHealth;
    0.333f,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    warpPitStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0 // actionParameter
};

//
// WARP PORTAL
//

const DecorateAnimation warpPortalWaitForPickupAnimation =
{
    { OBJ_WARP1PIC, 10, ActionWaitForPickup },
    { OBJ_WARP2PIC, 10, ActionWaitForPickup },
    { OBJ_WARP3PIC, 10, ActionWaitForPickup },
    { OBJ_WARP4PIC, 10, ActionWaitForPickup }
};

const DecorateState warpPortalStateWaitForPickup =
{
    warpPortalWaitForPickupAnimation,
    StateIdWaitForPickup // Loop
};

const DecorateAnimation warpPortalPickupAnimation =
{
    { OBJ_WARP1PIC, 8, ActionWarpInsideLevel }
};

const DecorateState warpPortalStatePickup =
{
    warpPortalPickupAnimation,
    StateIdWaitForPickup
};

const std::map<DecorateStateId, DecorateState> warpPortalStates =
{
    std::make_pair(StateIdWaitForPickup, warpPortalStateWaitForPickup),
    std::make_pair(StateIdPickup, warpPortalStatePickup)
};

const DecorateActor decorateWarpPortal1 =
{
    actorIdWarpPortal1,   // Id
    31, // spawnOnAllDifficulties;
    31, // spawnOnNormalAndHard;
    31,  // spawnOnHard
    1,  // initialHealth;
    0.333f,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    warpPortalStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    1 // actionParameter
};

const DecorateActor decorateWarpPortal2 =
{
    actorIdWarpPortal2,   // Id
    32, // spawnOnAllDifficulties;
    32, // spawnOnNormalAndHard;
    32,  // spawnOnHard
    1,  // initialHealth;
    0.333f,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    warpPortalStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    2 // actionParameter
};

const DecorateActor decorateWarpPortal3 =
{
    actorIdWarpPortal3,   // Id
    33, // spawnOnAllDifficulties;
    33, // spawnOnNormalAndHard;
    33,  // spawnOnHard
    1,  // initialHealth;
    0.333f,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    warpPortalStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    3 // actionParameter
};

const DecorateActor decorateWarpPortal4 =
{
    actorIdWarpPortal4,   // Id
    34, // spawnOnAllDifficulties;
    34, // spawnOnNormalAndHard;
    34,  // spawnOnHard
    1,  // initialHealth;
    0.333f,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    warpPortalStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    4 // actionParameter
};

const DecorateActor decorateWarpPortal5 =
{
    actorIdWarpPortal5,   // Id
    35, // spawnOnAllDifficulties;
    35, // spawnOnNormalAndHard;
    35,  // spawnOnHard
    1,  // initialHealth;
    0.333f,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    warpPortalStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    5 // actionParameter
};

//
// PLAYER
//

const DecorateAnimation playerWalkAnimation =
{
    { 0, 8, ActionNone }
};

const DecorateState playerStateWalk =
{
    playerWalkAnimation,
    StateIdWalk
};

const DecorateAnimation playerDyingAnimation =
{
    { 0, 8, ActionNone }
};

const DecorateState playerStateDying =
{
    playerDyingAnimation,
    StateIdDying
};

const std::map<DecorateStateId, DecorateState> playerStates =
{
    std::make_pair(StateIdWalk, playerStateWalk),
    std::make_pair(StateIdDying, playerStateDying)
};

const DecorateActor decoratePlayer =
{
    actorIdPlayer,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard
    100,  // initialHealth;
    0.4f,    // size;
    Always,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    playerStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0 // actionParameter
};
