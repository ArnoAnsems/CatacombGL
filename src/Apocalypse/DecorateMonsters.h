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
// DecorateMonsters
//
// Actor definitions for the Catacomb Apocalypse monsters.
//
#pragma once

#include "..\Engine\Decorate.h"
#include "DecorateProjectiles.h"
#include "EgaGraphApocalypse.h"
#include "AudioRepositoryApocalypse.h"

const uint16_t actorIdMonsterBlob = 54;
const uint16_t actorIdMonsterTimeLord = 55;
const uint16_t actorIdMonsterAndroidMage = 56;
const uint16_t actorIdMonsterCyborgDemon = 57;
const uint16_t actorIdMonsterStompy = 58;
const uint16_t actorIdMonsterWizard = 59;
const uint16_t actorIdMonsterBug = 60;
const uint16_t actorIdMonsterRunningEye = 61;
const uint16_t actorIdMonsterDemon = 62;
const uint16_t actorIdMonsterAquaMan = 63;
const uint16_t actorIdMonsterTroll = 64;

//
// WIZARD
//

const DecorateAnimation wizardWalkAnimation =
{
    { WIZARDWALK1PIC, 20, ActionChaseLikeMage },
    { WIZARDWALK2PIC, 20, ActionChaseLikeMage },
    { WIZARDWALK3PIC, 20, ActionChaseLikeMage },
    { WIZARDWALK4PIC, 20, ActionChaseLikeMage }
};

