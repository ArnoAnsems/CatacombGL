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
// Actor definitions for the Catacomb Apocalypse bonus items.
//
#pragma once

#include "../Engine/Decorate.h"
#include "EgaGraphApocalypse.h"
#include "AudioRepositoryApocalypse.h"

const uint16_t actorIdBonusExplosion = 20;
const uint16_t actorIdBonusBolt = 21;
const uint16_t actorIdBonusNuke = 22;
const uint16_t actorIdBonusPotion = 23;
const uint16_t actorIdBonusChest = 24;
const uint16_t actorIdBonusWaterChest = 25;
const uint16_t actorIdBonusFreezeTime = 26;
const uint16_t actorIdBonusOldChest = 27;
const uint16_t actorIdBonusKeyRed = 35;
const uint16_t actorIdBonusKeyYellow = 36;
const uint16_t actorIdBonusKeyGreen = 37;
const uint16_t actorIdBonusKeyBlue = 38;
const uint16_t actorIdBonusGemRed = 40;
const uint16_t actorIdBonusGemYellow = 41;
const uint16_t actorIdBonusGemGreen = 42;
const uint16_t actorIdBonusGemBlue = 43;
const uint16_t actorIdBonusGemPurple = 44;

//
// BONUS ITEM DESTROYED
//

const DecorateAnimation bonusDyingAnimation =
{
    { PSHOT_EXP1PIC, 7, ActionItemDestroyed },
    { PSHOT_EXP2PIC , 7, ActionNone },
    { PSHOT_EXP3PIC , 7, ActionNone },
    { 0 , 7, ActionRemove },
};
const DecorateState bonusStateDying =
{
    bonusDyingAnimation,
    StateIdDying   // Loop
};

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
    { PSHOT_EXP1PIC , 7, ActionNone },
    { PSHOT_EXP2PIC , 7, ActionNone },
    { PSHOT_EXP3PIC , 7, ActionNone },
    { 0 , 7, ActionRemove },
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
    { BOLTOBJPIC, 8, ActionWaitForPickupDestructable },
    { BOLT2OBJPIC, 8, ActionWaitForPickupDestructable },
    { BOLT3OBJPIC, 8, ActionWaitForPickupDestructable },
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
    std::make_pair(StateIdPickup, boltPickup),
    std::make_pair(StateIdDying, bonusStateDying)
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
    BOOMSND, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// NUKE
//

