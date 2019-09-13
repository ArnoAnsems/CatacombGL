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
// DecorateBonus
//
// Actor definitions for the Catacomb 3-D bonus items.
//
#pragma once

#include "..\Engine\Decorate.h"
#include "EgaGraphCatacomb3D.h"
#include "AudioRepositoryCatacomb3D.h"

const uint16_t actorIdBonusExplosion = 20;
const uint16_t actorIdBonusScroll1 = 27;
const uint16_t actorIdBonusScroll2 = 28;
const uint16_t actorIdBonusScroll3 = 29;
const uint16_t actorIdBonusScroll4 = 30;
const uint16_t actorIdBonusScroll5 = 31;
const uint16_t actorIdBonusScroll6 = 32;
const uint16_t actorIdBonusScroll7 = 33;
const uint16_t actorIdBonusScroll8 = 34;


//
// EXPLOSION
//

const DecorateAnimation explosionHiddenAnimation =
{
    { 0 , 2, ActionExplosionDelay }
};

const DecorateState explosionStateHidden =
{
    explosionHiddenAnimation,
    StateIdHidden   // Loop
};

const DecorateAnimation explosionDyingAnimation =
{
    { PSHOT1PIC , 8, ActionNone },
    { 0 , 8, ActionRemove },
};

const DecorateState explosionStateDying =
{
    explosionDyingAnimation,
    StateIdDying   // Loop
};

const std::map<DecorateStateId, DecorateState> explosionStates =
{
    std::make_pair(StateIdHidden, explosionStateHidden),
    std::make_pair(StateIdDying, explosionStateDying)
};

const DecorateActor decorateExplosion =
{
    actorIdBonusExplosion,   // Id
    0, // spawnOnAllDifficulties;
    0, // spawnOnNormalAndHard;
    0,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    explosionStates,
    StateIdHidden,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// SCROLLS
//

const DecorateAnimation scrollWaitForPickupAnimation =
{
    { SCROLLOBJPIC, 6, ActionWaitForPickup }
};

const DecorateState scrollWaitForPickup =
{
    scrollWaitForPickupAnimation,
    StateIdWaitForPickup
};

const DecorateAnimation scrollPickupAnimation =
{
    { 0, 6, ActionGiveScroll },
    { 0, 6, ActionRemove }
};

const DecorateState scrollPickup =
{
    scrollPickupAnimation,
    StateIdPickup
};

const std::map<DecorateStateId, DecorateState> scrollStates =
{
    std::make_pair(StateIdWaitForPickup, scrollWaitForPickup),
    std::make_pair(StateIdPickup, scrollPickup),
};

const DecorateActor decorateScroll1 =
{
    actorIdBonusScroll1,   // Id
    12, // spawnOnAllDifficulties;
    12, // spawnOnNormalAndHard;
    12,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    scrollStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

const DecorateActor decorateScroll2 =
{
    actorIdBonusScroll2,   // Id
    13, // spawnOnAllDifficulties;
    13, // spawnOnNormalAndHard;
    13,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    scrollStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    1, // actionParameter
    0 // ProjectileId
};

const DecorateActor decorateScroll3 =
{
    actorIdBonusScroll3,   // Id
    14, // spawnOnAllDifficulties;
    14, // spawnOnNormalAndHard;
    14,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    scrollStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    2, // actionParameter
    0 // ProjectileId
};

const DecorateActor decorateScroll4 =
{
    actorIdBonusScroll4,   // Id
    15, // spawnOnAllDifficulties;
    15, // spawnOnNormalAndHard;
    15,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    scrollStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    3, // actionParameter
    0 // ProjectileId
};

const DecorateActor decorateScroll5 =
{
    actorIdBonusScroll5,   // Id
    16, // spawnOnAllDifficulties;
    16, // spawnOnNormalAndHard;
    16,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    scrollStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    4, // actionParameter
    0 // ProjectileId
};

const DecorateActor decorateScroll6 =
{
    actorIdBonusScroll6,   // Id
    17, // spawnOnAllDifficulties;
    17, // spawnOnNormalAndHard;
    17,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    scrollStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    5, // actionParameter
    0 // ProjectileId
};

const DecorateActor decorateScroll7 =
{
    actorIdBonusScroll7,   // Id
    18, // spawnOnAllDifficulties;
    18, // spawnOnNormalAndHard;
    18,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    scrollStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    6, // actionParameter
    0 // ProjectileId
};

const DecorateActor decorateScroll8 =
{
    actorIdBonusScroll8,   // Id
    19, // spawnOnAllDifficulties;
    19, // spawnOnNormalAndHard;
    19,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    scrollStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    7, // actionParameter
    0 // ProjectileId
};
