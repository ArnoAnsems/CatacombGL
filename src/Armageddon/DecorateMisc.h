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
// Actor definitions for the Catacomb Armageddon decorative items, exploding walls, the player, etc.
//
#pragma once

#include "..\Engine\Decorate.h"
#include "DecorateProjectiles.h"
#include "EgaGraphArmageddon.h"
//#include "AudioRepositoryArmageddon.h"

const uint16_t actorIdNone = 0;
const uint16_t actorIdTomb1 = 1;
const uint16_t actorIdTomb2 = 2;
const uint16_t actorIdTomb3 = 3;
const uint16_t actorIdExplodingWall = 4;
const uint16_t actorIdWarpAntHill = 5;
const uint16_t actorIdWarpPortal1 = 6;
const uint16_t actorIdWarpPortal2 = 7;
const uint16_t actorIdWarpPortal3 = 8;
const uint16_t actorIdWarpPortal4 = 9;
const uint16_t actorIdWarpPortal5 = 10;
const uint16_t actorIdPlayer = 11;
const uint16_t actorIdColumn = 12;
const uint16_t actorIdSulphurGas = 13;
const uint16_t actorIdFirePot = 14;
const uint16_t actorIdFountain = 15;
const uint16_t actorIdArch1 = 80;
const uint16_t actorIdArch2 = 81;
const uint16_t actorIdArch3 = 82;
const uint16_t actorIdArch4 = 83;
const uint16_t actorIdArch5 = 84;
const uint16_t actorIdArch6 = 85;
const uint16_t actorIdArch7 = 86;
const uint16_t actorIdArch8 = 87;
const uint16_t actorIdArch9 = 88;
const uint16_t actorIdArch10 = 89;
const uint16_t actorIdArch11 = 90;
const uint16_t actorIdArch12 = 91;
const uint16_t actorIdArch13 = 92;
const uint16_t actorIdForceField = 93;

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
    45, // spawnOnAllDifficulties;
    45, // spawnOnNormalAndHard;
    45,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    tomb1States,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter,
    0 // ProjectileId
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
    46, // spawnOnAllDifficulties;
    46, // spawnOnNormalAndHard;
    46,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    tomb2States,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
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
    47, // spawnOnAllDifficulties;
    47, // spawnOnNormalAndHard;
    47,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    tomb3States,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
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
    0, // actionParameter
    0 // ProjectileId
};

//
// ANT HILL
//

const DecorateAnimation warpAntHillWaitForPickupAnimation =
{
    { ANT_HILLPIC, 20, ActionWaitForPickup }
};

const DecorateState warpAntHillStateWaitForPickup =
{
    warpAntHillWaitForPickupAnimation,
    StateIdWaitForPickup // Loop
};

const DecorateAnimation warpAntHillPickupAnimation =
{
    { ANT_HILLPIC, 8, ActionWarpToOtherLevel }
};

const DecorateState warpAntHillStatePickup =
{
    warpAntHillPickupAnimation,
    StateIdWaitForPickup
};

const std::map<DecorateStateId, DecorateState> warpAntHillStates =
{
    std::make_pair(StateIdWaitForPickup, warpAntHillStateWaitForPickup),
    std::make_pair(StateIdPickup, warpAntHillStatePickup)
};

const DecorateActor decorateWarpAntHill =
{
    actorIdWarpAntHill,   // Id
    53, // spawnOnAllDifficulties;
    53, // spawnOnNormalAndHard;
    53,  // spawnOnHard
    1,  // initialHealth;
    0.333f,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    warpAntHillStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
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
    1, // actionParameter
    0 // ProjectileId
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
    2, // actionParameter
    0 // ProjectileId
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
    3, // actionParameter
    0 // ProjectileId
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
    4, // actionParameter
    0 // ProjectileId
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
    5, // actionParameter
    0 // ProjectileId
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
    0, // actionParameter
    actorIdProjectilePlayerShot // ProjectileId
};

//
// COLUMN
//

const DecorateAnimation columnDecorationAnimation =
{
    { COLUMNPIC, 20, ActionNone }
};

const DecorateState columnStateDecoration =
{
    columnDecorationAnimation,
    StateIdDecoration // Loop
};

const std::map<DecorateStateId, DecorateState> columnStates =
{
    std::make_pair(StateIdDecoration, columnStateDecoration)
};

const DecorateActor decorateColumn =
{
    actorIdColumn,   // Id
    54, // spawnOnAllDifficulties;
    54, // spawnOnNormalAndHard;
    54,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    columnStates,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter,
    0 // ProjectileId
};

//
// SULPHUR GAS
//

