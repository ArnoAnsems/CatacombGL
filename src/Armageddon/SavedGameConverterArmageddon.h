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
// SavedGameConverterArmageddon
//
// Conversion of saved games from DOS format to CatacombGL; specifically for Catacomb Armageddon
//
#pragma once

#include "../Engine/ISavedGameConverter.h"

class SavedGameConverterArmageddon : public ISavedGameConverter
{
public:
    SavedGameConverterArmageddon();
    ~SavedGameConverterArmageddon();
    const uint16_t GetActorIdOfGate(const uint32_t state32, const int16_t temp1) const;
    const uint16_t GetActorId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const override;
    const DecorateStateId GetDecorateStateId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const override;
    const uint16_t GetAnimationFrame(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const override;
    const bool IsInertObject(const uint16_t obclass) const override;

private:
    const uint16_t GetActorIdOfBonus(const uint32_t state32) const;
    const uint16_t GetActorIdOfSolid(const uint32_t state32) const;
    const uint16_t GetActorIdOfRealSolid(const uint32_t state32) const;
    const uint16_t GetActorIdOfExplosion(const uint32_t state32) const;
    const uint16_t GetActorIdOfInert(const uint32_t state32) const;
};




