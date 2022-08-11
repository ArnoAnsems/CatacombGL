// Copyright (C) 2022 Arno Ansems
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

#include "SavedGameConverterCatacomb3D.h"
#include "DecorateAll.h"

static const DosFormatConfig dosFormatConfigCatacomb3D =
{
    "", // gameName
    "", // saveVersion
    {
        HeaderItemSignature, HeaderItemOldTest, HeaderItemPresent, HeaderItemName,
        HeaderItemDifficulty, HeaderItemMapOn, HeaderItemBolts, HeaderItemNukes, HeaderItemPotions,
        HeaderItemKeys, HeaderItemScrolls, HeaderItemScore, HeaderItemBody, HeaderItemShotpower
    },   // headerItems
    {
        ObjectActive, ObjectTiccount, ObjectObclass, ObjectState16, ObjectShootable, ObjectTileObject,
        ObjectDistance, ObjectDir, ObjectX, ObjectY, ObjectTileX, ObjectTileY, ObjectViewX, ObjectViewHeight,
        ObjectAngle, ObjectHitpoints, ObjectSpeed, ObjectSize, ObjectXL, ObjectXH, ObjectYL, ObjectYH,
        ObjectTemp1, ObjectTemp2, ObjectNext, ObjectPrev
    }   // objectItems
};

SavedGameConverterCatacomb3D::SavedGameConverterCatacomb3D()
{
}

SavedGameConverterCatacomb3D::~SavedGameConverterCatacomb3D()
{
}

const DosFormatConfig& SavedGameConverterCatacomb3D::GetDosFormatConfig() const
{
    return dosFormatConfigCatacomb3D;
}

const uint16_t SavedGameConverterCatacomb3D::GetActorId(const uint16_t obclass, const uint16_t state16, const uint32_t state32) const
{
    uint16_t actorId = 0;
    switch (obclass)
    {
    case 1:
        actorId = actorIdPlayer;
        break;
    case 2:
        // Bonus
        break;
    case 3:
        actorId = actorIdMonsterOrc;
        break;
    case 4:
        actorId = actorIdMonsterBat;
        break;
    case 5:
        // Skeleton
        break;
    case 6:
        actorId = actorIdMonsterTroll;
        break;
    case 7:
        actorId = actorIdMonsterDemon;
        break;
    case 8:
        actorId = actorIdMonsterMage;
        break;
    case 9:
        actorId = actorIdProjectilePlayerShot;
        break;
    case 10:
        actorId = actorIdProjectilePlayerBigShot;
        break;
    case 11:
        actorId = actorIdProjectileMageShot;
        break;
    case 12:
        // inertobj
        break;
    case 13:
        actorId = actorIdMonsterBounce;
        break;
    case 14:
        actorId = actorIdMonsterNemesis;
        break;
    case 15:
        // gateobj
        break;
    }
    return actorId;
}