// This object is not really hidden, but unlike the other decorative items it is non-solid and projectiles fly through it.
const DecorateAnimation sulphurGasHiddenAnimation =
{
    { SULPHUR_GAS_1PIC, 20, ActionNone },
    { SULPHUR_GAS_2PIC, 20, ActionNone },
    { SULPHUR_GAS_3PIC, 20, ActionNone }
};

const DecorateState sulphurGasStateHidden =
{
    sulphurGasHiddenAnimation,
    StateIdHidden // Loop
};

const std::map<DecorateStateId, DecorateState> sulphurGasStates =
{
    std::make_pair(StateIdHidden, sulphurGasStateHidden)
};

const DecorateActor decorateSulphurGas =
{
    actorIdSulphurGas,   // Id
    55, // spawnOnAllDifficulties;
    55, // spawnOnNormalAndHard;
    55,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    sulphurGasStates,
    StateIdHidden,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter,
    0 // ProjectileId
};

//
// FIRE POT
//

const DecorateAnimation firePotDecorationAnimation =
{
    { FIRE_POT_1PIC, 20, ActionNone },
    { FIRE_POT_2PIC, 20, ActionNone }
};

const DecorateState firePotStateDecoration =
{
    firePotDecorationAnimation,
    StateIdDecoration // Loop
};

const std::map<DecorateStateId, DecorateState> firePotStates =
{
    std::make_pair(StateIdDecoration, firePotStateDecoration)
};

const DecorateActor decorateFirePot =
{
    actorIdFirePot,   // Id
    56, // spawnOnAllDifficulties;
    56, // spawnOnNormalAndHard;
    56,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    firePotStates,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter,
    0 // ProjectileId
};

//
// FOUNTAIN
//

const DecorateAnimation fountainDecorationAnimation =
{
    { WFOUNTAINPIC, 20, ActionNone }
};

const DecorateState  fountainStateDecoration =
{
    fountainDecorationAnimation,
    StateIdDecoration // Loop
};

const std::map<DecorateStateId, DecorateState>  fountainStates =
{
    std::make_pair(StateIdDecoration,  fountainStateDecoration)
};

const DecorateActor decorateFountain =
{
    actorIdFountain,   // Id
    58, // spawnOnAllDifficulties;
    58, // spawnOnNormalAndHard;
    58,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    fountainStates,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter,
    0 // ProjectileId
};

//
// ARCHES
//

const DecorateAnimation arch1DecorationAnimation =
{
    { ARCH1PIC, 20, ActionNone }
};

