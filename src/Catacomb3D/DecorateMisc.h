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
// DecorateMisc
//
// Actor definitions for the Catacomb 3-D decorative items, exploding walls, the player, etc.
//
#pragma once

#include "../Engine/Decorate.h"
#include "DecorateProjectiles.h"
#include "EgaGraphCatacomb3D.h"

const uint16_t actorIdNone = 0;
const uint16_t actorIdExplodingWall = 4;
const uint16_t actorIdWarpToLevel = 5;
const uint16_t actorIdWarpPortal1 = 6;
const uint16_t actorIdWarpPortal2 = 7;
const uint16_t actorIdWarpPortal3 = 8;
const uint16_t actorIdWarpPortal4 = 9;
const uint16_t actorIdPlayer = 11;


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
// WARP TO LEVEL
//

const DecorateAnimation warpToLevelWaitForPickupAnimation =
{
    { WARP1PIC, 6, ActionWaitForPickup },
    { WARP2PIC, 6, ActionWaitForPickup },
    { WARP3PIC, 6, ActionWaitForPickup },
    { WARP4PIC, 6, ActionWaitForPickup }
};

const DecorateState warpToLevelStateWaitForPickup =
{
    warpToLevelWaitForPickupAnimation,
    StateIdWaitForPickup // Loop
};

const DecorateAnimation warpToLevelPickupAnimation =
{
    { WARP1PIC, 12, ActionWarpToOtherLevel }
};

const DecorateState warpToLevelStatePickup =
{
    warpToLevelPickupAnimation,
    StateIdWaitForPickup
};

const std::map<DecorateStateId, DecorateState> warpToLevelStates =
{
    std::make_pair(StateIdWaitForPickup, warpToLevelStateWaitForPickup),
    std::make_pair(StateIdPickup, warpToLevelStatePickup)
};

const DecorateActor decorateWarpToLevel =
{
    actorIdWarpToLevel,   // Id
    24, // spawnOnAllDifficulties;
    24, // spawnOnNormalAndHard;
    24,  // spawnOnHard
    1,  // initialHealth;
    0.333f,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    warpToLevelStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// WARP PORTAL
//

const DecorateAnimation warpPortalWaitForPickupAnimation =
{
    { WARP1PIC, 12, ActionWaitForPickup },
    { WARP2PIC, 12, ActionWaitForPickup },
    { WARP3PIC, 12, ActionWaitForPickup },
    { WARP4PIC, 12, ActionWaitForPickup }
};

const DecorateState warpPortalStateWaitForPickup =
{
    warpPortalWaitForPickupAnimation,
    StateIdWaitForPickup // Loop
};

const DecorateAnimation warpPortalPickupAnimation =
{
    { WARP1PIC, 8, ActionWarpInsideLevel }
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
