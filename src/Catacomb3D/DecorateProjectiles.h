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
// DecorateProjectiles
//
// Actor definitions for the Catacomb Armageddon projectiles.
//
#pragma once

#include "..\Engine\Decorate.h"
#include "EgaGraphCatacomb3D.h"
#include "AudioRepositoryCatacomb3D.h"

const uint16_t actorIdProjectilePlayerShot = 70;
const uint16_t actorIdProjectileMageShot = 72;

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
    { PSHOT2PIC , 8, ActionNone },
    { 0 , 8, ActionRemove },
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
    14 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    mageShotStates,
    StateIdProjectileFly,   // initialState;
    4,  // damage;
    0, // hitSound;
    10000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};
