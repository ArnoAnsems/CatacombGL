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

#include "SavedGameConverterApocalypse.h"

static const DosFormatConfig dosFormatConfigApocalypse =
{
    "CATACOMB  APOCALYPSE 3-D", // gameName
    "0.01", // saveVersion
    {
        HeaderItemFreezeTime, HeaderItemDifficulty, HeaderItemMapOn, HeaderItemBolts, HeaderItemNukes, HeaderItemPotions,
        HeaderItemKeys, HeaderItemScrolls, HeaderItemGems, HeaderItemScore, HeaderItemBody, HeaderItemShotpower,
        HeaderItemEasyModeOn
    }   // headerItems
};

SavedGameConverterApocalypse::SavedGameConverterApocalypse()
{
}

SavedGameConverterApocalypse::~SavedGameConverterApocalypse()
{
}

const DosFormatConfig& SavedGameConverterApocalypse::GetDosFormatConfig() const
{
    return dosFormatConfigApocalypse;
}