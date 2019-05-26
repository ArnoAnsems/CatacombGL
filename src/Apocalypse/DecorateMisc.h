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
// Actor definitions for the Catacomb Apocalypse decorative items, exploding walls, the player, etc.
//
#pragma once

#include "..\Engine\Decorate.h"
#include "DecorateProjectiles.h"
#include "EgaGraphApocalypse.h"
#include "AudioRepositoryApocalypse.h"

const uint16_t actorIdNone = 0;
const uint16_t actorIdExplodingWall = 4;
const uint16_t actorIdPlayer = 11;
const uint16_t actorIdColumn1 = 12;
const uint16_t actorIdColumn2 = 13;
const uint16_t actorIdColumn3 = 14;
const uint16_t actorIdColumn4 = 15;
const uint16_t actorIdColumn5 = 16;

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
// COLUMN 1
//

const DecorateAnimation column1DecorationAnimation =
{
    { COLUMN1PIC, 20, ActionNone }
};

const DecorateState column1StateDecoration =
{
    column1DecorationAnimation,
    StateIdDecoration // Loop
};

const std::map<DecorateStateId, DecorateState> column1States =
{
    std::make_pair(StateIdDecoration, column1StateDecoration)
};

const DecorateActor decorateColumn1 =
{
    actorIdColumn1,   // Id
    36, // spawnOnAllDifficulties;
    36, // spawnOnNormalAndHard;
    36,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    column1States,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter,
    0 // ProjectileId
};

//
// COLUMN 2
//

const DecorateAnimation column2DecorationAnimation =
{
    { COLUMN2PIC, 20, ActionNone }
};

const DecorateState column2StateDecoration =
{
    column2DecorationAnimation,
    StateIdDecoration // Loop
};

const std::map<DecorateStateId, DecorateState> column2States =
{
    std::make_pair(StateIdDecoration, column2StateDecoration)
};

const DecorateActor decorateColumn2 =
{
    actorIdColumn2,   // Id
    45, // spawnOnAllDifficulties;
    45, // spawnOnNormalAndHard;
    45,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    column2States,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter,
    0 // ProjectileId
};

//
// COLUMN 3
//

const DecorateAnimation column3DecorationAnimation =
{
    { COLUMN3PIC, 20, ActionNone }
};

const DecorateState column3StateDecoration =
{
    column3DecorationAnimation,
    StateIdDecoration // Loop
};

const std::map<DecorateStateId, DecorateState> column3States =
{
    std::make_pair(StateIdDecoration, column3StateDecoration)
};

const DecorateActor decorateColumn3 =
{
    actorIdColumn3,   // Id
    46, // spawnOnAllDifficulties;
    46, // spawnOnNormalAndHard;
    46,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    column3States,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter,
    0 // ProjectileId
};

//
// COLUMN 4
//

const DecorateAnimation column4DecorationAnimation =
{
    { COLUMN4PIC, 20, ActionNone }
};

const DecorateState column4StateDecoration =
{
    column4DecorationAnimation,
    StateIdDecoration // Loop
};

const std::map<DecorateStateId, DecorateState> column4States =
{
    std::make_pair(StateIdDecoration, column4StateDecoration)
};

const DecorateActor decorateColumn4 =
{
    actorIdColumn4,   // Id
    50, // spawnOnAllDifficulties;
    50, // spawnOnNormalAndHard;
    50,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    column4States,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter,
    0 // ProjectileId
};

//
// COLUMN 5
//

const DecorateAnimation column5DecorationAnimation =
{
    { COLUMN5PIC, 20, ActionNone }
};

const DecorateState column5StateDecoration =
{
    column5DecorationAnimation,
    StateIdDecoration // Loop
};

const std::map<DecorateStateId, DecorateState> column5States =
{
    std::make_pair(StateIdDecoration, column5StateDecoration)
};

const DecorateActor decorateColumn5 =
{
    actorIdColumn5,   // Id
    31, // spawnOnAllDifficulties;
    31, // spawnOnNormalAndHard;
    31,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    column5States,
    StateIdDecoration,   // initialState;
    0,  // meleeDamage;
    SHOOTWALLSND, // hitSound;
    0,    // speed;
    0, // actionParameter,
    0 // ProjectileId
};