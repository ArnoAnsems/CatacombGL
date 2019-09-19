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
    { ORCDIE1PIC, 8, ActionNone },
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
    { TROLLDIE1PIC, 8, ActionNone },
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
    0, // actionParameter
    0 // ProjectileId
};
