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
#include "DecorateAll.h"

SavedGameConverterApocalypse::SavedGameConverterApocalypse()
{
}

SavedGameConverterApocalypse::~SavedGameConverterApocalypse()
{
}

const uint16_t SavedGameConverterApocalypse::GetActorId(const SavedGameInDosFormat::ObjectInDosFormat& /*dosObject*/) const
{
    // TODO
    return 0;
}

const DecorateStateId SavedGameConverterApocalypse::GetDecorateStateId(const SavedGameInDosFormat::ObjectInDosFormat& /*dosObject*/) const
{
    // TODO
    return StateIdWalk;
}
const uint16_t SavedGameConverterApocalypse::GetAnimationFrame(const SavedGameInDosFormat::ObjectInDosFormat& /*dosObject*/) const
{
    // TODO
    return 0;
}

const bool SavedGameConverterApocalypse::IsInertObject(const uint16_t obclass) const
{
    // TODO
    return false;
}