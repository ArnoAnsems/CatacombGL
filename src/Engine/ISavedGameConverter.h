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

//
// ISavedGameConverter
//
// Interface for abstracting conversion of saved games from DOS format to CatacombGL format.
//
#pragma once

#include <stdint.h>
#include "SavedGameInDosFormat.h"
#include "Decorate.h"

class ISavedGameConverter
{
public:
    virtual ~ISavedGameConverter() {};
    virtual const uint16_t GetActorId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const = 0;
    virtual const DecorateStateId GetDecorateStateId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const = 0;
    virtual const uint16_t GetAnimationFrame(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const = 0;
    virtual const bool IsInertObject(const uint16_t obclass) const = 0;
    virtual void SetFarPointerOffset(const uint32_t playerState32) = 0;
};
