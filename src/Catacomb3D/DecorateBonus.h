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

#include "../Engine/Decorate.h"
#include "EgaGraphCatacomb3D.h"
#include "AudioRepositoryCatacomb3D.h"

const uint16_t actorIdBonusExplosion = 20;
const uint16_t actorIdBonusBolt = 21;
const uint16_t actorIdBonusNuke = 22;
const uint16_t actorIdBonusPotion = 23;
const uint16_t actorIdBonusChest = 24;
const uint16_t actorIdBonusScroll1 = 27;
const uint16_t actorIdBonusScroll2 = 28;
const uint16_t actorIdBonusScroll3 = 29;
const uint16_t actorIdBonusScroll4 = 30;
const uint16_t actorIdBonusScroll5 = 31;
const uint16_t actorIdBonusScroll6 = 32;
const uint16_t actorIdBonusScroll7 = 33;
const uint16_t actorIdBonusScroll8 = 34;
const uint16_t actorIdBonusKeyRed = 35;
const uint16_t actorIdBonusKeyYellow = 36;
const uint16_t actorIdBonusKeyGreen = 37;
const uint16_t actorIdBonusKeyBlue = 38;
const uint16_t actorIdGrelminar = 45;

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
// BOLT
//

const DecorateAnimation boltWaitForPickupAnimation =
{
    { BOLTOBJPIC, 8, ActionWaitForPickup },
    { BOLTOBJ2PIC, 8, ActionWaitForPickup }
};

const DecorateState boltWaitForPickup =
{
    boltWaitForPickupAnimation,
    StateIdWaitForPickup
};

const DecorateAnimation boltPickupAnimation =
{
    { 0, 6, ActionGiveBolt },
    { 0, 6, ActionRemove }
};

const DecorateState boltPickup =
{
    boltPickupAnimation,
    StateIdPickup
};

const std::map<DecorateStateId, DecorateState> boltStates =
{
    std::make_pair(StateIdWaitForPickup, boltWaitForPickup),
    std::make_pair(StateIdPickup, boltPickup)
};

