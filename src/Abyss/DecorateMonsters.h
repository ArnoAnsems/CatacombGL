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
// DecorateMonsters
//
// Actor definitions for the Catacomb Abyss monsters.
//
#pragma once

#include "../Engine/Decorate.h"
#include "DecorateProjectiles.h"
#include "EgaGraphAbyss.h"
#include "AudioRepositoryAbyss.h"

const uint16_t actorIdMonsterZombie = 50;
const uint16_t actorIdMonsterSkeleton = 51;
const uint16_t actorIdMonsterWallSkeleton = 52;
const uint16_t actorIdMonsterOrc = 53;
const uint16_t actorIdMonsterTroll = 54;
const uint16_t actorIdMonsterDemon = 55;
const uint16_t actorIdMonsterRedDemon = 56;
const uint16_t actorIdMonsterSpook = 57;
const uint16_t actorIdMonsterBat = 58;
const uint16_t actorIdMonsterMage = 59;
const uint16_t actorIdMonsterEye = 60;
const uint16_t actorIdMonsterWetMan = 61;
const uint16_t actorIdMonsterNemesis = 62;

//
// ZOMBIE
//

const DecorateAnimation zombieHiddenAnimation =
{
    { 0 , 13, ActionHide }
};

const DecorateState zombieStateHidden = 
{
    zombieHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation zombieRiseAnimation =
{
    { ZOMB_APPEAR1PIC, 24, ActionNone },
    { ZOMB_APPEAR2PIC, 24, ActionNone },
    { ZOMB_APPEAR3PIC, 24, ActionNone },
    { ZOMB_APPEAR4PIC, 24, ActionNone },
    { ZOMB_WALK3PIC, 1, ActionNone }
};

const DecorateState zombieStateRise =
{
    zombieRiseAnimation,
    StateIdWalk
};

const DecorateAnimation zombieWalkAnimation =
{
    { ZOMB_WALK1PIC, 13, ActionChase },
    { ZOMB_WALK2PIC, 13, ActionChase },
    { ZOMB_WALK3PIC, 13, ActionChase }
};

const DecorateState zombieStateWalk =
{
    zombieWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation zombieAttackAnimation =
{
    { ZOMB_ATTACKPIC, 15, ActionAttack },
    { ZOMB_WALK1PIC, 20, ActionNone }
};

const DecorateState zombieStateAttack =
{
    zombieAttackAnimation,
    StateIdWalk
};

const DecorateAnimation zombiePainAnimation =
{
    { ZOMB_OUCHPIC, 15, ActionNone }
};

const DecorateState zombieStatePain =
{
    zombiePainAnimation,
    StateIdWalk
};

const DecorateAnimation zombieDyingAnimation =
{
    { ZOMB_DIE1PIC, 16, ActionNone },
    { ZOMB_DIE2PIC, 16, ActionNone },
    { ZOMB_DIE3PIC, 16, ActionNone }
};

const DecorateState zombieStateDying =
{
    zombieDyingAnimation,
    StateIdDead
};

const DecorateAnimation zombieDeadAnimation =
{
    { ZOMB_DIE3PIC, 16, ActionNone }
};

const DecorateState zombieStateDead =
{
    zombieDeadAnimation,
    StateIdDead //  Loop
};

const std::map<DecorateStateId, DecorateState> zombieStates =
{
    std::make_pair(StateIdHidden, zombieStateHidden),
    std::make_pair(StateIdRise, zombieStateRise),
    std::make_pair(StateIdWalk, zombieStateWalk),
    std::make_pair(StateIdAttack, zombieStateAttack),
    std::make_pair(StateIdPain, zombieStatePain),
    std::make_pair(StateIdDying, zombieStateDying),
    std::make_pair(StateIdDead, zombieStateDead)
};

const DecorateActor decorateZombie =
{
    actorIdMonsterZombie,   // Id
    53, // spawnOnAllDifficulties;
    52, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    5,  // initialHealth;
    35 * pixelRadius,    // size;
    WithPurpleGem,  // radarVisibility;
    EgaBrightMagenta,   // radarColor;
    zombieStates,
    StateIdHidden,   // initialState;
    8,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2500,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// SKELETON
//

const DecorateAnimation skeletonWalkAnimation =
{
    { SKELETON_1PIC, 10, ActionChase },
    { SKELETON_2PIC, 10, ActionChase },
    { SKELETON_3PIC, 10, ActionChase },
    { SKELETON_4PIC, 10, ActionChase }
};

const DecorateState skeletonStateWalk =
{
    skeletonWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation skeletonAttackAnimation =
{
    { SKELETON_ATTACK_1PIC, 12, ActionNone },
    { SKELETON_ATTACK_2PIC, 12, ActionNone },
    { SKELETON_ATTACK_3PIC, 12, ActionAttack },
    { SKELETON_1PIC, 40, ActionNone }
};

const DecorateState skeletonStateAttack =
{
    skeletonAttackAnimation,
    StateIdWalk
};

const DecorateAnimation skeletonPainAnimation =
{
    { SKELETON_OUCHPIC, 18, ActionNone }
};

const DecorateState skeletonStatePain =
{
    skeletonPainAnimation,
    StateIdWalk
};

const DecorateAnimation skeletonDyingAnimation =
{
    { SKELETON_OUCHPIC, 18, ActionNone },
    { SKELETON_DEATH_1PIC, 18, ActionNone },
    { SKELETON_DEATH_2PIC, 18, ActionNone }
};

const DecorateState skeletonStateDying =
{
    skeletonDyingAnimation,
    StateIdDead
};

const DecorateAnimation skeletonDeadAnimation =
{
    { SKELETON_DEATH_2PIC, 16, ActionNone }
};

const DecorateState skeletonStateDead =
{
    skeletonDeadAnimation,
    StateIdDead //  Loop
};

const std::map<DecorateStateId, DecorateState> skeletonStates =
{
    std::make_pair(StateIdWalk, skeletonStateWalk),
    std::make_pair(StateIdAttack, skeletonStateAttack),
    std::make_pair(StateIdPain, skeletonStatePain),
    std::make_pair(StateIdDying, skeletonStateDying),
    std::make_pair(StateIdDead, skeletonStateDead)
};

const DecorateActor decorateSkeleton =
{
    actorIdMonsterSkeleton,   // Id
    55, // spawnOnAllDifficulties;
    56, // spawnOnNormalAndHard;
    0,  // spawnOnHard;
    12,  // initialHealth;
    35 * pixelRadius,    // size;
    WithYellowGem,  // radarVisibility;
    EgaBrightYellow,   // radarColor;
    skeletonStates,
    StateIdWalk,   // initialState;
    6,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2036,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// WALL SKELETON
//

const DecorateAnimation wallSkeletonHiddenAnimation =
{
    { 0 , 13, ActionHide }
};

const DecorateState wallSkeletonStateHidden = 
{
    wallSkeletonHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation wallSkeletonRiseAnimation =
{
    { 0, 2400, ActionSpawnSkeleton },
    { 0, 12, ActionRemove }
};

const DecorateState wallSkeletonStateRise =
{
    wallSkeletonRiseAnimation,
    StateIdRise
};

const std::map<DecorateStateId, DecorateState> wallSkeletonStates =
{
    std::make_pair(StateIdHidden, wallSkeletonStateHidden),
    std::make_pair(StateIdRise, wallSkeletonStateRise),
};

const DecorateActor decorateWallSkeleton =
{
    actorIdMonsterWallSkeleton,   // Id
    69, // spawnOnAllDifficulties;
    70, // spawnOnNormalAndHard;
    71,  // spawnOnHard;
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightYellow,   // radarColor;
    wallSkeletonStates,
    StateIdHidden,   // initialState;
    6,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    0,    // speed;
    actorIdMonsterSkeleton, // actionParameter
    0 // ProjectileId
};

//
// ORC
//

const DecorateAnimation orcWalkAnimation =
{
    { ORC1PIC, 20, ActionChase },
    { ORC2PIC, 20, ActionChase },
    { ORC3PIC, 20, ActionChase },
    { ORC4PIC, 20, ActionChase }
};

const DecorateState orcStateWalk =
{
   orcWalkAnimation,
   StateIdWalk // Loop
};

const DecorateAnimation orcAttackAnimation =
{
    { ORCATTACK1PIC, 20, ActionNone },
    { ORCATTACK2PIC, 20, ActionNone },
    { ORCATTACK2PIC, 30, ActionAttack },
    { ORC1PIC, 40, ActionNone }
};

const DecorateState orcStateAttack =
{
    orcAttackAnimation,
    StateIdWalk
};

const DecorateAnimation orcPainAnimation =
{
    { ORCOUCHPIC, 15, ActionNone }
};

const DecorateState orcStatePain =
{
    orcPainAnimation,
    StateIdWalk
};

const DecorateAnimation orcDyingAnimation =
{
    { ORCDIE1PIC, 18, ActionNone },
    { ORCDIE1PIC, 18, ActionNone },
    { ORCDIE3PIC, 18, ActionNone }
};

const DecorateState orcStateDying =
{
    orcDyingAnimation,
    StateIdDead
};

const DecorateAnimation orcDeadAnimation =
{
    { ORCDIE3PIC, 16, ActionNone }
};

const DecorateState orcStateDead =
{
    orcDeadAnimation,
    StateIdDead //  Loop
};

const std::map<DecorateStateId, DecorateState> orcStates =
{
    std::make_pair(StateIdWalk, orcStateWalk),
    std::make_pair(StateIdAttack, orcStateAttack),
    std::make_pair(StateIdPain, orcStatePain),
    std::make_pair(StateIdDying, orcStateDying),
    std::make_pair(StateIdDead, orcStateDead)
};

const DecorateActor decorateOrc =
{
    actorIdMonsterOrc,   // Id
    23, // spawnOnAllDifficulties;
    37, // spawnOnNormalAndHard;
    42,  // spawnOnHard;
    6,  // initialHealth;
    32 * pixelRadius,    // size;
    WithGreenGem,  // radarVisibility;
    EgaGreen,   // radarColor;
    orcStates,
    StateIdWalk,   // initialState;
    4,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    1536,    // speed;
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
    { TROLLATTACK2PIC, 15, ActionNone },
    { TROLLATTACK3PIC, 30, ActionAttack },
    { TROLL1PIC, 40, ActionNone }
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
    { TROLLDIE2PIC, 18, ActionNone },
    { TROLLDIE3PIC, 18, ActionNone }
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
    36, // spawnOnNormalAndHard;
    41,  // spawnOnHard;
    14,  // initialHealth;
    35 * pixelRadius,    // size;
    WithGreenGem,  // radarVisibility;
    EgaBrightGreen,   // radarColor;
    trollStates,
    StateIdWalk,   // initialState;
    8,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2500,    // speed;
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
    { DEMONOUCHPIC, 14, ActionChase }
};

const DecorateState demonStatePain =
{
    demonPainAnimation,
    StateIdWalk
};

const DecorateAnimation demonDyingAnimation =
{
    { DEMONDIE1PIC, 40, ActionNone },
    { DEMONDIE2PIC, 30, ActionNone },
    { DEMONDIE3PIC, 30, ActionNone }
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
    26, // spawnOnAllDifficulties;
    39, // spawnOnNormalAndHard;
    44,  // spawnOnHard;
    50,  // initialHealth;
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
// RED DEMON
//

const DecorateAnimation redDemonWalkAnimation =
{
    { RED_DEMON1PIC, 20, ActionChase },
    { RED_DEMON2PIC, 20, ActionChase },
    { RED_DEMON3PIC, 20, ActionChase },
    { RED_DEMON4PIC, 20, ActionChase }
};

const DecorateState redDemonStateWalk =
{
    redDemonWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation redDemonAttackAnimation =
{
    { RED_DEMONATTACK1PIC, 20, ActionNone },
    { RED_DEMONATTACK2PIC, 20, ActionNone },
    { RED_DEMONATTACK3PIC, 30, ActionAttack },
    { RED_DEMON1PIC, 40, ActionNone }
};

const DecorateState redDemonStateAttack =
{
    redDemonAttackAnimation,
    StateIdWalk
};

const DecorateAnimation redDemonPainAnimation =
{
    { RED_DEMONOUCHPIC, 15, ActionChase }
};

const DecorateState redDemonStatePain =
{
    redDemonPainAnimation,
    StateIdWalk
};

const DecorateAnimation redDemonDyingAnimation =
{
    { RED_DEMONDIE1PIC, 40, ActionNone },
    { RED_DEMONDIE2PIC, 30, ActionNone },
    { RED_DEMONDIE3PIC, 30, ActionNone }
};

const DecorateState redDemonStateDying =
{
    redDemonDyingAnimation,
    StateIdDead
};

const DecorateAnimation redDemonDeadAnimation =
{
    { RED_DEMONDIE3PIC, 16, ActionNone }
};

const DecorateState redDemonStateDead =
{
    redDemonDeadAnimation,
    StateIdDead //  Loop
};

const std::map<DecorateStateId, DecorateState> redDemonStates =
{
    std::make_pair(StateIdWalk, redDemonStateWalk),
    std::make_pair(StateIdAttack, redDemonStateAttack),
    std::make_pair(StateIdPain, redDemonStatePain),
    std::make_pair(StateIdDying, redDemonStateDying),
    std::make_pair(StateIdDead, redDemonStateDead)
};

const DecorateActor decorateRedDemon =
{
    actorIdMonsterRedDemon,   // Id
    20, // spawnOnAllDifficulties;
    24, // spawnOnNormalAndHard;
    30,  // spawnOnHard; Actually, should include 20 and 24 as well.
    50,  // initialHealth;
    35 * pixelRadius,    // size;
    WithRedGem,  // radarVisibility;
    EgaRed,   // radarColor;
    redDemonStates,
    StateIdWalk,   // initialState;
    15,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2048,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// SPOOK
//

const DecorateAnimation spookHiddenAnimation =
{
    { 0 , 13, ActionHide }
};

const DecorateState spookStateHidden = 
{
    spookHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation spookRiseAnimation =
{
    { SPOOK_INOUTPIC, 7, ActionNone },
    { 0, 7, ActionNone },
    { SPOOK_INOUTPIC, 7, ActionNone }
};

const DecorateState spookStateRise =
{
    spookRiseAnimation,
    StateIdWalk
};

const DecorateAnimation spookWalkAnimation =
{
    { SPOOK1PIC, 10, ActionNone },
    { SPOOK2PIC, 10, ActionChase },
    { SPOOK3PIC, 10, ActionChase },
    { SPOOK4PIC, 10, ActionChase },
    { SPOOK3PIC, 10, ActionChase },
    { SPOOK2PIC, 10, ActionChase }
};

const DecorateState spookStateWalk =
{
    spookWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation spookAttackAnimation =
{
    { SPOOK_ATTACKPIC, 35, ActionNone },
    { SPOOK3PIC, 20, ActionAttack }
};

const DecorateState spookStateAttack =
{
    spookAttackAnimation,
    StateIdWalk
};

const DecorateAnimation spookPainAnimation =
{
    { SPOOKHITPIC, 5, ActionNone }
};

const DecorateState spookStatePain =
{
    spookPainAnimation,
    StateIdWalk
};

const DecorateAnimation spookDyingAnimation =
{
    { SPOOK_INOUTPIC, 9, ActionNone },
    { SPOOK4PIC, 9, ActionNone },
    { SPOOK_INOUTPIC, 9, ActionNone },
    { SPOOK4PIC, 9, ActionNone },
    { SPOOK_INOUTPIC, 9, ActionNone },
    { 0, 11, ActionRemove },
};

const DecorateState spookStateDying =
{
    spookDyingAnimation,
    StateIdDying  // Will not loop, due to the ActionRemove.
};

const std::map<DecorateStateId, DecorateState> spookStates =
{
    std::make_pair(StateIdHidden, spookStateHidden),
    std::make_pair(StateIdRise, spookStateRise),
    std::make_pair(StateIdWalk, spookStateWalk),
    std::make_pair(StateIdAttack, spookStateAttack),
    std::make_pair(StateIdPain,spookStatePain),
    std::make_pair(StateIdDying, spookStateDying)
};

const DecorateActor decorateSpook =
{
    actorIdMonsterSpook,   // Id
    49, // spawnOnAllDifficulties;
    50, // spawnOnNormalAndHard;
    51,  // spawnOnHard; Actually, should include 20 and 24 as well.
    5,  // initialHealth;
    35 * pixelRadius,    // size;
    WithYellowGem,  // radarVisibility;
    EgaBrown,   // radarColor;
    spookStates,
    StateIdHidden,   // initialState;
    2,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    1900,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// BAT
//

const DecorateAnimation batWalkAnimation =
{
    { BAT1PIC, 6, ActionNone },
    { BAT2PIC, 6, ActionChase },
    { BAT3PIC, 6, ActionChase },
    { BAT4PIC, 6, ActionChase }
};

const DecorateState batStateWalk =
{
    batWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation batAttackAnimation =
{
    { BAT4PIC, 5, ActionAttack },
    { BAT4PIC, 75, ActionRunAway }
};

const DecorateState batStateAttack =
{
    batAttackAnimation,
    StateIdWalk
};

const DecorateAnimation batDyingAnimation =
{
    { BATDIE1PIC, 18, ActionNone },
    { BATDIE2PIC, 18, ActionNone },
    { 0, 18, ActionRemove }
};

const DecorateState batStateDying =
{
    batDyingAnimation,
    StateIdDying  // Will not loop, due to the ActionRemove.
};

const std::map<DecorateStateId, DecorateState> batStates =
{
    std::make_pair(StateIdWalk, batStateWalk),
    std::make_pair(StateIdAttack, batStateAttack),
    std::make_pair(StateIdDying, batStateDying)
};

const DecorateActor decorateBat =
{
    actorIdMonsterBat,   // Id
    25, // spawnOnAllDifficulties;
    38, // spawnOnNormalAndHard;
    43,  // spawnOnHard; Actually, should include 20 and 24 as well.
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Always,  // radarVisibility;
    EgaDarkGray,   // radarColor;
    batStates,
    StateIdWalk,   // initialState;
    4,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2000,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// MAGE
//

const DecorateAnimation mageWalkAnimation =
{
    { MAGE1PIC, 20, ActionChaseLikeMage },
    { MAGE2PIC, 20, ActionChaseLikeMage }
};

const DecorateState mageStateWalk =
{
    mageWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation mageAttackAnimation =
{
    { MAGEATTACKPIC, 30, ActionAttack },
    { MAGE1PIC, 10, ActionChaseLikeMage }
};

const DecorateState mageStateAttack =
{
    mageAttackAnimation,
    StateIdWalk
};

const DecorateAnimation magePainAnimation =
{
    { MAGEOUCHPIC, 10, ActionNone }
};

const DecorateState mageStatePain =
{
    magePainAnimation,
    StateIdWalk
};

const DecorateAnimation mageDyingAnimation =
{
    { MAGEDIE1PIC, 20, ActionNone }
};

const DecorateState mageStateDying =
{
    mageDyingAnimation,
    StateIdDead
};

const DecorateAnimation mageDeadAnimation =
{
    { MAGEDIE2PIC, 20, ActionNone }
};

const DecorateState mageStateDead =
{
    mageDeadAnimation,
    StateIdDead // Loop
};

const std::map<DecorateStateId, DecorateState> mageStates =
{
    std::make_pair(StateIdWalk, mageStateWalk),
    std::make_pair(StateIdAttack, mageStateAttack),
    std::make_pair(StateIdPain, mageStatePain),
    std::make_pair(StateIdDying, mageStateDying),
    std::make_pair(StateIdDead, mageStateDead)
};

const DecorateActor decorateMage =
{
    actorIdMonsterMage,   // Id
    27, // spawnOnAllDifficulties;
    40, // spawnOnNormalAndHard;
    45,  // spawnOnHard
    5,  // initialHealth;
    35 * pixelRadius,    // size;
    WithAllGems,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    mageStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    3072,    // speed;
    0, // actionParameter
    actorIdProjectileMageShot // ProjectileId
};

//
// EYE
//

const DecorateAnimation eyeWalkAnimation =
{
    { EYE_WALK1PIC, 20, ActionChaseLikeMage },
    { EYE_WALK2PIC, 20, ActionChaseLikeMage },
    { EYE_WALK3PIC, 20, ActionChaseLikeMage },
    { EYE_WALK2PIC, 20, ActionChaseLikeMage }
};

const DecorateState eyeStateWalk =
{
    eyeWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation eyeAttackAnimation =
{
    { EYE_WALK1PIC, 40, ActionAttack }
};

const DecorateState eyeStateAttack =
{
    eyeAttackAnimation,
    StateIdWalk
};

const DecorateAnimation eyePainAnimation =
{
    { EYE_OUCH1PIC, 8, ActionNone },
    { EYE_OUCH2PIC, 8, ActionNone }
};

const DecorateState eyeStatePain =
{
    eyePainAnimation,
    StateIdWalk
};

const DecorateAnimation eyeDyingAnimation =
{
    { EYE_DEATH1PIC, 22, ActionNone },
    { EYE_DEATH2PIC, 22, ActionNone },
    { 0, 22, ActionRemove }
};

const DecorateState eyeStateDying =
{
    eyeDyingAnimation,
    StateIdDying  // Will not loop, due to the ActionRemove.
};

const std::map<DecorateStateId, DecorateState> eyeStates =
{
    std::make_pair(StateIdWalk, eyeStateWalk),
    std::make_pair(StateIdAttack, eyeStateAttack),
    std::make_pair(StateIdPain,eyeStatePain),
    std::make_pair(StateIdDying, eyeStateDying)
};

const DecorateActor decorateEye =
{
    actorIdMonsterEye,   // Id
    66, // spawnOnAllDifficulties;
    67, // spawnOnNormalAndHard;
    68,  // spawnOnHard
    14,  // initialHealth;
    35 * pixelRadius,    // size;
    WithRedGem,  // radarVisibility;
    EgaRed,   // radarColor;
    eyeStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    1200,    // speed;
    0, // actionParameter
    actorIdProjectileEyeShot // ProjectileId
};

//
// WETMAN
//

const DecorateAnimation wetManHiddenAnimation =
{
    { WET_BUBBLE1PIC, 13, ActionHideUnderWater },
    { WET_BUBBLE2PIC, 15, ActionHideUnderWater },
    { 0, 18, ActionHideUnderWater }
};

const DecorateState wetManStateHidden = 
{
    wetManHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation wetManRiseAnimation =
{
    { WET_BUBBLE2PIC, 15, ActionNone },
    { WET_EYESPIC, 20, ActionNone },
    { WET_RISE1PIC, 20, ActionNone },
    { WET_RISE2PIC, 20, ActionNone }
};

const DecorateState wetManStateRise =
{
    wetManRiseAnimation,
    StateIdWalk
};

const DecorateAnimation wetManSinkAnimation =
{
    { WET_RISE2PIC, 20, ActionNone },
    { WET_RISE1PIC, 20, ActionNone },
    { WET_EYESPIC, 20, ActionNone }
};

const DecorateState wetManStateSink =
{
    wetManSinkAnimation,
    StateIdHidden
};

const DecorateAnimation wetManPeekAnimation =
{
    { WET_EYESPIC, 45, ActionNone }
};

const DecorateState wetManStatePeek =
{
    wetManPeekAnimation,
    StateIdHidden
};

const DecorateAnimation wetManWalkAnimation =
{
    { WET_WALK1PIC, 12, ActionChaseLikeWetMan },
    { WET_WALK2PIC, 12, ActionChaseLikeWetMan },
    { WET_WALK3PIC, 12, ActionChaseLikeWetMan },
    { WET_WALK4PIC, 12, ActionChaseLikeWetMan }
};

const DecorateState wetManStateWalk =
{
    wetManWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation wetManAttackAnimation =
{
    { WET_ATTACK1PIC, 10, ActionNone },
    { WET_ATTACK2PIC, 20, ActionNone },
    { WET_ATTACK3PIC, 10, ActionAttack }
};

const DecorateState wetManStateAttack =
{
    wetManAttackAnimation,
    StateIdWalk
};

const DecorateAnimation wetManPainAnimation =
{
    { WET_OUCHPIC, 10, ActionNone }
};

const DecorateState wetManStatePain =
{
    wetManPainAnimation,
    StateIdWalk
};

const DecorateAnimation wetManDyingAnimation =
{
    { WET_DIE1PIC, 27, ActionNone },
    { WET_DIE2PIC, 29, ActionNone },
    { WET_DIE3PIC, 44, ActionNone },
    { WET_BUBBLE2PIC, 26, ActionNone },
    { WET_BUBBLE1PIC, 23, ActionNone },
    { 0, 22, ActionRemove }
};

const DecorateState wetManStateDying =
{
    wetManDyingAnimation,
    StateIdDying  // Will not loop, due to the ActionRemove.
};

const std::map<DecorateStateId, DecorateState> wetManStates =
{
    std::make_pair(StateIdHidden, wetManStateHidden),
    std::make_pair(StateIdRise, wetManStateRise),
    std::make_pair(StateIdSink, wetManStateSink),
    std::make_pair(StateIdPeek, wetManStatePeek),
    std::make_pair(StateIdWalk, wetManStateWalk),
    std::make_pair(StateIdAttack, wetManStateAttack),
    std::make_pair(StateIdPain, wetManStatePain),
    std::make_pair(StateIdDying, wetManStateDying)
};

const DecorateActor decorateWetMan =
{
    actorIdMonsterWetMan,   // Id
    63, // spawnOnAllDifficulties;
    64, // spawnOnNormalAndHard;
    65,  // spawnOnHard
    18,  // initialHealth;
    35 * pixelRadius,    // size;
    WithBlueGem,  // radarVisibility;
    EgaBrightBlue,   // radarColor;
    wetManStates,
    StateIdHidden,   // initialState;
    7,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2200,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// NEMESIS
//

const DecorateAnimation nemesisWalkAnimation =
{
    { GREL1PIC, 20, ActionChase },
    { GREL2PIC, 20, ActionChase }
};

const DecorateState nemesisStateWalk =
{
    nemesisWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation nemesisAttackAnimation =
{
    { GRELATTACKPIC, 30, ActionAttack },
    { GREL1PIC, 50, ActionNone }
};

const DecorateState nemesisStateAttack =
{
    nemesisAttackAnimation,
    StateIdWalk
};

const DecorateAnimation nemesisPainAnimation =
{
    { GRELHITPIC, 6, ActionNone }
};

const DecorateState nemesisStatePain =
{
    nemesisPainAnimation,
    StateIdWalk
};

const DecorateAnimation nemesisDyingAnimation =
{
    { GRELDIE1PIC, 22, ActionNone },
    { GRELDIE2PIC, 22, ActionNone },
    { GRELDIE3PIC, 22, ActionNone },
    { GRELDIE4PIC, 22, ActionNone },
    { GRELDIE5PIC, 22, ActionNone },
    { GRELDIE5PIC, 1, ActionDropItem }
};

const DecorateState nemesisStateDying =
{
    nemesisDyingAnimation,
    StateIdDead
};

const DecorateAnimation nemesisDeadAnimation =
{
    { GRELDIE6PIC, 20, ActionNone }
};

const DecorateState nemesisStateDead =
{
    nemesisDeadAnimation,
    StateIdDead // Loop
};

const std::map<DecorateStateId, DecorateState> nemesisStates =
{
    std::make_pair(StateIdWalk, nemesisStateWalk),
    std::make_pair(StateIdAttack, nemesisStateAttack),
    std::make_pair(StateIdPain, nemesisStatePain),
    std::make_pair(StateIdDying, nemesisStateDying),
    std::make_pair(StateIdDead, nemesisStateDead)
};

const DecorateActor decorateNemesis =
{
    actorIdMonsterNemesis,   // Id
    28, // spawnOnAllDifficulties;
    28, // spawnOnNormalAndHard;
    28,  // spawnOnHard
    100,  // initialHealth;
    35 * pixelRadius,    // size;
    WithAllGems,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    nemesisStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2048,    // speed;
    0, // actionParameter
    actorIdProjectileNemesisShot // ProjectileId
};
