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

#include "SavedGameConverterAbyss.h"

SavedGameConverterAbyss::SavedGameConverterAbyss()
{
}

SavedGameConverterAbyss::~SavedGameConverterAbyss()
{
}

const uint16_t SavedGameConverterAbyss::GetActorId(const SavedGameInDosFormat::ObjectInDosFormat& /*dosObject*/) const
{
    // TODO
    return 0;
}

const bool SavedGameConverterAbyss::IsInertObject(const uint16_t obclass) const
{
    // TODO
    return false;
}