const DecorateState arch1StateArch =
{
    arch1DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch1States =
{
    std::make_pair(StateIdArch, arch1StateArch)
};

const DecorateActor decorateArch1 =
{
    actorIdArch1,   // Id
    52, // spawnOnAllDifficulties;
    52, // spawnOnNormalAndHard;
    52,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch1States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateAnimation arch2DecorationAnimation =
{
    { ARCH2PIC, 20, ActionNone }
};

const DecorateState arch2StateArch =
{
    arch2DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch2States =
{
    std::make_pair(StateIdArch, arch2StateArch)
};

const DecorateActor decorateArch2 =
{
    actorIdArch2,   // Id
    60, // spawnOnAllDifficulties;
    60, // spawnOnNormalAndHard;
    60, // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch2States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateAnimation arch3DecorationAnimation =
{
    { ARCH3PIC, 20, ActionNone }
};

const DecorateState arch3StateArch =
{
    arch3DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch3States =
{
    std::make_pair(StateIdArch, arch3StateArch)
};

const DecorateActor decorateArch3 =
{
    actorIdArch3,   // Id
    63, // spawnOnAllDifficulties;
    63, // spawnOnNormalAndHard;
    63,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch3States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateAnimation arch4DecorationAnimation =
{
    { ARCH4PIC, 20, ActionNone }
};

const DecorateState arch4StateArch =
{
    arch4DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch4States =
{
    std::make_pair(StateIdArch, arch4StateArch)
};

const DecorateActor decorateArch4 =
{
    actorIdArch4,   // Id
    62, // spawnOnAllDifficulties;
    62, // spawnOnNormalAndHard;
    62,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch4States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateAnimation arch5DecorationAnimation =
{
    { ARCH5PIC, 20, ActionNone }
};

const DecorateState arch5StateArch =
{
    arch5DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch5States =
{
    std::make_pair(StateIdArch, arch5StateArch)
};

const DecorateActor decorateArch5 =
{
    actorIdArch5,   // Id
    63, // spawnOnAllDifficulties;
    63, // spawnOnNormalAndHard;
    63,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch5States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateAnimation arch6DecorationAnimation =
{
    { ARCH6PIC, 20, ActionNone }
};

const DecorateState arch6StateArch =
{
    arch6DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch6States =
{
    std::make_pair(StateIdArch, arch6StateArch)
};

const DecorateActor decorateArch6 =
{
    actorIdArch6,   // Id
    64, // spawnOnAllDifficulties;
    64, // spawnOnNormalAndHard;
    64,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch6States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateAnimation arch7DecorationAnimation =
{
    { ARCH7PIC, 20, ActionNone }
};

const DecorateState arch7StateArch =
{
    arch7DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch7States =
{
    std::make_pair(StateIdArch, arch7StateArch)
};

const DecorateActor decorateArch7 =
{
    actorIdArch7,   // Id
    67, // spawnOnAllDifficulties;
    67, // spawnOnNormalAndHard;
    67,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch7States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateAnimation arch8DecorationAnimation =
{
    { ARCH8PIC, 20, ActionNone }
};

const DecorateState arch8StateArch =
{
    arch8DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch8States =
{
    std::make_pair(StateIdArch, arch8StateArch)
};

const DecorateActor decorateArch8 =
{
    actorIdArch8,   // Id
    68, // spawnOnAllDifficulties;
    68, // spawnOnNormalAndHard;
    68,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch8States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateAnimation arch9DecorationAnimation =
{
    { ARCH9PIC, 20, ActionNone }
};

const DecorateState arch9StateArch =
{
    arch9DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch9States =
{
    std::make_pair(StateIdArch, arch9StateArch)
};

const DecorateActor decorateArch9 =
{
    actorIdArch9,   // Id
    69, // spawnOnAllDifficulties;
    69, // spawnOnNormalAndHard;
    69,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch9States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateAnimation arch10DecorationAnimation =
{
    { ARCH10PIC, 20, ActionNone }
};

const DecorateState arch10StateArch =
{
    arch10DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch10States =
{
    std::make_pair(StateIdArch, arch10StateArch)
};

const DecorateActor decorateArch10 =
{
    actorIdArch10,   // Id
    70, // spawnOnAllDifficulties;
    70, // spawnOnNormalAndHard;
    70,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch10States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateAnimation arch11DecorationAnimation =
{
    { ARCH11PIC, 20, ActionNone }
};

const DecorateState arch11StateArch =
{
    arch11DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch11States =
{
    std::make_pair(StateIdArch, arch11StateArch)
};

const DecorateActor decorateArch11 =
{
    actorIdArch11,   // Id
    71, // spawnOnAllDifficulties;
    71, // spawnOnNormalAndHard;
    71,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch11States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateAnimation arch12DecorationAnimation =
{
    { ARCH12PIC, 20, ActionNone }
};

const DecorateState arch12StateArch =
{
    arch12DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch12States =
{
    std::make_pair(StateIdArch, arch12StateArch)
};

const DecorateActor decorateArch12 =
{
    actorIdArch12,   // Id
    66, // spawnOnAllDifficulties;
    66, // spawnOnNormalAndHard;
    66,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch12States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateAnimation arch13DecorationAnimation =
{
    { ARCH13PIC, 20, ActionNone }
};

const DecorateState arch13StateArch =
{
    arch13DecorationAnimation,
    StateIdArch // Loop
};

const std::map<DecorateStateId, DecorateState> arch13States =
{
    std::make_pair(StateIdArch, arch13StateArch)
};

const DecorateActor decorateArch13 =
{
    actorIdArch13,   // Id
    57, // spawnOnAllDifficulties;
    57, // spawnOnNormalAndHard;
    57,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    arch13States,
    StateIdArch,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// FORCEFIELD
//

const DecorateAnimation forceFieldDecorationAnimation =
{
    { FORCE_FIELD_1PIC, 10, ActionForceField },
    { FORCE_FIELD_2PIC, 10, ActionForceField },
    { FORCE_FIELD_3PIC, 10, ActionForceField },
    { FORCE_FIELD_4PIC, 10, ActionForceField }
};

const DecorateState forceFieldStateArch =
{
    forceFieldDecorationAnimation,
    StateIdArch // Loop
};

const DecorateAnimation forceFieldDyingAnimation =
{
    { 0 , 7, ActionRemove },
};
const DecorateState forceFieldStateDying =
{
    forceFieldDyingAnimation,
    StateIdDying   // Loop
};

const std::map<DecorateStateId, DecorateState> forceFieldStates =
{
    std::make_pair(StateIdArch, forceFieldStateArch),
    std::make_pair(StateIdDying, forceFieldStateDying)
};

const DecorateActor decorateForceField =
{
    actorIdForceField,   // Id
    59, // spawnOnAllDifficulties;
    59, // spawnOnNormalAndHard;
    59,  // spawnOnHard
    20,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    forceFieldStates,
    StateIdArch,   // initialState;
    20,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};