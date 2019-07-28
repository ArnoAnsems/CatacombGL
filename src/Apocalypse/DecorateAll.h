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
#pragma once

#include "DecorateProjectiles.h"
#include "DecorateMisc.h"
#include "DecorateBonus.h"
#include "DecorateMonsters.h"

const std::map<uint16_t, const DecorateActor> decorateApocalypseAll =
{
    // Misc
    std::make_pair(actorIdExplodingWall, decorateExplodingWall),
    std::make_pair(actorIdPortal, decoratePortal),
    std::make_pair(actorIdPlayer, decoratePlayer),
    std::make_pair(actorIdColumn1, decorateColumn1),
    std::make_pair(actorIdColumn2, decorateColumn2),
    std::make_pair(actorIdColumn3, decorateColumn3),
    std::make_pair(actorIdColumn4, decorateColumn4),
    std::make_pair(actorIdColumn5, decorateColumn5),
    std::make_pair(actorIdTomb1, decorateTomb1),
    std::make_pair(actorIdTomb2, decorateTomb2),
    std::make_pair(actorIdFutureFirePot, decorateFutureFirePot),
    std::make_pair(actorIdOldFirePot, decorateOldFirePot),
    std::make_pair(actorIdFakeWall, decorateFakeWall),
    std::make_pair(actorIdForceField, decorateForceField),

    // Bonus
    std::make_pair(actorIdBonusExplosion, decorateExplosion),
    std::make_pair(actorIdBonusBolt, decorateBolt),
    std::make_pair(actorIdBonusNuke, decorateNuke),
    std::make_pair(actorIdBonusPotion, decoratePotion),
    std::make_pair(actorIdBonusChest, decorateChest),
    std::make_pair(actorIdBonusWaterChest, decorateWaterChest),
    std::make_pair(actorIdBonusFreezeTime, decorateFreezeTime),
    std::make_pair(actorIdBonusOldChest, decorateOldChest),
    std::make_pair(actorIdBonusKeyRed, decorateKeyRed),
    std::make_pair(actorIdBonusKeyYellow, decorateKeyYellow),
    std::make_pair(actorIdBonusKeyGreen, decorateKeyGreen),
    std::make_pair(actorIdBonusKeyBlue, decorateKeyBlue),
    std::make_pair(actorIdBonusGemRed, decorateGemRed),
    std::make_pair(actorIdBonusGemYellow, decorateGemYellow),
    std::make_pair(actorIdBonusGemGreen, decorateGemGreen),
    std::make_pair(actorIdBonusGemBlue, decorateGemBlue),
    std::make_pair(actorIdBonusGemPurple, decorateGemPurple),

    // Monsters
    std::make_pair(actorIdMonsterNemesis, decorateNemesis),
    std::make_pair(actorIdMonsterBounce, decorateBounce),
    std::make_pair(actorIdMonsterBlob, decorateBlob),
    std::make_pair(actorIdMonsterWizard, decorateWizard),
    std::make_pair(actorIdMonsterBug, decorateBug),
    std::make_pair(actorIdMonsterTimeLord, decorateTimeLord),
    std::make_pair(actorIdMonsterAndroidMage, decorateAndroidMage),
    std::make_pair(actorIdMonsterCyborgDemon, decorateCyborgDemon),
    std::make_pair(actorIdMonsterStompy, decorateStompy),
    std::make_pair(actorIdMonsterRunningEye, decorateRunningEye),
    std::make_pair(actorIdMonsterDemon, decorateDemon),
    std::make_pair(actorIdMonsterAquaMan, decorateAquaMan),
    std::make_pair(actorIdMonsterTroll, decorateTroll),
    std::make_pair(actorIdMonsterRay, decorateRay),
    std::make_pair(actorIdMonsterSkeleton, decorateSkeleton),
    std::make_pair(actorIdMonsterRoboTank, decorateRoboTank),
    std::make_pair(actorIdMonsterShooterEye, decorateShooterEye),
    std::make_pair(actorIdMonsterInvisDude, decorateInvisDude),

    // Projectiles
    std::make_pair(actorIdProjectilePlayerShot, decoratePlayerShot),
    std::make_pair(actorIdProjectilePlayerBigShot, decoratePlayerBigShot),
    std::make_pair(actorIdProjectileWizardShot, decorateWizardShot),
    std::make_pair(actorIdProjectileBlobShot, decorateBlobShot),
    std::make_pair(actorIdProjectileBugShot, decorateBugShot),
    std::make_pair(actorIdProjectileTimeLordShot, decorateTimeLordShot),
    std::make_pair(actorIdProjectileAndroidMageShot, decorateAndroidMageShot),
    std::make_pair(actorIdProjectileStompyShot, decorateStompyShot),
    std::make_pair(actorIdProjectileRayShot, decorateRayShot),
    std::make_pair(actorIdProjectileSkeletonShot, decorateSkeletonShot),
    std::make_pair(actorIdProjectileRoboTankShot, decorateRoboTankShot),
    std::make_pair(actorIdProjectileEyeShot, decorateEyeShot),
    std::make_pair(actorIdProjectileNemesisShot, decorateNemesisShot),
};
