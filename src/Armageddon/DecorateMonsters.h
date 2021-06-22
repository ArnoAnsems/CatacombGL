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
// Actor definitions for the Catacomb Armageddon monsters.
//
#pragma once

#include "../Engine/Decorate.h"
#include "DecorateProjectiles.h"
#include "EgaGraphArmageddon.h"
#include "AudioRepositoryArmageddon.h"

const uint16_t actorIdMonsterMage = 59;
const uint16_t actorIdMonsterZombie = 50;
const uint16_t actorIdMonsterSkeleton = 51;
const uint16_t actorIdMonsterWallSkeleton = 52;
const uint16_t actorIdMonsterTree = 53;
const uint16_t actorIdMonsterBunny = 54;
const uint16_t actorIdMonsterWretchedPox = 55;
const uint16_t actorIdMonsterRedDemon = 56;
const uint16_t actorIdMonsterViper = 57;
const uint16_t actorIdMonsterBat = 58;
const uint16_t actorIdMonsterEye = 60;
const uint16_t actorIdMonsterDragon = 61;
const uint16_t actorIdMonsterNemesis = 62;
const uint16_t actorIdMonsterAnt = 63;
const uint16_t actorIdMonsterSuccubus = 64;
const uint16_t actorIdMonsterSkeletonHanging = 65;

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
    36, // spawnOnAllDifficulties;
    36, // spawnOnNormalAndHard;
    36, // spawnOnHard;
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
// ANT
//

const DecorateAnimation antHiddenAnimation =
{
    { ANT_EGG1PIC , 10, ActionHide }
};

