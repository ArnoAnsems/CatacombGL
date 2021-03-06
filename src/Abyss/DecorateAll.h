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
#pragma once

#include "DecorateMonsters.h"
#include "DecorateProjectiles.h"
#include "DecorateBonus.h"
#include "DecorateMisc.h"

const std::map<uint16_t, const DecorateActor> decorateAbyssAll =
{
    std::make_pair(actorIdTomb1, decorateTomb1),
    std::make_pair(actorIdTomb2, decorateTomb2),
    std::make_pair(actorIdTomb3, decorateTomb3),
    std::make_pair(actorIdExplodingWall, decorateExplodingWall),
    std::make_pair(actorIdWarpPit, decorateWarpPit),
    std::make_pair(actorIdWarpPortal1, decorateWarpPortal1),
    std::make_pair(actorIdWarpPortal2, decorateWarpPortal2),
    std::make_pair(actorIdWarpPortal3, decorateWarpPortal3),
    std::make_pair(actorIdWarpPortal4, decorateWarpPortal4),
    std::make_pair(actorIdWarpPortal5, decorateWarpPortal5),
    std::make_pair(actorIdPlayer, decoratePlayer),
    std::make_pair(actorIdBonusExplosion, decorateExplosion),
    std::make_pair(actorIdBonusBolt, decorateBolt),
    std::make_pair(actorIdBonusNuke, decorateNuke),
    std::make_pair(actorIdBonusPotion, decoratePotion),
    std::make_pair(actorIdBonusChest, decorateChest),
    std::make_pair(actorIdBonusWaterChest, decorateWaterChest),
    std::make_pair(actorIdBonusFreezeTime, decorateFreezeTime),
    std::make_pair(actorIdBonusScroll1, decorateScroll1),
    std::make_pair(actorIdBonusScroll2, decorateScroll2),
    std::make_pair(actorIdBonusScroll3, decorateScroll3),
    std::make_pair(actorIdBonusScroll4, decorateScroll4),
    std::make_pair(actorIdBonusScroll5, decorateScroll5),
    std::make_pair(actorIdBonusScroll6, decorateScroll6),
    std::make_pair(actorIdBonusScroll7, decorateScroll7),
    std::make_pair(actorIdBonusScroll8, decorateScroll8),
    std::make_pair(actorIdBonusKeyRed, decorateKeyRed),
    std::make_pair(actorIdBonusKeyYellow, decorateKeyYellow),
    std::make_pair(actorIdBonusKeyGreen, decorateKeyGreen),
    std::make_pair(actorIdBonusKeyBlue, decorateKeyBlue),
    std::make_pair(actorIdBonusKeyRed2, decorateKeyRed2),
    std::make_pair(actorIdBonusGemRed, decorateGemRed),
    std::make_pair(actorIdBonusGemYellow, decorateGemYellow),
    std::make_pair(actorIdBonusGemGreen, decorateGemGreen),
    std::make_pair(actorIdBonusGemBlue, decorateGemBlue),
    std::make_pair(actorIdBonusGemPurple, decorateGemPurple),
    std::make_pair(actorIdMonsterZombie, decorateZombie),
    std::make_pair(actorIdMonsterSkeleton, decorateSkeleton),
    std::make_pair(actorIdMonsterWallSkeleton, decorateWallSkeleton),
    std::make_pair(actorIdMonsterOrc, decorateOrc),
    std::make_pair(actorIdMonsterTroll, decorateTroll),
    std::make_pair(actorIdMonsterDemon, decorateDemon),
    std::make_pair(actorIdMonsterRedDemon, decorateRedDemon),
    std::make_pair(actorIdMonsterSpook, decorateSpook),
    std::make_pair(actorIdMonsterBat, decorateBat),
    std::make_pair(actorIdMonsterMage, decorateMage),
    std::make_pair(actorIdMonsterEye, decorateEye),
    std::make_pair(actorIdMonsterWetMan, decorateWetMan),
    std::make_pair(actorIdMonsterNemesis, decorateNemesis),
    std::make_pair(actorIdProjectilePlayerShot, decoratePlayerShot),
    std::make_pair(actorIdProjectilePlayerBigShot, decoratePlayerBigShot),
    std::make_pair(actorIdProjectileMageShot, decorateMageShot),
    std::make_pair(actorIdProjectileEyeShot, decorateEyeShot),
    std::make_pair(actorIdProjectileNemesisShot, decorateNemesisShot)
};

