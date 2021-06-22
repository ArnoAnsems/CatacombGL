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
// DecorateProjectiles
//
// Actor definitions for the Catacomb Abyss projectiles.
//
#pragma once

#include "../Engine/Decorate.h"
#include "EgaGraphAbyss.h"
#include "AudioRepositoryAbyss.h"

const uint16_t actorIdProjectilePlayerShot = 70;
const uint16_t actorIdProjectilePlayerBigShot = 71;
const uint16_t actorIdProjectileMageShot = 72;
const uint16_t actorIdProjectileEyeShot = 73;
const uint16_t actorIdProjectileNemesisShot = 74;

//
// PLAYER SHOT
//

const DecorateAnimation playerShotFlyAnimation =
{
    { PSHOT1PIC , 8, ActionPlayerProjectile },
    { PSHOT2PIC , 8, ActionPlayerProjectile }
};

const DecorateState playerShotStateFly = 
{
    playerShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const DecorateAnimation playerShotDyingAnimation =
{
    { PSHOT_EXP1PIC , 7, ActionNone },
    { PSHOT_EXP2PIC , 7, ActionNone },
    { PSHOT_EXP3PIC , 7, ActionNone },
    { 0 , 7, ActionRemove },
};

const DecorateState playerShotStateDying = 
{
    playerShotDyingAnimation,
    StateIdDying   // Loop
};

const std::map<DecorateStateId, DecorateState> playerShotStates =
{
    std::make_pair(StateIdProjectileFly, playerShotStateFly),
    std::make_pair(StateIdDying, playerShotStateDying)
};

const DecorateActor decoratePlayerShot =
{
    actorIdProjectilePlayerShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    7 * pixelRadius,    // size;
    Flickering,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    playerShotStates,
    StateIdProjectileFly,   // initialState;
    1,  // damage;
    0, // hitSound;
    10000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// PLAYER BIG SHOT
// Fired by the nuke / X-terminator
//

const DecorateActor decoratePlayerBigShot =
{
    actorIdProjectilePlayerBigShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    24 * pixelRadius,    // size;
    Flickering,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    playerShotStates,
    StateIdProjectileFly,   // initialState;
    3,  // damage;
    0, // hitSound;
    10000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// MAGE SHOT
//

const DecorateAnimation mageShotFlyAnimation =
{
    { PSHOT1PIC , 8, ActionMonsterProjectile },
    { PSHOT2PIC , 8, ActionMonsterProjectile }
};

const DecorateState mageShotStateFly = 
{
    mageShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const std::map<DecorateStateId, DecorateState> mageShotStates =
{
    std::make_pair(StateIdProjectileFly, mageShotStateFly),
    std::make_pair(StateIdDying, playerShotStateDying)
};

const DecorateActor decorateMageShot =
{
    actorIdProjectileMageShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    mageShotStates,
    StateIdProjectileFly,   // initialState;
    2,  // damage;
    0, // hitSound;
    10000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// EYE SHOT
//

const DecorateAnimation eyeShotFlyAnimation =
{
    { EYE_SHOT1PIC , 8, ActionMonsterProjectile },
    { EYE_SHOT2PIC , 8, ActionMonsterProjectile }
};

const DecorateState eyeShotStateFly = 
{
    eyeShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const std::map<DecorateStateId, DecorateState> eyeShotStates =
{
    std::make_pair(StateIdProjectileFly, eyeShotStateFly),
    std::make_pair(StateIdDying, playerShotStateDying)
};

const DecorateActor decorateEyeShot =
{
    actorIdProjectileEyeShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    eyeShotStates,
    StateIdProjectileFly,   // initialState;
    1,  // damage;
    0, // hitSound;
    5000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// NEMESIS SHOT (FLAMING SKULL)
//

const DecorateAnimation nemesisShotFlyAnimation =
{
    { SKULL_SHOTPIC , 8, ActionMonsterProjectile }
};

const DecorateState nemesisShotStateFly = 
{
    nemesisShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const std::map<DecorateStateId, DecorateState> nemesisShotStates =
{
    std::make_pair(StateIdProjectileFly, nemesisShotStateFly),
    std::make_pair(StateIdDying, playerShotStateDying)
};

const DecorateActor decorateNemesisShot =
{
    actorIdProjectileNemesisShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    nemesisShotStates,
    StateIdProjectileFly,   // initialState;
    25,  // damage;
    0, // hitSound;
    10000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