const DecorateAnimation nukeWaitForPickupAnimation =
{
    { NUKEOBJPIC, 8, ActionWaitForPickupDestructable },
    { NUKE2OBJPIC, 8, ActionWaitForPickupDestructable },
    { NUKE3OBJPIC, 8, ActionWaitForPickupDestructable },
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
    std::make_pair(StateIdPickup, nukePickup),
    std::make_pair(StateIdDying, bonusStateDying)
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
    BOOMSND, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// POTION
//

const DecorateAnimation potionWaitForPickupAnimation =
{
    { POTIONOBJPIC, 8, ActionWaitForPickupDestructable }
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
    std::make_pair(StateIdPickup, potionPickup),
    std::make_pair(StateIdDying, bonusStateDying)
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
    BOOMSND, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// CHEST
//

const DecorateAnimation chestWaitForPickupAnimation
{
    { CHESTOBJPIC, 8, ActionWaitForPickupDestructable }
};

const DecorateState chestWaitForPickup =
{
    chestWaitForPickupAnimation,
    StateIdWaitForPickup
};

const DecorateAnimation chestPickupAnimation =
{
    { 0, 6, ActionGiveChest },
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
    std::make_pair(StateIdPickup, chestPickup),
    std::make_pair(StateIdDying, bonusStateDying)
};

const DecorateActor decorateChest =
{
    actorIdBonusChest,   // Id
    49, // spawnOnAllDifficulties;
    49, // spawnOnNormalAndHard;
    49,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    chestStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    BOOMSND, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// WATER CHEST
//

const DecorateAnimation waterChestWaitForPickupAnimation =
{
    { O_WATER_CHEST1PIC, 8, ActionWaitForPickupDestructable },
    { O_WATER_CHEST2PIC, 8, ActionWaitForPickupDestructable }
};

const DecorateState waterChestWaitForPickup =
{
    waterChestWaitForPickupAnimation,
    StateIdWaitForPickup
};

const std::map<DecorateStateId, DecorateState> waterChestStates =
{
    std::make_pair(StateIdWaitForPickup, waterChestWaitForPickup),
    std::make_pair(StateIdPickup, chestPickup),
    std::make_pair(StateIdDying, bonusStateDying)
};

const DecorateActor decorateWaterChest =
{
    actorIdBonusWaterChest,   // Id
    49, // spawnOnAllDifficulties;
    49, // spawnOnNormalAndHard;
    49,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    waterChestStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    BOOMSND, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// OLD CHEST
//

const DecorateAnimation oldChestWaitForPickupAnimation
{
    { OLD_CHESTPIC, 8, ActionWaitForPickupDestructable }
};

const DecorateState oldChestWaitForPickup =
{
    oldChestWaitForPickupAnimation,
    StateIdWaitForPickup
};

const DecorateAnimation oldChestPickupAnimation =
{
    { 0, 6, ActionGiveChest },
    { 0, 6, ActionRemove }
};

const DecorateState oldChestPickup =
{
    oldChestPickupAnimation,
    StateIdPickup
};

const std::map<DecorateStateId, DecorateState> oldChestStates =
{
    std::make_pair(StateIdWaitForPickup, oldChestWaitForPickup),
    std::make_pair(StateIdPickup, oldChestPickup),
    std::make_pair(StateIdDying, bonusStateDying)
};

const DecorateActor decorateOldChest =
{
    actorIdBonusOldChest,   // Id
    48, // spawnOnAllDifficulties;
    48, // spawnOnNormalAndHard;
    48,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    oldChestStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    BOOMSND, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// FREEZE TIME
//

const DecorateAnimation freezeTimeWaitForPickupAnimation =
{
    { TIMEOBJ1PIC, 6, ActionWaitForPickupDestructable },
    { TIMEOBJ2PIC, 6, ActionWaitForPickupDestructable }
};

const DecorateState freezeTimeWaitForPickup =
{
    freezeTimeWaitForPickupAnimation,
    StateIdWaitForPickup
};

const DecorateAnimation freezeTimePickupAnimation =
{
    { 0, 6, ActionFreezeTime },
    { 0, 6, ActionRemove }
};

const DecorateState freezeTimePickup =
{
    freezeTimePickupAnimation,
    StateIdPickup
};

const std::map<DecorateStateId, DecorateState> freezeTimeStates =
{
    std::make_pair(StateIdWaitForPickup, freezeTimeWaitForPickup),
    std::make_pair(StateIdPickup, freezeTimePickup),
    std::make_pair(StateIdDying, bonusStateDying)
};

const DecorateActor decorateFreezeTime =
{
    actorIdBonusFreezeTime,   // Id
    57, // spawnOnAllDifficulties;
    57, // spawnOnNormalAndHard;
    57,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    freezeTimeStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    BOOMSND, // hitSound;
    0,    // speed;
    0, // actionParameter
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
// GEM RED
//

const DecorateAnimation gemRedWaitForPickupAnimation =
{
    { RGEM1PIC, 30, ActionWaitForPickup }
};

const DecorateState gemRedWaitForPickup =
{
    gemRedWaitForPickupAnimation,
    StateIdWaitForPickup
};

const DecorateAnimation gemPickupAnimation =
{
    { 0, 6, ActionGiveGem },
    { 0, 6, ActionRemove }
};

const DecorateState gemPickup =
{
    gemPickupAnimation,
    StateIdPickup
};

const std::map<DecorateStateId, DecorateState>gemRedStates =
{
    std::make_pair(StateIdWaitForPickup, gemRedWaitForPickup),
    std::make_pair(StateIdPickup, gemPickup),
};

const DecorateActor decorateGemRed =
{
    actorIdBonusGemRed,   // Id
    40, // spawnOnAllDifficulties;
    40, // spawnOnNormalAndHard;
    40,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    gemRedStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    0, // actionParameter
    0 // ProjectileId
};

//
// GEM YELLOW
//

const DecorateAnimation gemYellowWaitForPickupAnimation =
{
    { YGEM1PIC, 30, ActionWaitForPickup }
};

const DecorateState gemYellowWaitForPickup =
{
    gemYellowWaitForPickupAnimation,
    StateIdWaitForPickup
};

const std::map<DecorateStateId, DecorateState>gemYellowStates =
{
    std::make_pair(StateIdWaitForPickup, gemYellowWaitForPickup),
    std::make_pair(StateIdPickup, gemPickup),
};

const DecorateActor decorateGemYellow =
{
    actorIdBonusGemYellow,   // Id
    43, // spawnOnAllDifficulties;
    43, // spawnOnNormalAndHard;
    43,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    gemYellowStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    3, // actionParameter
    0 // ProjectileId
};

//
// GEM GREEN
//

const DecorateAnimation gemGreenWaitForPickupAnimation =
{
    { GGEM1PIC, 30, ActionWaitForPickup }
};

const DecorateState gemGreenWaitForPickup =
{
    gemGreenWaitForPickupAnimation,
    StateIdWaitForPickup
};

const std::map<DecorateStateId, DecorateState>gemGreenStates =
{
    std::make_pair(StateIdWaitForPickup, gemGreenWaitForPickup),
    std::make_pair(StateIdPickup, gemPickup),
};

const DecorateActor decorateGemGreen =
{
    actorIdBonusGemGreen,   // Id
    41, // spawnOnAllDifficulties;
    41, // spawnOnNormalAndHard;
    41,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    gemGreenStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    1, // actionParameter
    0 // ProjectileId
};

//
// GEM BLUE
//

const DecorateAnimation gemBlueWaitForPickupAnimation =
{
    { BGEM1PIC, 30, ActionWaitForPickup }
};

const DecorateState gemBlueWaitForPickup =
{
    gemBlueWaitForPickupAnimation,
    StateIdWaitForPickup
};

const std::map<DecorateStateId, DecorateState>gemBlueStates =
{
    std::make_pair(StateIdWaitForPickup, gemBlueWaitForPickup),
    std::make_pair(StateIdPickup, gemPickup),
};

const DecorateActor decorateGemBlue =
{
    actorIdBonusGemBlue,   // Id
    42, // spawnOnAllDifficulties;
    42, // spawnOnNormalAndHard;
    42,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    gemBlueStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    2, // actionParameter
    0 // ProjectileId
};

//
// GEM PURPLE
//

const DecorateAnimation gemPurpleWaitForPickupAnimation =
{
    { PGEM1PIC, 30, ActionWaitForPickup }
};

const DecorateState gemPurpleWaitForPickup =
{
    gemPurpleWaitForPickupAnimation,
    StateIdWaitForPickup
};

const std::map<DecorateStateId, DecorateState>gemPurpleStates =
{
    std::make_pair(StateIdWaitForPickup, gemPurpleWaitForPickup),
    std::make_pair(StateIdPickup, gemPickup),
};

const DecorateActor decorateGemPurple =
{
    actorIdBonusGemPurple,   // Id
    44, // spawnOnAllDifficulties;
    44, // spawnOnNormalAndHard;
    44,  // spawnOnHard
    1,  // initialHealth;
    35 * pixelRadius,    // size;
    Never,  // radarVisibility;
    EgaBrightWhite,   // radarColor;
    gemPurpleStates,
    StateIdWaitForPickup,   // initialState;
    0,  // meleeDamage;
    0, // hitSound;
    0,    // speed;
    4, // actionParameter
    0 // ProjectileId
};