const DecorateActor decorateBolt =
{
    actorIdBonusBolt,   // Id
    5, // spawnOnAllDifficulties;
    5, // spawnOnNormalAndHard;
    5,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    boltStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// NUKE
//

const DecorateAnimation nukeWaitForPickupAnimation =
{
    { NUKEOBJPIC, 8, ActionWaitForPickup },
    { NUKEOBJ2PIC, 8, ActionWaitForPickup }
};

const DecorateState nukeWaitForPickup =
{
    nukeWaitForPickupAnimation,
    StateIdWaitForPickup
};

const DecorateAnimation nukePickupAnimation =
{
    { 0, 6, ActionGiveNuke },
    { 0, 6, ActionRemove }
};

const DecorateState nukePickup =
{
    nukePickupAnimation,
    StateIdPickup
};

const std::map<DecorateStateId, DecorateState> nukeStates =
{
    std::make_pair(StateIdWaitForPickup, nukeWaitForPickup),
    std::make_pair(StateIdPickup, nukePickup)
};

const DecorateActor decorateNuke =
{
    actorIdBonusNuke,   // Id
    6, // spawnOnAllDifficulties;
    6, // spawnOnNormalAndHard;
    6,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    nukeStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// POTION
//

const DecorateAnimation potionWaitForPickupAnimation =
{
    { POTIONOBJPIC, 8, ActionWaitForPickup }
};

const DecorateState potionWaitForPickup =
{
    potionWaitForPickupAnimation,
    StateIdWaitForPickup
};

const DecorateAnimation potionPickupAnimation =
{
    { 0, 6, ActionGivePotion },
    { 0, 6, ActionRemove }
};

const DecorateState potionPickup =
{
    potionPickupAnimation,
    StateIdPickup
};

const std::map<DecorateStateId, DecorateState> potionStates =
{
    std::make_pair(StateIdWaitForPickup, potionWaitForPickup),
    std::make_pair(StateIdPickup, potionPickup)
};

const DecorateActor decoratePotion =
{
    actorIdBonusPotion,   // Id
    7, // spawnOnAllDifficulties;
    7, // spawnOnNormalAndHard;
    7,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    potionStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// CHEST
//

const DecorateAnimation chestWaitForPickupAnimation
{
    { CHESTOBJPIC, 8, ActionWaitForPickup }
};

const DecorateState chestWaitForPickup =
{
    chestWaitForPickupAnimation,
    StateIdWaitForPickup
};

const DecorateAnimation chestPickupAnimation =
{
    { 0, 6, ActionGivePointsForChest },
    { 0, 6, ActionRemove }
};

const DecorateState chestPickup =
{
    chestPickupAnimation,
    StateIdPickup
};

const std::map<DecorateStateId, DecorateState> chestStates =
{
    std::make_pair(StateIdWaitForPickup, chestWaitForPickup),
    std::make_pair(StateIdPickup, chestPickup)
};

const DecorateActor decorateChest =
{
    actorIdBonusChest,   // Id
    21, // spawnOnAllDifficulties;
    21, // spawnOnNormalAndHard;
    21,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    chestStates,
    StateIdWaitForPickup,   // initialState;
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

//
// KEY RED
//

const DecorateAnimation keyRedWaitForPickupAnimation =
{
    { RKEYOBJPIC, 6, ActionWaitForPickup }
};

const DecorateState keyRedWaitForPickup =
{
    keyRedWaitForPickupAnimation,
    StateIdWaitForPickup
};

const DecorateAnimation keyPickupAnimation =
{
    { 0, 6, ActionGiveKey },
    { 0, 6, ActionRemove }
};

const DecorateState keyPickup =
{
    keyPickupAnimation,
    StateIdPickup
};

const std::map<DecorateStateId, DecorateState> keyRedStates =
{
    std::make_pair(StateIdWaitForPickup, keyRedWaitForPickup),
    std::make_pair(StateIdPickup, keyPickup),
};

const DecorateActor decorateKeyRed =
{
    actorIdBonusKeyRed,   // Id
    8, // spawnOnAllDifficulties;
    8, // spawnOnNormalAndHard;
    8,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    keyRedStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// KEY YELLOW
//

const DecorateAnimation keyYellowWaitForPickupAnimation =
{
    { YKEYOBJPIC, 6, ActionWaitForPickup }
};

const DecorateState keyYellowWaitForPickup =
{
    keyYellowWaitForPickupAnimation,
    StateIdWaitForPickup
};

const std::map<DecorateStateId, DecorateState> keyYellowStates =
{
    std::make_pair(StateIdWaitForPickup, keyYellowWaitForPickup),
    std::make_pair(StateIdPickup, keyPickup),
};

const DecorateActor decorateKeyYellow =
{
    actorIdBonusKeyYellow,   // Id
    9, // spawnOnAllDifficulties;
    9, // spawnOnNormalAndHard;
    9,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    keyYellowStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    1, // actionParameter
    0 // ProjectileId
};

//
// KEY GREEN
//

const DecorateAnimation keyGreenWaitForPickupAnimation =
{
    { GKEYOBJPIC, 6, ActionWaitForPickup }
};

const DecorateState keyGreenWaitForPickup =
{
    keyGreenWaitForPickupAnimation,
    StateIdWaitForPickup
};

const std::map<DecorateStateId, DecorateState> keyGreenStates =
{
    std::make_pair(StateIdWaitForPickup, keyGreenWaitForPickup),
    std::make_pair(StateIdPickup, keyPickup),
};

const DecorateActor decorateKeyGreen =
{
    actorIdBonusKeyGreen,   // Id
    10, // spawnOnAllDifficulties;
    10, // spawnOnNormalAndHard;
    10,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    keyGreenStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    2, // actionParameter
    0 // ProjectileId
};

//
// KEY BLUE
//

const DecorateAnimation keyBlueWaitForPickupAnimation =
{
    { BKEYOBJPIC, 6, ActionWaitForPickup }
};

const DecorateState keyBlueWaitForPickup =
{
    keyBlueWaitForPickupAnimation,
    StateIdWaitForPickup
};

const std::map<DecorateStateId, DecorateState> keyBlueStates =
{
    std::make_pair(StateIdWaitForPickup, keyBlueWaitForPickup),
    std::make_pair(StateIdPickup, keyPickup),
};

const DecorateActor decorateKeyBlue =
{
    actorIdBonusKeyBlue,   // Id
    11, // spawnOnAllDifficulties;
    11, // spawnOnNormalAndHard;
    11,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    keyBlueStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    3, // actionParameter
    0 // ProjectileId
};

//
// GRELMINAR (ends the game when picked up)
//

const DecorateAnimation grelminarWaitForPickupAnimation
{
    { NEMESISPIC, 8, ActionWaitForPickup }
};

const DecorateState grelminarWaitForPickup =
{
    grelminarWaitForPickupAnimation,
    StateIdWaitForPickup
};

const DecorateAnimation grelminarPickupAnimation =
{
    { NEMESISPIC, 1, ActionGivePoints },
    { NEMESISPIC, 60, ActionVictory }
};

const DecorateState grelminarPickup =
{
    grelminarPickupAnimation,
    StateIdPickup
};

const std::map<DecorateStateId, DecorateState> grelminarStates =
{
    std::make_pair(StateIdWaitForPickup, grelminarWaitForPickup),
    std::make_pair(StateIdPickup, grelminarPickup)
};

const DecorateActor decorateGrelminar =
{
    actorIdGrelminar,   // Id
    20, // spawnOnAllDifficulties;
    20, // spawnOnNormalAndHard;
    20,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    grelminarStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    100000, // actionParameter - # points added when picked up
    0 // ProjectileId
};