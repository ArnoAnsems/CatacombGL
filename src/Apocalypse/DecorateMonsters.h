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

const uint16_t actorIdMonsterWizard = 59;

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
