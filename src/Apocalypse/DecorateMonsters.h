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
const uint16_t actorIdMonsterWizard = 59;
const uint16_t actorIdMonsterBug = 60;

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