const DecorateState antStateHidden =
{
    antHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation antRiseAnimation =
{
    { ANT_EGG2PIC, 45, ActionNone }
};

const DecorateState antStateRise =
{
    antRiseAnimation,
    StateIdWalk
};

const DecorateAnimation antWalkAnimation =
{
    { ANT_WALK1PIC, 20, ActionChase },
    { ANT_WALK2PIC, 20, ActionChase },
    { ANT_WALK3PIC, 20, ActionChase }
};

const DecorateState antStateWalk =
{
    antWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation antAttackAnimation =
{
    { ANT_ATTACKPIC, 20, ActionNone },
    { ANT_WALK2PIC, 15, ActionAttack }
};

const DecorateState antStateAttack =
{
    antAttackAnimation,
    StateIdWalk
};

const DecorateAnimation antPainAnimation =
{
    { ANT_WALK1PIC, 15, ActionNone }
};

const DecorateState antStatePain =
{
    antPainAnimation,
    StateIdWalk
};

const DecorateAnimation antDyingAnimation =
{
    { ANT_DEATH1PIC, 40, ActionNone },
    { ANT_DEATH2PIC, 10, ActionNone },
    { ANT_DEATH3PIC, 10, ActionNone }
};

const DecorateState antStateDying =
{
    antDyingAnimation,
    StateIdDead
};

const DecorateAnimation antDeadAnimation =
{
    { ANT_DEATH2PIC, 10, ActionNone },
    { ANT_DEATH3PIC, 10, ActionNone }
};

const DecorateState antStateDead =
{
    antDeadAnimation,
    StateIdDead //  Loop
};

const std::map<DecorateStateId, DecorateState> antStates =
{
    std::make_pair(StateIdHidden, antStateHidden),
    std::make_pair(StateIdRise, antStateRise),
    std::make_pair(StateIdWalk, antStateWalk),
    std::make_pair(StateIdAttack, antStateAttack),
    std::make_pair(StateIdPain, antStatePain),
    std::make_pair(StateIdDying, antStateDying),
    std::make_pair(StateIdDead, antStateDead)
};

const DecorateActor decorateAnt =
{
    actorIdMonsterAnt,   // Id
    30, // spawnOnAllDifficulties;
    30, // spawnOnNormalAndHard;
    30, // spawnOnHard;
    15,  // initialHealth;
    35 * pixelRadius,    // size;
    WithYellowGem,  // radarVisibility;
    EgaBrown,   // radarColor;
    antStates,
    StateIdHidden,   // initialState;
    2,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    1900,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// TREE
//

const DecorateAnimation treeHiddenAnimation =
{
    { TREE_IDLEPIC , 13, ActionStatue }
};

const DecorateState treeStateHidden =
{
    treeHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation treeRiseAnimation =
{
    { TREE_AWAKENINGPIC, 50, ActionNone }
};

const DecorateState treeStateRise =
{
    treeRiseAnimation,
    StateIdWalk
};

const DecorateAnimation treeWalkAnimation =
{
    { TREE_WALK1PIC, 13, ActionChase },
    { TREE_WALK2PIC, 13, ActionChase }
};

const DecorateState treeStateWalk =
{
    treeWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation treeAttackAnimation =
{
    { TREE_ATTACK1PIC, 15, ActionAttack },
    { TREE_ATTACK2PIC, 15, ActionAttack },
    { TREE_ATTACK3PIC, 15, ActionNone }
};

const DecorateState treeStateAttack =
{
    treeAttackAnimation,
    StateIdWalk
};

const DecorateAnimation treePainAnimation =
{
    { TREE_AWAKENINGPIC, 15, ActionNone }
};

const DecorateState treeStatePain =
{
    treePainAnimation,
    StateIdWalk
};

const DecorateAnimation treeDyingAnimation =
{
    { TREE_DEATH1PIC, 45, ActionNone },
    { TREE_DEATH2PIC, 25, ActionNone }
};

const DecorateState treeStateDying =
{
    treeDyingAnimation,
    StateIdDead
};

const DecorateAnimation treeDeadAnimation =
{
    { TREE_DEATH1PIC, 15, ActionBurningTree },
    { TREE_DEATH2PIC, 15, ActionBurningTree },
    { TREE_DEATH3PIC, 15, ActionBurningTree },
};

const DecorateState treeStateDead =
{
    treeDeadAnimation,
    StateIdDead //  Loop
};

const std::map<DecorateStateId, DecorateState> treeStates =
{
    std::make_pair(StateIdHidden, treeStateHidden),
    std::make_pair(StateIdRise, treeStateRise),
    std::make_pair(StateIdWalk, treeStateWalk),
    std::make_pair(StateIdAttack, treeStateAttack),
    std::make_pair(StateIdPain, treeStatePain),
    std::make_pair(StateIdDying, treeStateDying),
    std::make_pair(StateIdDead, treeStateDead)
};

const DecorateActor decorateTree =
{
    actorIdMonsterTree,   // Id
    50, // spawnOnAllDifficulties;
    50, // spawnOnNormalAndHard;
    50, // spawnOnHard;
    12,  // initialHealth;
    35 * pixelRadius,    // size;
    WithYellowGem,  // radarVisibility;
    EgaBrown,   // radarColor;
    treeStates,
    StateIdHidden,   // initialState;
    7,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2500,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// BUNNY
//

// This monster initially spawns as a harmless bunny, hopping from left (StateIdPeekAlternative) to right (StateIdPeek).
const DecorateAnimation bunnyPeekAltAnimation =
{
    { BUNNY_LEFT1PIC , 55, ActionNone },
    { BUNNY_LEFT1PIC , 10, ActionHarmlessBunny },
    { BUNNY_LEFT2PIC , 30, ActionNone }
};

const DecorateState bunnyStatePeekAlt =
{
    bunnyPeekAltAnimation,
    StateIdPeekAlternative   // Loop
};

const DecorateAnimation bunnyPeekAnimation =
{
    { BUNNY_RIGHT1PIC , 55, ActionNone },
    { BUNNY_RIGHT1PIC , 10, ActionHarmlessBunny },
    { BUNNY_RIGHT2PIC , 30, ActionNone }
};

const DecorateState bunnyStatePeek =
{
    bunnyPeekAnimation,
    StateIdPeek   // Loop
};

const DecorateAnimation bunnyRiseAnimation =
{
    { BUNNY_META1PIC, 30, ActionNone },
    { BUNNY_META2PIC, 30, ActionNone }
};

const DecorateState bunnyStateRise =
{
    bunnyRiseAnimation,
    StateIdWalk
};

const DecorateAnimation bunnyWalkAnimation =
{
    { BUNNY_WALK1PIC, 25, ActionChase },
    { BUNNY_WALK2PIC, 25, ActionChase }
};

const DecorateState bunnyStateWalk =
{
    bunnyWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation bunnyAttackAnimation =
{
    { BUNNY_WALK1PIC, 25, ActionNone },
    { BUNNY_WALK2PIC, 25, ActionAttack }
};

const DecorateState bunnyStateAttack =
{
    bunnyAttackAnimation,
    StateIdWalk
};

const DecorateAnimation bunnyPainAnimation =
{
    { BUNNY_OUCHPIC, 30, ActionNone },
    { BUNNY_WALK1PIC, 50, ActionNone }
};

const DecorateState bunnyStatePain =
{
    bunnyPainAnimation,
    StateIdWalk
};

const DecorateAnimation bunnyDyingAnimation =
{
    { BUNNY_OUCHPIC, 40, ActionNone },
    { BUNNY_DEATH1PIC, 50, ActionNone },
    { BUNNY_DEATH2PIC, 20, ActionNone }
};

const DecorateState bunnyStateDying =
{
    bunnyDyingAnimation,
    StateIdDead
};

const DecorateAnimation bunnyDeadAnimation =
{
    { BUNNY_DEATH2PIC, 20, ActionNone }
};

const DecorateState bunnyStateDead =
{
    bunnyDeadAnimation,
    StateIdDead //  Loop
};

const std::map<DecorateStateId, DecorateState> bunnyStates =
{
    std::make_pair(StateIdPeekAlternative, bunnyStatePeekAlt),
    std::make_pair(StateIdPeek, bunnyStatePeek),
    std::make_pair(StateIdRise, bunnyStateRise),
    std::make_pair(StateIdWalk, bunnyStateWalk),
    std::make_pair(StateIdAttack, bunnyStateAttack),
    std::make_pair(StateIdPain, bunnyStatePain),
    std::make_pair(StateIdDying, bunnyStateDying),
    std::make_pair(StateIdDead, bunnyStateDead)
};

const DecorateActor decorateBunny =
{
    actorIdMonsterBunny,   // Id
    51, // spawnOnAllDifficulties;
    51, // spawnOnNormalAndHard;
    51, // spawnOnHard;
    10,  // initialHealth;
    35 * pixelRadius,    // size;
    Always,  // radarVisibility;
    EgaLightGray,   // radarColor;
    bunnyStates,
    StateIdPeekAlternative,   // initialState;
    4,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    1947,    // speed;
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
    { SKELETON_OUCHPIC, 8, ActionNone }
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
    37, // spawnOnAllDifficulties;
    37, // spawnOnNormalAndHard;
    37,  // spawnOnHard;
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
    { 0, 4400, ActionSpawnSkeleton },
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
    38, // spawnOnAllDifficulties;
    38, // spawnOnNormalAndHard;
    38,  // spawnOnHard;
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
// SKELETON HANGING
//

const DecorateAnimation skeletonHangingHiddenAnimation =
{
    { SKEL_HANGPIC , 20, ActionHangingSkeleton }
};

const DecorateState skeletonHangingStateHidden =
{
    skeletonHangingHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation skeletonHangingRiseAnimation =
{
    { SKEL_HANGPIC , 20, ActionNone }
};

const DecorateState skeletonHangingStateRise =
{
    skeletonHangingRiseAnimation,
    StateIdWalk
};

const std::map<DecorateStateId, DecorateState> skeletonHangingStates =
{
    std::make_pair(StateIdHidden, skeletonHangingStateHidden),
    std::make_pair(StateIdRise, skeletonHangingStateRise),
    std::make_pair(StateIdWalk, skeletonStateWalk),
    std::make_pair(StateIdAttack, skeletonStateAttack),
    std::make_pair(StateIdPain, skeletonStatePain),
    std::make_pair(StateIdDying, skeletonStateDying),
    std::make_pair(StateIdDead, skeletonStateDead)
};

const DecorateActor decorateSkeletonHanging =
{
    actorIdMonsterSkeletonHanging,   // Id
    65, // spawnOnAllDifficulties;
    65, // spawnOnNormalAndHard;
    65,  // spawnOnHard;
    12,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightYellow,   // radarColor;
    skeletonHangingStates,
    StateIdHidden,   // initialState;
    6,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2036,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// VIPER
//

const DecorateAnimation viperHiddenAnimation =
{
    { GODESS_STATUEPIC , 20, ActionStatue }
};

const DecorateState viperStateHidden =
{
    viperHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation viperRiseAnimation =
{
    { GODESS_STATUEPIC, 1, ActionNone }
};

const DecorateState viperStateRise =
{
    viperRiseAnimation,
    StateIdWalk
};

const DecorateAnimation viperWalkAnimation =
{
    { GODESS_WALK1PIC, 20, ActionChase },
    { GODESS_WALK1PIC, 20, ActionChase },
    { GODESS_WALK1PIC, 20, ActionChase }
};

const DecorateState viperStateWalk =
{
    viperWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation viperAttackAnimation =
{
    { GODESS_ATTACK1PIC, 10, ActionNone },
    { GODESS_ATTACK2PIC, 8, ActionNone },
    { GODESS_ATTACK3PIC, 10, ActionAttack },
    { GODESS_WALK1PIC, 25, ActionNone }
};

const DecorateState viperStateAttack =
{
    viperAttackAnimation,
    StateIdWalk
};

const DecorateAnimation viperPainAnimation =
{
    { GODESS_OUCHPIC, 10, ActionNone }
};

const DecorateState viperStatePain =
{
    viperPainAnimation,
    StateIdWalk
};

const DecorateAnimation viperDyingAnimation =
{
    { GODESS_OUCHPIC, 10, ActionNone },
    { GODESS_DEATH1PIC, 65, ActionNone },
    { GODESS_DEATH2PIC, 30, ActionNone }
};

const DecorateState viperStateDying =
{
    viperDyingAnimation,
    StateIdDead
};

const DecorateAnimation viperDeadAnimation =
{
    { GODESS_DEATH2PIC, 30, ActionNone }
};

const DecorateState viperStateDead =
{
    viperDeadAnimation,
    StateIdDead //  Loop
};

const std::map<DecorateStateId, DecorateState> viperStates =
{
    std::make_pair(StateIdHidden, viperStateHidden),
    std::make_pair(StateIdRise, viperStateRise),
    std::make_pair(StateIdWalk, viperStateWalk),
    std::make_pair(StateIdAttack, viperStateAttack),
    std::make_pair(StateIdPain, viperStatePain),
    std::make_pair(StateIdDying, viperStateDying),
    std::make_pair(StateIdDead, viperStateDead)
};

const DecorateActor decorateViper =
{
    actorIdMonsterViper,   // Id
    21, // spawnOnAllDifficulties;
    21, // spawnOnNormalAndHard;
    21, // spawnOnHard;
    12,  // initialHealth;
    35 * pixelRadius,    // size;
    WithGreenGem,  // radarVisibility;
    EgaGreen,   // radarColor;
    viperStates,
    StateIdHidden,   // initialState;
    15,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    3000,    // speed;
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
    25, // spawnOnNormalAndHard;
    25,  // spawnOnHard; Actually, should include 20 and 24 as well.
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
// WRETCHED POX
//

const DecorateAnimation wretchedPoxWalkAnimation =
{
    { FATDEMON_WALK1PIC, 13, ActionChase },
    { FATDEMON_WALK2PIC, 13, ActionChase },
    { FATDEMON_WALK3PIC, 13, ActionChase },
    { FATDEMON_WALK4PIC, 13, ActionChase }
};

const DecorateState wretchedPoxStateWalk =
{
    wretchedPoxWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation wretchedPoxAttackAnimation =
{
    { FATDEMON_ATTACK1PIC, 20, ActionNone },
    { FATDEMON_ATTACK2PIC, 20, ActionAttack },
    { FATDEMON_WALK1PIC, 40, ActionNone }
};

const DecorateState wretchedPoxStateAttack =
{
    wretchedPoxAttackAnimation,
    StateIdWalk
};

const DecorateAnimation wretchedPoxPainAnimation =
{
    { FATDEMON_OUCHPIC, 14, ActionNone }
};

const DecorateState wretchedPoxStatePain =
{
    wretchedPoxPainAnimation,
    StateIdWalk
};

const DecorateAnimation wretchedPoxDyingAnimation =
{
    { FATDEMON_BLOWUP1PIC, 25, ActionNone },{ FATDEMON_BLOWUP2PIC, 25, ActionNone },
    { FATDEMON_BLOWUP1PIC, 15, ActionNone },{ FATDEMON_BLOWUP2PIC, 15, ActionNone },
    { FATDEMON_BLOWUP1PIC, 6, ActionNone },{ FATDEMON_BLOWUP2PIC, 6, ActionNone },
    { FATDEMON_BLOWUP1PIC, 6, ActionNone },{ FATDEMON_BLOWUP2PIC, 6, ActionNone },
    { FATDEMON_BLOWUP1PIC, 6, ActionNone },{ FATDEMON_BLOWUP2PIC, 6, ActionNone },
    { FATDEMON_BLOWUP1PIC, 6, ActionNone },{ FATDEMON_BLOWUP2PIC, 6, ActionNone },
    { FATDEMON_BLOWUP1PIC, 6, ActionNone },{ FATDEMON_BLOWUP2PIC, 6, ActionNone },
    { FATDEMON_BLOWUP1PIC, 6, ActionNone },{ FATDEMON_BLOWUP2PIC, 6, ActionNone },
    { FATDEMON_BLOWUP3PIC, 30, ActionNone },
    { FATDEMON_EXPLODEPIC, 40, ActionExplodeSound },
    { FATDEMON_FEETPIC, 30, ActionNone },
};

const DecorateState wretchedPoxStateDying =
{
    wretchedPoxDyingAnimation,
    StateIdDead
};

const DecorateAnimation wretchedPoxDeadAnimation =
{
    { FATDEMON_FEETPIC, 30, ActionNone }
};

const DecorateState wretchedPoxStateDead =
{
    wretchedPoxDeadAnimation,
    StateIdDead //  Loop
};

const std::map<DecorateStateId, DecorateState> wretchedPoxStates =
{
    std::make_pair(StateIdWalk, wretchedPoxStateWalk),
    std::make_pair(StateIdAttack, wretchedPoxStateAttack),
    std::make_pair(StateIdPain, wretchedPoxStatePain),
    std::make_pair(StateIdDying, wretchedPoxStateDying),
    std::make_pair(StateIdDead, wretchedPoxStateDead)
};

const DecorateActor decorateWretchedPox =
{
    actorIdMonsterWretchedPox,   // Id
    22, // spawnOnAllDifficulties;
    22, // spawnOnNormalAndHard;
    22,  // spawnOnHard; Actually, should include 20 and 24 as well.
    10,  // initialHealth;
    35 * pixelRadius,    // size;
    WithGreenGem,  // radarVisibility;
    EgaBrightGreen,   // radarColor;
    wretchedPoxStates,
    StateIdWalk,   // initialState;
    8,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2500,    // speed;
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
    { RED_DEMONATTACK3PIC, 30, ActionAttack }
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
    { RED_DEMONOUCHPIC, 9, ActionNone }, { RED_DEMONDIE1PIC, 9, ActionNone },
    { RED_DEMONOUCHPIC, 9, ActionNone }, { RED_DEMONDIE1PIC, 9, ActionNone },
    { RED_DEMONOUCHPIC, 9, ActionNone }, { RED_DEMONDIE1PIC, 9, ActionNone },
    { RED_DEMONOUCHPIC, 9, ActionNone }, { RED_DEMONDIE1PIC, 9, ActionNone },
    { RED_DEMONOUCHPIC, 9, ActionNone }, { RED_DEMONDIE1PIC, 9, ActionNone },
    { RED_DEMONOUCHPIC, 9, ActionNone }, { RED_DEMONDIE1PIC, 9, ActionNone },
    { RED_DEMONDIE2PIC, 20, ActionNone },
    { RED_DEMONDIE3PIC, 10, ActionNone }
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
    20, // spawnOnNormalAndHard;
    20,  // spawnOnHard;
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
    27, // spawnOnNormalAndHard;
    27,  // spawnOnHard
    12,  // initialHealth;
    35 * pixelRadius,    // size;
    WithRedGem,  // radarVisibility;
    EgaBrightRed,   // radarColor;
    mageStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    3072,    // speed;
    0, // actionParameter
    actorIdProjectileMageShot // ProjectileId
};

//
// SUCCUBUS
//

const DecorateAnimation succubusWalkAnimation =
{
    { SUCCUBUS_WALK1PIC, 10, ActionChaseLikeMage },
    { SUCCUBUS_WALK2PIC, 10, ActionChaseLikeMage },
    { SUCCUBUS_WALK3PIC, 10, ActionChaseLikeMage },
    { SUCCUBUS_WALK4PIC, 10, ActionChaseLikeMage }
};

const DecorateState succubusStateWalk =
{
    succubusWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation succubusAttackAnimation =
{
    { SUCCUBUS_ATTACK1PIC, 15, ActionNone },
    { SUCCUBUS_ATTACK2PIC, 15, ActionAttack },
    { SUCCUBUS_WALK2PIC, 10, ActionNone }
};

const DecorateState succubusStateAttack =
{
    succubusAttackAnimation,
    StateIdWalk
};

const DecorateAnimation succubusPainAnimation =
{
    { SUCCUBUS_OUCHPIC, 15, ActionNone }
};

const DecorateState succubusStatePain =
{
    succubusPainAnimation,
    StateIdWalk
};

const DecorateAnimation succubusDyingAnimation =
{
    { SUCCUBUS_DEATH1PIC, 55, ActionNone }
};

const DecorateState succubusStateDying =
{
    succubusDyingAnimation,
    StateIdDead
};

const DecorateAnimation succubusDeadAnimation =
{
    { SUCCUBUS_DEATH2PIC, 20, ActionNone }
};

const DecorateState succubusStateDead =
{
    succubusDeadAnimation,
    StateIdDead // Loop
};

const std::map<DecorateStateId, DecorateState> succubusStates =
{
    std::make_pair(StateIdWalk, succubusStateWalk),
    std::make_pair(StateIdAttack, succubusStateAttack),
    std::make_pair(StateIdPain, succubusStatePain),
    std::make_pair(StateIdDying, succubusStateDying),
    std::make_pair(StateIdDead, succubusStateDead)
};

const DecorateActor decorateSuccubus =
{
    actorIdMonsterSuccubus,   // Id
    23, // spawnOnAllDifficulties;
    23, // spawnOnNormalAndHard;
    23,  // spawnOnHard
    12,  // initialHealth;
    30 * pixelRadius,    // size;
    WithBlueGem,  // radarVisibility;
    EgaBrightBlue,   // radarColor;
    succubusStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2500,    // speed;
    0, // actionParameter
    actorIdProjectileSuccubusShot // ProjectileId
};

//
// DRAGON
//

const DecorateAnimation dragonHiddenAnimation =
{
    { DRAGON_BUBBLES1PIC, 13, ActionHideUnderWater },
    { DRAGON_BUBBLES2PIC, 15, ActionHideUnderWater },
    { 0, 35, ActionHideUnderWater }
};

const DecorateState dragonStateHidden =
{
    dragonHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation dragonRiseAnimation =
{
    { DRAGON_BUBBLES2PIC, 15, ActionNone },
    { DRAGON_EYESPIC, 20, ActionNone },
    { DRAGON_RISE1PIC, 20, ActionNone },
    { DRAGON_RISE2PIC, 20, ActionNone }
};

const DecorateState dragonStateRise =
{
    dragonRiseAnimation,
    StateIdWalk
};

const DecorateAnimation dragonSinkAnimation =
{
    { DRAGON_RISE2PIC, 20, ActionNone },
    { DRAGON_RISE1PIC, 20, ActionNone },
    { DRAGON_EYESPIC, 20, ActionNone }
};

const DecorateState dragonStateSink =
{
    dragonSinkAnimation,
    StateIdHidden
};

const DecorateAnimation dragonPeekAnimation =
{
    { DRAGON_EYESPIC, 45, ActionNone }
};

const DecorateState dragonStatePeek =
{
    dragonPeekAnimation,
    StateIdHidden
};

const DecorateAnimation dragonWalkAnimation =
{
    { DRAGON_WALK1PIC, 12, ActionChaseLikeWetMan },
    { DRAGON_WALK2PIC, 12, ActionChaseLikeWetMan },
    { DRAGON_WALK3PIC, 12, ActionChaseLikeWetMan },
    { DRAGON_WALK4PIC, 12, ActionChaseLikeWetMan }
};

const DecorateState dragonStateWalk =
{
    dragonWalkAnimation,
    StateIdWalk // Loop
};

const DecorateAnimation dragonAttackAnimation =
{
    { DRAGON_ATTACK1PIC, 10, ActionNone },
    { DRAGON_ATTACK2PIC, 20, ActionNone },
    { DRAGON_ATTACK3PIC, 10, ActionAttack }
};

const DecorateState dragonStateAttack =
{
    dragonAttackAnimation,
    StateIdWalk
};

const DecorateAnimation dragonPainAnimation =
{
    { DRAGON_OUCHPIC, 10, ActionNone }
};

const DecorateState dragonStatePain =
{
    dragonPainAnimation,
    StateIdWalk
};

const DecorateAnimation dragonDyingAnimation =
{
    { DRAGON_DEATH1PIC, 27, ActionNone },
    { DRAGON_DEATH2PIC, 29, ActionNone },
    { DRAGON_DEATH3PIC, 44, ActionNone },
    { DRAGON_BUBBLES2PIC, 26, ActionNone },
    { DRAGON_BUBBLES1PIC, 23, ActionNone },
    { 0, 22, ActionRemove }
};

const DecorateState dragonStateDying =
{
    dragonDyingAnimation,
    StateIdDying  // Will not loop, due to the ActionRemove.
};

const std::map<DecorateStateId, DecorateState> dragonStates =
{
    std::make_pair(StateIdHidden, dragonStateHidden),
    std::make_pair(StateIdRise, dragonStateRise),
    std::make_pair(StateIdSink, dragonStateSink),
    std::make_pair(StateIdPeek, dragonStatePeek),
    std::make_pair(StateIdWalk, dragonStateWalk),
    std::make_pair(StateIdAttack, dragonStateAttack),
    std::make_pair(StateIdPain, dragonStatePain),
    std::make_pair(StateIdDying, dragonStateDying)
};

const DecorateActor decorateDragon =
{
    actorIdMonsterDragon,   // Id
    24, // spawnOnAllDifficulties;
    24, // spawnOnNormalAndHard;
    24,  // spawnOnHard
    20,  // initialHealth;
    35 * pixelRadius,    // size;
    WithBlueGem,  // radarVisibility;
    EgaBlue,   // radarColor;
    dragonStates,
    StateIdHidden,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2200,    // speed;
    0, // actionParameter
    actorIdProjectileDragonShot // ProjectileId
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
    { EYE_SCOWLPIC, 20, ActionAttack }
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
    26, // spawnOnAllDifficulties;
    26, // spawnOnNormalAndHard;
    26,  // spawnOnHard
    15,  // initialHealth;
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
