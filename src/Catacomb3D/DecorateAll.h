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

#include "DecorateMisc.h"
#include "DecorateProjectiles.h"
#include "DecorateBonus.h"

const std::map<uint16_t, const DecorateActor> decorateCatacomb3DAll =
{
    std::make_pair(actorIdExplodingWall, decorateExplodingWall),
    std::make_pair(actorIdPlayer, decoratePlayer),
    std::make_pair(actorIdBonusExplosion, decorateExplosion),
    std::make_pair(actorIdBonusBolt, decorateBolt),
    std::make_pair(actorIdBonusNuke, decorateNuke),
    std::make_pair(actorIdBonusPotion, decoratePotion),
    std::make_pair(actorIdBonusChest, decorateChest),
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
    std::make_pair(actorIdProjectilePlayerShot, decoratePlayerShot),
};