const DecorateState wizardStateWalk =
{
    wizardWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation wizardAttackAnimation =
{
    { WIZARDATTACK1PIC, 20, ActionAttack },
    { WIZARDATTACK1PIC, 20, ActionChaseLikeMage }
};

const DecorateState wizardStateAttack =
{
    wizardAttackAnimation,
    StateIdWalk
};

const DecorateAnimation wizardPainAnimation =
{
    { WIZARDOUCHPIC, 15, ActionNone }
};

const DecorateState wizardStatePain =
{
    wizardPainAnimation,
    StateIdWalk
};

const DecorateAnimation wizardDyingAnimation =
{
    { WIZARDDEATH1PIC, 45, ActionSmallMonsterSound },
    { WIZARDDEATH2PIC, 30, ActionNone },
    { WIZARDDEATH3PIC, 15, ActionNone },
    { WIZARDDEATH4PIC, 15, ActionNone }
};

const DecorateState wizardStateDying =
{
    wizardDyingAnimation,
    StateIdDead
};

const DecorateAnimation wizardDeadAnimation =
{
    { WIZARDDEATH4PIC, 20, ActionNone }
};

const DecorateState wizardStateDead =
{
    wizardDeadAnimation,
    StateIdDead // Loop
};

const std::map<DecorateStateId, DecorateState> wizardStates =
{
    std::make_pair(StateIdWalk, wizardStateWalk),
    std::make_pair(StateIdAttack, wizardStateAttack),
    std::make_pair(StateIdPain, wizardStatePain),
    std::make_pair(StateIdDying, wizardStateDying),
    std::make_pair(StateIdDead, wizardStateDead)
};

const DecorateActor decorateWizard =
{
    actorIdMonsterWizard,   // Id
    23, // spawnOnAllDifficulties;
    23, // spawnOnNormalAndHard;
    23,  // spawnOnHard
    10,  // initialHealth;
    35 * pixelRadius,    // size;
    WithAllGems,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    wizardStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    1536,    // speed;
    0, // actionParameter
    actorIdProjectileWizardShot // ProjectileId
};

//
// BLOB
//

const DecorateAnimation blobHiddenAnimation =
{
    { BLOBGND1PIC, 13, ActionHideUnderWater },
    { BLOBGND1PIC, 15, ActionHideUnderWater }
};

const DecorateState blobStateHidden =
{
    blobHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation blobRiseAnimation =
{
    { BLOBRISE1PIC, 20, ActionNone },
    { BLOBRISE2PIC, 20, ActionNone }
};

const DecorateState blobStateRise =
{
    blobRiseAnimation,
    StateIdWalk
};

const DecorateAnimation blobSinkAnimation =
{
    { BLOBRISE2PIC, 20, ActionNone },
    { BLOBRISE1PIC, 20, ActionNone }
};

const DecorateState blobStateSink =
{
    blobSinkAnimation,
    StateIdHidden
};

const DecorateAnimation blobWalkAnimation =
{
    { BLOBWALK1PIC, 15, ActionChaseLikeWetMan },
    { BLOBWALK2PIC, 15, ActionChaseLikeWetMan },
    { BLOBWALK3PIC, 15, ActionChaseLikeWetMan }
};

const DecorateState blobStateWalk =
{
    blobWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation blobAttackAnimation =
{
    { BLOBWALK1PIC, 1, ActionAttack }
};

const DecorateState blobStateAttack =
{
    blobAttackAnimation,
    StateIdWalk
};


const DecorateAnimation blobPainAnimation =
{
    { BLOBRISE2PIC, 10, ActionChaseLikeWetMan }
};

const DecorateState blobStatePain =
{
    blobPainAnimation,
    StateIdWalk
};

const DecorateAnimation blobDyingAnimation =
{
    { BLOBDEATH1PIC, 30, ActionExplodeSound },
    { BLOBDEATH2PIC, 30, ActionNone },
    { BLOBDEATH3PIC, 30, ActionNone },
    { 0, 30, ActionRemove }
};

const DecorateState blobStateDying =
{
    blobDyingAnimation,
    StateIdDying  // Will not loop, due to the ActionRemove.
};

const std::map<DecorateStateId, DecorateState> blobStates =
{
    std::make_pair(StateIdHidden, blobStateHidden),
    std::make_pair(StateIdRise, blobStateRise),
    std::make_pair(StateIdSink, blobStateSink),
    std::make_pair(StateIdWalk, blobStateWalk),
    std::make_pair(StateIdAttack, blobStateAttack),
    std::make_pair(StateIdPain, blobStatePain),
    std::make_pair(StateIdDying, blobStateDying)
};

const DecorateActor decorateBlob =
{
    actorIdMonsterBlob,   // Id
    13, // spawnOnAllDifficulties;
    13, // spawnOnNormalAndHard;
    13,  // spawnOnHard
    13,  // initialHealth;
    14 * pixelRadius,    // size;
    WithRedGem,  // radarVisibility;
    EgaBrightRed,   // radarColor;
    blobStates,
    StateIdHidden,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2200,    // speed;
    0, // actionParameter
    actorIdProjectileBlobShot // ProjectileId
};

//
// BUG
//

const DecorateAnimation bugWalkAnimation =
{
    { BUG_WALK1PIC, 15, ActionChaseLikeMage },
    { BUG_WALK2PIC, 15, ActionChaseLikeMage },
    { BUG_WALK3PIC, 15, ActionChaseLikeMage }
};

const DecorateState bugStateWalk =
{
    bugWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation bugAttackAnimation =
{
    { BUG_ATTACK1PIC, 20, ActionNone },
    { BUG_ATTACK2PIC, 20, ActionNone },
    { BUG_ATTACK2PIC, 1, ActionAttack },
    { BUG_ATTACK1PIC, 15, ActionAttack }
};

const DecorateState bugStateAttack =
{
    bugAttackAnimation,
    StateIdWalk
};

const DecorateAnimation bugPainAnimation =
{
    { BUG_WALK1PIC, 10, ActionNone }
};

const DecorateState bugStatePain =
{
    bugPainAnimation,
    StateIdWalk
};

const DecorateAnimation bugDyingAnimation =
{
    { BUG_DEATH1PIC, 35, ActionSmallMonsterSound },
    { BUG_DEATH2PIC, 10, ActionNone }
};

const DecorateState bugStateDying =
{
    bugDyingAnimation,
    StateIdDead
};

const DecorateAnimation bugDeadAnimation =
{
    { BUG_DEATH2PIC, 20, ActionNone }
};

const DecorateState bugStateDead =
{
    bugDeadAnimation,
    StateIdDead // Loop
};

const std::map<DecorateStateId, DecorateState> bugStates =
{
    std::make_pair(StateIdWalk, bugStateWalk),
    std::make_pair(StateIdAttack, bugStateAttack),
    std::make_pair(StateIdPain, bugStatePain),
    std::make_pair(StateIdDying, bugStateDying),
    std::make_pair(StateIdDead, bugStateDead)
};

const DecorateActor decorateBug =
{
    actorIdMonsterBug,   // Id
    14, // spawnOnAllDifficulties;
    14, // spawnOnNormalAndHard;
    14,  // spawnOnHard
    10,  // initialHealth;
    20 * pixelRadius,    // size;
    WithYellowGem,  // radarVisibility;
    EgaLightGray,   // radarColor;
    bugStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    1500,    // speed;
    0, // actionParameter
    actorIdProjectileBugShot // ProjectileId
};

//
// TIME LORD (EGYPTIAN HEAD)
//

const DecorateAnimation timeLordHiddenAnimation =
{
    { 0 , 13, ActionHide }
};

const DecorateState timeLordStateHidden =
{
    timeLordHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation timeLordRiseAnimation =
{
    { HEADPIC, 1, ActionNone }
};

const DecorateState timeLordStateRise =
{
    timeLordRiseAnimation,
    StateIdWalk
};

const DecorateAnimation timeLordWalkAnimation =
{
    { HEADPIC, 20, ActionChaseLikeMage }
};

const DecorateState timeLordStateWalk =
{
    timeLordWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation timeLordAttackAnimation =
{
    { HEADPIC, 20, ActionAttack }
};

const DecorateState timeLordStateAttack =
{
    timeLordAttackAnimation,
    StateIdWalk
};

const DecorateAnimation timeLordDyingAnimation =
{
    { 0, 1, ActionRemove }
};

const DecorateState timeLordStateDying =
{
    timeLordDyingAnimation,
    StateIdDying
};

const std::map<DecorateStateId, DecorateState> timeLordStates =
{
    std::make_pair(StateIdHidden, timeLordStateHidden),
    std::make_pair(StateIdRise, timeLordStateRise),
    std::make_pair(StateIdWalk, timeLordStateWalk),
    std::make_pair(StateIdAttack, timeLordStateAttack),
    std::make_pair(StateIdDying, timeLordStateDying)
};

const DecorateActor decorateTimeLord =
{
    actorIdMonsterTimeLord,   // Id
    29, // spawnOnAllDifficulties;
    29, // spawnOnNormalAndHard;
    29, // spawnOnHard;
    16,  // initialHealth;
    35 * pixelRadius,    // size;
    WithYellowGem,  // radarVisibility;
    EgaBrown,   // radarColor;
    timeLordStates,
    StateIdHidden,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    3000,    // speed;
    0, // actionParameter
    actorIdProjectileTimeLordShot // ProjectileId
};

//
// ANDROID MAGE
//

const DecorateAnimation androidMageWalkAnimation =
{
    { FMAGEWALK1PIC, 20, ActionChaseLikeMage },
    { FMAGEWALK2PIC, 20, ActionChaseLikeMage },
    { FMAGEWALK3PIC, 20, ActionChaseLikeMage },
};

const DecorateState androidMageStateWalk =
{
    androidMageWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation androidMageAttackAnimation =
{
    { FMAGEATTACK1PIC, 20, ActionNone },
    { FMAGEATTACK1PIC, 1, ActionAttack },
    { FMAGEATTACK2PIC, 30, ActionNone },
};

const DecorateState androidMageStateAttack =
{
    androidMageAttackAnimation,
    StateIdWalk
};

const DecorateAnimation androidMagePainAnimation =
{
    { FMAGEATTACK1PIC, 10, ActionNone }
};

const DecorateState androidMageStatePain =
{
    androidMagePainAnimation,
    StateIdWalk
};

const DecorateAnimation androidMageDyingAnimation =
{
    { FMAGEDEATH1PIC, 40, ActionNone },
    { FMAGEDEATH2PIC, 30, ActionSmallMonsterSound },
};

const DecorateState androidMageStateDying =
{
    androidMageDyingAnimation,
    StateIdDead
};

const DecorateAnimation androidMageDeadAnimation =
{
    { FMAGEDEATH3PIC, 20, ActionNone }
};

const DecorateState androidMageStateDead =
{
    androidMageDeadAnimation,
    StateIdDead // Loop
};

const std::map<DecorateStateId, DecorateState> androidMageStates =
{
    std::make_pair(StateIdWalk, androidMageStateWalk),
    std::make_pair(StateIdAttack, androidMageStateAttack),
    std::make_pair(StateIdPain, androidMageStatePain),
    std::make_pair(StateIdDying, androidMageStateDying),
    std::make_pair(StateIdDead, androidMageStateDead)
};

const DecorateActor decorateAndroidMage =
{
    actorIdMonsterAndroidMage,   // Id
    17, // spawnOnAllDifficulties;
    17, // spawnOnNormalAndHard;
    17,  // spawnOnHard
    12,  // initialHealth;
    35 * pixelRadius,    // size;
    WithBlueGem,  // radarVisibility;
    EgaBrightBlue,   // radarColor;
    androidMageStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    3072,    // speed;
    0, // actionParameter
    actorIdProjectileAndroidMageShot // ProjectileId
};

//
// CYBORG DEMON
//

const DecorateAnimation cyborgDemonWalkAnimation =
{
    { CYBORG1PIC, 20, ActionChase },
    { CYBORG2PIC, 20, ActionChase },
    { CYBORG3PIC, 20, ActionChase },
    { CYBORG4PIC, 20, ActionChase }
};

const DecorateState cyborgDemonStateWalk =
{
    cyborgDemonWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation cyborgDemonAttackAnimation =
{
    { CYBORGATTACK1PIC, 20, ActionNone },
    { CYBORGATTACK2PIC, 20, ActionNone },
    { CYBORGATTACK3PIC, 30, ActionAttack }
};

const DecorateState cyborgDemonStateAttack =
{
    cyborgDemonAttackAnimation,
    StateIdWalk
};

const DecorateAnimation cyborgDemonPainAnimation =
{
    { CYBORGOUCHPIC, 30, ActionNone }
};

const DecorateState cyborgDemonStatePain =
{
    cyborgDemonPainAnimation,
    StateIdWalk
};

const DecorateAnimation cyborgDemonDyingAnimation =
{
    { CYBORGOUCHPIC, 40, ActionNone },
    { CYBORGDIE1PIC, 30, ActionLargeMonsterSound }
};

const DecorateState cyborgDemonStateDying =
{
    cyborgDemonDyingAnimation,
    StateIdDead
};

const DecorateAnimation cyborgDemonDeadAnimation =
{
    { CYBORGDIE2PIC, 20, ActionNone }
};

const DecorateState cyborgDemonStateDead =
{
    cyborgDemonDeadAnimation,
    StateIdDead //  Loop
};

const std::map<DecorateStateId, DecorateState> cyborgDemonStates =
{
    std::make_pair(StateIdWalk, cyborgDemonStateWalk),
    std::make_pair(StateIdAttack, cyborgDemonStateAttack),
    std::make_pair(StateIdPain, cyborgDemonStatePain),
    std::make_pair(StateIdDying, cyborgDemonStateDying),
    std::make_pair(StateIdDead, cyborgDemonStateDead)
};

const DecorateActor decorateCyborgDemon =
{
    actorIdMonsterCyborgDemon,   // Id
    15, // spawnOnAllDifficulties;
    15, // spawnOnNormalAndHard;
    15,  // spawnOnHard;
    30,  // initialHealth;
    35 * pixelRadius,    // size;
    WithPurpleGem,  // radarVisibility;
    EgaMagenta,   // radarColor;
    cyborgDemonStates,
    StateIdWalk,   // initialState;
    15,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2048,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// STOMPY (WALKING RED TANK)
//

const DecorateAnimation stompyWalkAnimation =
{
    { STOMPYWALK1PIC, 15, ActionChaseLikeMage },
    { STOMPYWALK2PIC, 15, ActionChaseLikeMage },
    { STOMPYWALK3PIC, 15, ActionChaseLikeMage },
    { STOMPYWALK4PIC, 15, ActionChaseLikeMage }
};

const DecorateState stompyStateWalk =
{
    stompyWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation stompyAttackAnimation =
{
    { STOMPYATTACK1PIC, 10, ActionNone },
    { STOMPYATTACK2PIC, 15, ActionNone },
    { STOMPYATTACK2PIC, 1, ActionAttack },
    { STOMPYATTACK1PIC, 10, ActionNone }
};

const DecorateState stompyStateAttack =
{
    stompyAttackAnimation,
    StateIdWalk
};

const DecorateAnimation stompyPainAnimation =
{
    { STOMPYATTACK1PIC, 10, ActionNone }
};

const DecorateState stompyStatePain =
{
    stompyPainAnimation,
    StateIdWalk
};

const DecorateAnimation stompyDyingAnimation =
{
    { STOMPYDEATH1PIC, 45, ActionExplodeSound },
    { STOMPYDEATH2PIC, 30, ActionNone },
    { STOMPYDEATH3PIC, 25, ActionNone },
    { STOMPYDEATH4PIC, 20, ActionNone },
    { 0, 10, ActionRemove }
};

const DecorateState stompyStateDying =
{
    stompyDyingAnimation,
    StateIdDying
};

const std::map<DecorateStateId, DecorateState> stompyStates =
{
    std::make_pair(StateIdWalk, stompyStateWalk),
    std::make_pair(StateIdAttack, stompyStateAttack),
    std::make_pair(StateIdPain, stompyStatePain),
    std::make_pair(StateIdDying, stompyStateDying)
};

const DecorateActor decorateStompy =
{
    actorIdMonsterStompy,   // Id
    21, // spawnOnAllDifficulties;
    21, // spawnOnNormalAndHard;
    21,  // spawnOnHard
    20,  // initialHealth;
    25 * pixelRadius,    // size;
    WithRedGem,  // radarVisibility;
    EgaRed,   // radarColor;
    stompyStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    1800,    // speed;
    0, // actionParameter
    actorIdProjectileStompyShot // ProjectileId
};

//
// RUNNING EYE
//

const DecorateAnimation runningEyeWalkAnimation =
{
    { EYE_WALK1PIC, 20, ActionChaseLikeRunningEye },
    { EYE_WALK2PIC, 20, ActionChaseLikeRunningEye },
    { EYE_WALK3PIC, 20, ActionChaseLikeRunningEye },
    { EYE_WALK2PIC, 20, ActionChaseLikeRunningEye }
};

const DecorateState runningEyeStateWalk =
{
    runningEyeWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation runningEyePainAnimation =
{
    { EYE_OUCH1PIC, 8, ActionNone },
    { EYE_OUCH2PIC, 8, ActionNone }
};

const DecorateState runningEyeStatePain =
{
    runningEyePainAnimation,
    StateIdWalk
};

const DecorateAnimation runningEyeDyingAnimation =
{
    { EYE_DEATH1PIC, 22, ActionNone },
    { EYE_DEATH2PIC, 22, ActionSmallMonsterSound },
    { EYE_DEATH3PIC, 22, ActionNone }
};

const DecorateState runningEyeStateDying =
{
    runningEyeDyingAnimation,
    StateIdDead
};

const DecorateAnimation runningEyeDeadAnimation =
{
    { EYE_DEATH4PIC, 22, ActionNone }
};

const DecorateState runningEyeStateDead =
{
    runningEyeDeadAnimation,
    StateIdDead  // Loop
};

const std::map<DecorateStateId, DecorateState> runningEyeStates =
{
    std::make_pair(StateIdWalk, runningEyeStateWalk),
    std::make_pair(StateIdPain,runningEyeStatePain),
    std::make_pair(StateIdDying, runningEyeStateDying),
    std::make_pair(StateIdDead, runningEyeStateDead)
};

const DecorateActor decorateRunningEye =
{
    actorIdMonsterRunningEye,   // Id
    27, // spawnOnAllDifficulties;
    27, // spawnOnNormalAndHard;
    27,  // spawnOnHard
    15,  // initialHealth;
    35 * pixelRadius,    // size;
    WithPurpleGem,  // radarVisibility;
    EgaBrightMagenta,   // radarColor;
    runningEyeStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    3500,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// DEMON
//

const DecorateAnimation demonWalkAnimation =
{
    { DEMON1PIC, 20, ActionChase },
    { DEMON2PIC, 20, ActionChase },
    { DEMON3PIC, 20, ActionChase },
    { DEMON4PIC, 20, ActionChase }
};

const DecorateState demonStateWalk =
{
    demonWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation demonAttackAnimation =
{
    { DEMONATTACK1PIC, 20, ActionNone },
    { DEMONATTACK2PIC, 20, ActionNone },
    { DEMONATTACK3PIC, 30, ActionAttack },
    { DEMON1PIC, 40, ActionNone }
};

const DecorateState demonStateAttack =
{
    demonAttackAnimation,
    StateIdWalk
};

const DecorateAnimation demonPainAnimation =
{
    { DEMONOUCHPIC, 15, ActionChase }
};

const DecorateState demonStatePain =
{
    demonPainAnimation,
    StateIdWalk
};

const DecorateAnimation demonDyingAnimation =
{
    { DEMONDIE1PIC, 40, ActionNone },
    { DEMONDIE2PIC, 30, ActionLargeMonsterSound }
};

const DecorateState demonStateDying =
{
    demonDyingAnimation,
    StateIdDead
};

const DecorateAnimation demonDeadAnimation =
{
    { DEMONDIE3PIC, 16, ActionNone }
};

const DecorateState demonStateDead =
{
    demonDeadAnimation,
    StateIdDead //  Loop
};

const std::map<DecorateStateId, DecorateState> demonStates =
{
    std::make_pair(StateIdWalk, demonStateWalk),
    std::make_pair(StateIdAttack, demonStateAttack),
    std::make_pair(StateIdPain, demonStatePain),
    std::make_pair(StateIdDying, demonStateDying),
    std::make_pair(StateIdDead, demonStateDead)
};

const DecorateActor decorateDemon =
{
    actorIdMonsterDemon,   // Id
    30, // spawnOnAllDifficulties;
    30, // spawnOnNormalAndHard;
    30,  // spawnOnHard;
    30,  // initialHealth;
    35 * pixelRadius,    // size;
    WithBlueGem,  // radarVisibility;
    EgaBlue,   // radarColor;
    demonStates,
    StateIdWalk,   // initialState;
    15,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2048,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// AQUAMAN
//

const DecorateAnimation aquaManHiddenAnimation =
{
    { EYESTALKUNDER1PIC, 25, ActionHideUnderWater },
    { EYESTALKUNDER2PIC, 20, ActionHideUnderWater },
    { EYESTALKUNDER3PIC, 20, ActionHideUnderWater }
};

const DecorateState aquaManStateHidden =
{
    aquaManHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation aquaManRiseAnimation =
{
    { EYESTALKRISE1PIC, 20, ActionNone },
    { EYESTALKRISE2PIC, 15, ActionNone }
};

const DecorateState aquaManStateRise =
{
    aquaManRiseAnimation,
    StateIdWalk
};

const DecorateAnimation aquaManSinkAnimation =
{
    { EYESTALKRISE2PIC, 15, ActionNone },
    { EYESTALKRISE1PIC, 20, ActionNone }
};

const DecorateState aquaManStateSink =
{
    aquaManSinkAnimation,
    StateIdHidden
};

const DecorateAnimation aquaManPeekAnimation =
{
    { EYESTALKUNDER4PIC, 40, ActionNone } // look left
};

const DecorateState aquaManStatePeek =
{
    aquaManPeekAnimation,
    StateIdHidden
};

const DecorateAnimation aquaManPeekAltAnimation =
{
    { EYESTALKUNDER5PIC, 40, ActionNone } // look right
};

const DecorateState aquaManStatePeekAlt =
{
    aquaManPeekAltAnimation,
    StateIdHidden
};

const DecorateAnimation aquaManWalkAnimation =
{
    { EYESTALKWALK1PIC, 12, ActionChaseLikeWetMan },
    { EYESTALKWALK2PIC, 12, ActionChaseLikeWetMan }
};

const DecorateState aquaManStateWalk =
{
    aquaManWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation aquaManAttackAnimation =
{
    { EYESTALKATTACKPIC, 10, ActionNone },
    { EYESTALKWALK1PIC, 10, ActionAttack }
};

const DecorateState aquaManStateAttack =
{
    aquaManAttackAnimation,
    StateIdWalk
};

const DecorateAnimation aquaManDyingAnimation =
{
    { EYESTALKDEATH1PIC, 8, ActionNone },
    { EYESTALKDEATH2PIC, 8, ActionNone },
    { EYESTALKDEATH1PIC, 8, ActionNone },
    { EYESTALKDEATH2PIC, 8, ActionNone },
    { EYESTALKDEATH1PIC, 8, ActionNone },
    { EYESTALKDEATH2PIC, 8, ActionNone },
    { EYESTALKDEATH2PIC, 30, ActionNone },
    { EYESTALKDEATH3PIC, 40, ActionNone },
    { EYESTALKDEATH4PIC, 30, ActionExplodeSound },
    { EYESTALKDEATH5PIC, 20, ActionNone },
    { 0, 1, ActionRemove }
};

const DecorateState aquaManStateDying =
{
    aquaManDyingAnimation,
    StateIdDying  // Will not loop, due to the ActionRemove.
};

const std::map<DecorateStateId, DecorateState> aquaManStates =
{
    std::make_pair(StateIdHidden, aquaManStateHidden),
    std::make_pair(StateIdRise, aquaManStateRise),
    std::make_pair(StateIdSink, aquaManStateSink),
    std::make_pair(StateIdPeek, aquaManStatePeek),
    std::make_pair(StateIdPeekAlternative, aquaManStatePeekAlt),
    std::make_pair(StateIdWalk, aquaManStateWalk),
    std::make_pair(StateIdAttack, aquaManStateAttack),
    std::make_pair(StateIdDying, aquaManStateDying)
};

const DecorateActor decorateAquaMan =
{
    actorIdMonsterAquaMan,   // Id
    12, // spawnOnAllDifficulties;
    12, // spawnOnNormalAndHard;
    12,  // spawnOnHard
    15,  // initialHealth;
    32 * pixelRadius,    // size;
    WithGreenGem,  // radarVisibility;
    EgaGreen,   // radarColor;
    aquaManStates,
    StateIdHidden,   // initialState;
    7,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    1000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// TROLL
//

const DecorateAnimation trollWalkAnimation =
{
    { TROLL1PIC, 13, ActionChase },
    { TROLL2PIC, 13, ActionChase },
    { TROLL3PIC, 13, ActionChase },
    { TROLL4PIC, 13, ActionChase }
};

const DecorateState trollStateWalk =
{
   trollWalkAnimation,
   StateIdWalk // Loop
};

const DecorateAnimation trollAttackAnimation =
{
    { TROLLATTACK1PIC, 15, ActionNone },
    { TROLLATTACK2PIC, 20, ActionNone },
    { TROLL1PIC, 30, ActionAttack }
};

const DecorateState trollStateAttack =
{
    trollAttackAnimation,
    StateIdWalk
};

const DecorateAnimation trollPainAnimation =
{
    { TROLLOUCHPIC, 14, ActionChase }
};

const DecorateState trollStatePain =
{
    trollPainAnimation,
    StateIdWalk
};

const DecorateAnimation trollDyingAnimation =
{
    { TROLLDIE1PIC, 18, ActionNone },
    { TROLLDIE2PIC, 15, ActionLargeMonsterSound }
};

const DecorateState trollStateDying =
{
    trollDyingAnimation,
    StateIdDead
};

const DecorateAnimation trollDeadAnimation =
{
    { TROLLDIE3PIC, 16, ActionNone }
};

const DecorateState trollStateDead =
{
    trollDeadAnimation,
    StateIdDead //  Loop
};

const std::map<DecorateStateId, DecorateState> trollStates =
{
    std::make_pair(StateIdWalk, trollStateWalk),
    std::make_pair(StateIdAttack, trollStateAttack),
    std::make_pair(StateIdPain, trollStatePain),
    std::make_pair(StateIdDying, trollStateDying),
    std::make_pair(StateIdDead, trollStateDead)
};

const DecorateActor decorateTroll =
{
    actorIdMonsterTroll,   // Id
    22, // spawnOnAllDifficulties;
    22, // spawnOnNormalAndHard;
    22,  // spawnOnHard;
    15,  // initialHealth;
    35 * pixelRadius,    // size;
    WithYellowGem,  // radarVisibility;
    EgaBrightYellow,   // radarColor;
    trollStates,
    StateIdWalk,   // initialState;
    10,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2500,    // speed;
    0, // actionParameter
    0 // ProjectileId
};