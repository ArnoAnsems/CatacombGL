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
// Actor definitions for the Catacomb 3-D monsters.
//
#pragma once

#include "..\Engine\Decorate.h"
#include "DecorateProjectiles.h"
#include "EgaGraphCatacomb3D.h"
#include "AudioRepositoryCatacomb3D.h"

const uint16_t actorIdMonsterOrc = 53;
const uint16_t actorIdMonsterTroll = 54;
const uint16_t actorIdMonsterDemon = 55;
const uint16_t actorIdMonsterBounce = 56;
const uint16_t actorIdMonsterBat = 58;
const uint16_t actorIdMonsterMage = 59;
const uint16_t actorIdMonsterNemesis = 62;

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
    { ORCATTACK2PIC, 50, ActionAttack },
    { ORC1PIC, 40, ActionNone }
};

const DecorateState orcStateAttack =
{
    orcAttackAnimation,
    StateIdWalk
};

const DecorateAnimation orcPainAnimation =
{
    { ORCOUCHPIC, 10, ActionNone }
};

const DecorateState orcStatePain =
{
    orcPainAnimation,
    StateIdWalk
};

const DecorateAnimation orcDyingAnimation =
{
    { ORCDIE1PIC, 8, ActionGivePoints },
    { ORCDIE1PIC, 8, ActionNone }
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
    3,  // initialHealth;
    32 * pixelRadius,    // size;
    WithGreenGem,  // radarVisibility;
    EgaGreen,   // radarColor;
    orcStates,
    StateIdWalk,   // initialState;
    4,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    1536,    // speed;
    100, // actionParameter - # points added when killed
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
    { TROLLATTACK1PIC, 20, ActionNone },
    { TROLLATTACK2PIC, 50, ActionAttack },
    { TROLL1PIC, 40, ActionNone }
};

const DecorateState trollStateAttack =
{
    trollAttackAnimation,
    StateIdWalk
};

const DecorateAnimation trollPainAnimation =
{
    { TROLLOUCHPIC, 8, ActionChase }
};

const DecorateState trollStatePain =
{
    trollPainAnimation,
    StateIdWalk
};

const DecorateAnimation trollDyingAnimation =
{
    { TROLLDIE1PIC, 8, ActionGivePoints },
    { TROLLDIE2PIC, 8, ActionNone }
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
    10,  // initialHealth;
    40 * pixelRadius,    // size;
    WithGreenGem,  // radarVisibility;
    EgaBrightGreen,   // radarColor;
    trollStates,
    StateIdWalk,   // initialState;
    8,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2500,    // speed;
    400, // actionParameter - # points added when killed
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
    { DEMONATTACK2PIC, 20, ActionAttack },
    { DEMONATTACK3PIC, 30, ActionNone },
    { DEMON1PIC, 40, ActionNone }
};

const DecorateState demonStateAttack =
{
    demonAttackAnimation,
    StateIdWalk
};

const DecorateAnimation demonPainAnimation =
{
    { DEMONOUCHPIC, 10, ActionNone }
};

const DecorateState demonStatePain =
{
    demonPainAnimation,
    StateIdWalk
};

const DecorateAnimation demonDyingAnimation =
{
    { DEMONDIE1PIC, 20, ActionGivePoints },
    { DEMONDIE2PIC, 20, ActionNone },
    { DEMONDIE3PIC, 10, ActionNone }
};

const DecorateState demonStateDying =
{
    demonDyingAnimation,
    StateIdDead
};

const DecorateAnimation demonDeadAnimation =
{
    { DEMONDIE3PIC, 10, ActionNone }
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
    32 * pixelRadius,    // size;
    WithBlueGem,  // radarVisibility;
    EgaBlue,   // radarColor;
    demonStates,
    StateIdWalk,   // initialState;
    15,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2048,    // speed;
    1000, // actionParameter - # points added when killed
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
    { GRELATTACKPIC, 20, ActionNone },
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
    { GRELDIE1PIC, 20, ActionGivePoints },
    { GRELDIE2PIC, 20, ActionNone },
    { GRELDIE3PIC, 20, ActionNone },
    { GRELDIE4PIC, 20, ActionNone },
    { GRELDIE5PIC, 20, ActionNone }
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
    56 * pixelRadius,    // size;
    WithAllGems,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    nemesisStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2048,    // speed;
    10000, // actionParameter - # points added when killed
    actorIdProjectileMageShot // ProjectileId
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
    { BATDIE1PIC, 18, ActionGivePoints },
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
    24 * pixelRadius,    // size;
    Always,  // radarVisibility;
    EgaDarkGray,   // radarColor;
    batStates,
    StateIdWalk,   // initialState;
    4,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2000,    // speed;
    100, // actionParameter - # points added when killed
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
    { MAGEATTACKPIC, 20, ActionNone },
    { MAGEATTACKPIC, 30, ActionAttack },
    { MAGE1PIC, 100, ActionNone }
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
    { MAGEDIE1PIC, 20, ActionGivePoints }
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
    32 * pixelRadius,    // size;
    WithAllGems,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    mageStates,
    StateIdWalk,   // initialState;
    0,  // meleeDamage;
    SHOOTMONSTERSND, // hitSound;
    2048,    // speed;
    600, // actionParameter - # points added when killed
    actorIdProjectileMageShot // ProjectileId
};

//
// BOUNCE
//

const DecorateAnimation bounceWalkAnimation =
{
    { BIGPSHOT1PIC, 8, ActionBounce },
    { BIGPSHOT2PIC, 8, ActionBounce }
};

const DecorateState bounceStateWalk =
{
    bounceWalkAnimation,
    StateIdWalk   // Loop
};

const DecorateAnimation bounceAttackAnimation =
{
    { BIGPSHOT1PIC , 1, ActionAttack }
};

const DecorateState bounceStateAttack =
{
    bounceAttackAnimation,
    StateIdWalk
};

const std::map<DecorateStateId, DecorateState> bounceStates =
{
    std::make_pair(StateIdWalk, bounceStateWalk),
    std::make_pair(StateIdAttack, bounceStateAttack)
};

const DecorateActor decorateBounce =
{
    actorIdMonsterBounce,   // Id
    29, // spawnOnAllDifficulties;
    29, // spawnOnNormalAndHard;
    29,  // spawnOnHard;
    10,  // initialHealth;
    24 * pixelRadius,    // size;
    Flickering,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    bounceStates,
    StateIdWalk,   // initialState;
    10,  // damage;
    SHOOTMONSTERSND, // hitSound;
    4096,    // speed;
    0, // actionParameter
    0 // ProjectileId
};