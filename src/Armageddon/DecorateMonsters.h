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

#include "..\Engine\Decorate.h"
#include "DecorateProjectiles.h"
#include "EgaGraphArmageddon.h"
#include "AudioRepositoryArmageddon.h"

const uint16_t actorIdMonsterZombie = 50;
const uint16_t actorIdMonsterTree = 53;
const uint16_t actorIdMonsterBat = 58;

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
// TREE
//

const DecorateAnimation treeHiddenAnimation =
{
    { TREE_IDLEPIC , 13, ActionHide }
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
    { TREE_DEATH1PIC, 15, ActionNone },
    { TREE_DEATH2PIC, 15, ActionNone },
    { TREE_DEATH3PIC, 15, ActionNone }
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
    { NULL, 18, ActionRemove }
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
