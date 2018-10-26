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
#include "EgaGraphArmageddon.h"
//#include "AudioRepositoryArmageddon.h"

const uint16_t actorIdNone = 0;
const uint16_t actorIdTomb1 = 1;
const uint16_t actorIdTomb2 = 2;
const uint16_t actorIdTomb3 = 3;
const uint16_t actorIdPlayer = 11;
const uint16_t actorIdColumn = 12;
const uint16_t actorIdSulphurGas = 13;
const uint16_t actorIdFirePot = 14;
const uint16_t actorIdFountain = 15;

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
    0 // ProjectileId
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

const DecorateAnimation sulphurGasDecorationAnimation =
{
    { SULPHUR_GAS_1PIC, 20, ActionNone },
    { SULPHUR_GAS_2PIC, 20, ActionNone },
    { SULPHUR_GAS_3PIC, 20, ActionNone }
};

const DecorateState sulphurGasStateDecoration =
{
    sulphurGasDecorationAnimation,
    StateIdDecoration // Loop
};

const std::map<DecorateStateId, DecorateState> sulphurGasStates =
{
    std::make_pair(StateIdDecoration, sulphurGasStateDecoration)
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
    StateIdDecoration,   // initialState;
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
