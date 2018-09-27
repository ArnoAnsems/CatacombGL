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
const uint16_t actorIdPlayer = 1;

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

