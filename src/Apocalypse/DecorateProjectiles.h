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
// Actor definitions for the Catacomb Apocalypse projectiles.
//
#pragma once

#include "..\Engine\Decorate.h"
#include "EgaGraphApocalypse.h"
#include "AudioRepositoryApocalypse.h"

const uint16_t actorIdProjectilePlayerShot = 70;
const uint16_t actorIdProjectilePlayerBigShot = 71;
const uint16_t actorIdProjectileWizardShot = 72;
const uint16_t actorIdProjectileBlobShot = 73;

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
// WIZARD SHOT
//

const DecorateAnimation wizardShotFlyAnimation =
{
    { WIZARD_SHOT1PIC , 8, ActionMonsterProjectile },
    { WIZARD_SHOT2PIC , 8, ActionMonsterProjectile }
};

const DecorateState wizardShotStateFly =
{
    wizardShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const std::map<DecorateStateId, DecorateState> wizardShotStates =
{
    std::make_pair(StateIdProjectileFly, wizardShotStateFly),
    std::make_pair(StateIdDying, playerShotStateDying)
};

const DecorateActor decorateWizardShot =
{
    actorIdProjectileWizardShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    wizardShotStates,
    StateIdProjectileFly,   // initialState;
    3,  // damage;
    0, // hitSound;
    10000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// BLOB SHOT
//

const DecorateAnimation blobShotFlyAnimation =
{
    { BLOB_SHOT1PIC , 8, ActionMonsterProjectile },
    { BLOB_SHOT2PIC , 8, ActionMonsterProjectile }
};

const DecorateState blobShotStateFly =
{
    blobShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const std::map<DecorateStateId, DecorateState> blobShotStates =
{
    std::make_pair(StateIdProjectileFly, blobShotStateFly),
    std::make_pair(StateIdDying, playerShotStateDying)
};

const DecorateActor decorateBlobShot =
{
    actorIdProjectileBlobShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    blobShotStates,
    StateIdProjectileFly,   // initialState;
    2,  // damage;
    0, // hitSound;
    10000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};
