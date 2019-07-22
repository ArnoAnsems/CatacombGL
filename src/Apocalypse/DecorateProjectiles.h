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
#include "DecorateBonus.h"
#include "EgaGraphApocalypse.h"
#include "AudioRepositoryApocalypse.h"

const uint16_t actorIdProjectilePlayerShot = 70;
const uint16_t actorIdProjectilePlayerBigShot = 71;
const uint16_t actorIdProjectileWizardShot = 72;
const uint16_t actorIdProjectileBlobShot = 73;
const uint16_t actorIdProjectileBugShot = 74;
const uint16_t actorIdProjectileTimeLordShot = 75;
const uint16_t actorIdProjectileAndroidMageShot = 76;
const uint16_t actorIdProjectileStompyShot = 77;
const uint16_t actorIdProjectileRayShot = 78;
const uint16_t actorIdProjectileSkeletonShot = 79;
const uint16_t actorIdProjectileRoboTankShot = 80;
const uint16_t actorIdProjectileEyeShot = 81;
const uint16_t actorIdProjectileNemesisShot = 82;

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

//
// BUG SHOT
//

const DecorateAnimation bugShotFlyAnimation =
{
    { BUG_SHOT1PIC, 10, ActionMonsterProjectile },
    { BUG_SHOT1PIC, 10, ActionMonsterProjectile }
};

const DecorateState bugShotStateFly =
{
    bugShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const std::map<DecorateStateId, DecorateState> bugShotStates =
{
    std::make_pair(StateIdProjectileFly, bugShotStateFly),
    std::make_pair(StateIdDying, playerShotStateDying)
};

const DecorateActor decorateBugShot =
{
    actorIdProjectileBugShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    bugShotStates,
    StateIdProjectileFly,   // initialState;
    3,  // damage;
    0, // hitSound;
    8000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// TIME LORD (EGYPTIAN HEAD) SHOT
//

const DecorateAnimation timeLordShotFlyAnimation =
{
    { PSHOT1PIC, 10, ActionMonsterProjectile },
    { PSHOT2PIC, 10, ActionMonsterProjectile }
};

const DecorateState timeLordShotStateFly =
{
    timeLordShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const std::map<DecorateStateId, DecorateState> timeLordShotStates =
{
    std::make_pair(StateIdProjectileFly, timeLordShotStateFly),
    std::make_pair(StateIdDying, bonusStateDying)
};

const DecorateActor decorateTimeLordShot =
{
    actorIdProjectileTimeLordShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    bugShotStates,
    StateIdProjectileFly,   // initialState;
    5,  // damage;
    0, // hitSound;
    10000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// ANDROID MAGE SHOT
//

const DecorateAnimation androidMageShotFlyAnimation =
{
    { FMAGESHOT1PIC , 8, ActionMonsterProjectile },
    { FMAGESHOT2PIC , 8, ActionMonsterProjectile }
};

const DecorateState androidMageShotStateFly =
{
    androidMageShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const std::map<DecorateStateId, DecorateState> androidMageShotStates =
{
    std::make_pair(StateIdProjectileFly, androidMageShotStateFly),
    std::make_pair(StateIdDying, bonusStateDying)
};

const DecorateActor decorateAndroidMageShot =
{
    actorIdProjectileAndroidMageShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    androidMageShotStates,
    StateIdProjectileFly,   // initialState;
    7,  // damage;
    0, // hitSound;
    10000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// STOMPY SHOT
//

const DecorateAnimation stompyShotRiseAnimation =
{
    { STOMPYSHOT1PIC, 6, ActionMonsterProjectile },
    { STOMPYSHOT2PIC, 6, ActionMonsterProjectile },
    { STOMPYSHOT1PIC, 6, ActionMonsterProjectile },
};

const DecorateState stompyShotStateRise =
{
    stompyShotRiseAnimation,
    StateIdProjectileFly   // Loop
};

const DecorateAnimation stompyShotFlyAnimation =
{
    { STOMPYSHOT3PIC, 6, ActionMonsterProjectile },
    { STOMPYSHOT4PIC, 6, ActionMonsterProjectile }
};

const DecorateState stompyShotStateFly =
{
    stompyShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const std::map<DecorateStateId, DecorateState> stompyShotStates =
{
    std::make_pair(StateIdRise, stompyShotStateRise),
    std::make_pair(StateIdProjectileFly, stompyShotStateFly),
    std::make_pair(StateIdDying, bonusStateDying)
};

const DecorateActor decorateStompyShot =
{
    actorIdProjectileStompyShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    stompyShotStates,
    StateIdRise,   // initialState;
    7,  // damage;
    0, // hitSound;
    8500,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// MANTA RAY SHOT
//

const DecorateAnimation rayShotFlyAnimation =
{
    { RAYSHOT1PIC , 8, ActionMonsterProjectile },
    { RAYSHOT2PIC , 8, ActionMonsterProjectile }
};

const DecorateState rayShotStateFly =
{
    rayShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const std::map<DecorateStateId, DecorateState> rayShotStates =
{
    std::make_pair(StateIdProjectileFly, rayShotStateFly),
    std::make_pair(StateIdDying, playerShotStateDying)
};

const DecorateActor decorateRayShot =
{
    actorIdProjectileRayShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    rayShotStates,
    StateIdProjectileFly,   // initialState;
    5,  // damage;
    0, // hitSound;
    10000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// SKELETON SHOT
//

const DecorateAnimation skeletonShotFlyAnimation =
{
    { RAMBONESHOT1PIC , 10, ActionMonsterProjectile },
    { RAMBONESHOT2PIC , 10, ActionMonsterProjectile }
};

const DecorateState skeletonShotStateFly =
{
    skeletonShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const std::map<DecorateStateId, DecorateState> skeletonShotStates =
{
    std::make_pair(StateIdProjectileFly, skeletonShotStateFly),
    std::make_pair(StateIdDying, bonusStateDying)
};

const DecorateActor decorateSkeletonShot =
{
    actorIdProjectileSkeletonShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    skeletonShotStates,
    StateIdProjectileFly,   // initialState;
    7,  // damage;
    0, // hitSound;
    10000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// ROBO TANK SHOT
//

const DecorateAnimation roboTankShotFlyAnimation =
{
    { PSHOT1PIC, 10, ActionMonsterProjectile },
    { PSHOT2PIC, 10, ActionMonsterProjectile }
};

const DecorateState roboTankShotStateFly =
{
    roboTankShotFlyAnimation,
    StateIdProjectileFly   // Loop
};

const std::map<DecorateStateId, DecorateState> roboTankShotStates =
{
    std::make_pair(StateIdProjectileFly, roboTankShotStateFly),
    std::make_pair(StateIdDying, bonusStateDying)
};

const DecorateActor decorateRoboTankShot =
{
    actorIdProjectileRoboTankShot,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    bugShotStates,
    StateIdProjectileFly,   // initialState;
    15,  // damage;
    0, // hitSound;
    7000,    // speed;
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
    5,  // damage;
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

