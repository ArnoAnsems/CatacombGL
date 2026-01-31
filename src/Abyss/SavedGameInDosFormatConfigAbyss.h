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
#pragma once

#include "../Engine/SavedGameInDosFormatConfig.h"

static const SavedGameInDosFormatConfig savedGameInDosFormatConfigAbyssV112 =
{
    "CATACOMB ABYSS 3-D", // gameName
    "0.93", // saveVersion
    {
        HeaderItemFreezeTime, HeaderItemBolts, HeaderItemNukes, HeaderItemPotions,
        HeaderItemKeys, HeaderItemScrolls, HeaderItemGems, HeaderItemScore, HeaderItemBody, HeaderItemShotpower
    },   // headerItems
    {
        ObjectActive, ObjectTiccount, ObjectObclass, ObjectState16, ObjectFlags,
        ObjectDistance, ObjectDir, ObjectX, ObjectY, ObjectTileX, ObjectTileY, ObjectViewX, ObjectViewHeight,
        ObjectAngle, ObjectHitpoints, ObjectSpeed, ObjectSize, ObjectXL, ObjectXH, ObjectYL, ObjectYH,
        ObjectTemp1, ObjectTemp2, ObjectNext, ObjectPrev
    }   // objectItems
};

static const SavedGameInDosFormatConfig savedGameInDosFormatConfigAbyss =
{
    "CATACOMB ABYSS 3-D", // gameName
    "0.93", // saveVersion
    {
        HeaderItemFreezeTime, HeaderItemDifficulty, HeaderItemMapOn, HeaderItemBolts, HeaderItemNukes, HeaderItemPotions,
        HeaderItemKeys, HeaderItemScrolls, HeaderItemGems, HeaderItemScore, HeaderItemBody, HeaderItemShotpower,
        HeaderItemEasyModeOn
    },   // headerItems
    {
        ObjectActive, ObjectTiccount, ObjectObclass, ObjectState16, ObjectFlags,
        ObjectDistance, ObjectDir, ObjectX, ObjectY, ObjectTileX, ObjectTileY, ObjectViewX, ObjectViewHeight,
        ObjectAngle, ObjectHitpoints, ObjectSpeed, ObjectSize, ObjectXL, ObjectXH, ObjectYL, ObjectYH,
        ObjectTemp1, ObjectTemp2, ObjectNext, ObjectPrev
    }   // objectItems
};
