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
// DecorateBonus
//
// Actor definitions for the Catacomb Apocalypse bonus items.
//
#pragma once

#include "..\Engine\Decorate.h"
#include "EgaGraphApocalypse.h"
#include "AudioRepositoryApocalypse.h"

const uint16_t actorIdBonusExplosion = 20;

//
// BONUS ITEM DESTROYED
//

const DecorateAnimation bonusDyingAnimation =
{
    { PSHOT_EXP1PIC, 7, ActionItemDestroyed },
    { PSHOT_EXP2PIC , 7, ActionNone },
    { PSHOT_EXP3PIC , 7, ActionNone },
    { 0 , 7, ActionRemove },
};
const DecorateState bonusStateDying =
{
    bonusDyingAnimation,
    StateIdDying   // Loop
};

//
// EXPLOSION
//

const DecorateAnimation explosionHiddenAnimation =
{
    { 0 , 2, ActionExplosionDelay }
};

const DecorateState explosionStateHidden =
{
    explosionHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation explosionDyingAnimation =
{
    { PSHOT_EXP1PIC , 7, ActionNone },
    { PSHOT_EXP2PIC , 7, ActionNone },
    { PSHOT_EXP3PIC , 7, ActionNone },
    { 0 , 7, ActionRemove },
};

const DecorateState explosionStateDying =
{
    explosionDyingAnimation,
    StateIdDying   // Loop
};

const std::map<DecorateStateId, DecorateState> explosionStates =
{
    std::make_pair(StateIdHidden, explosionStateHidden),
    std::make_pair(StateIdDying, explosionStateDying)
};

const DecorateActor decorateExplosion =
{
    actorIdBonusExplosion,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    explosionStates,
    StateIdHidden,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};
