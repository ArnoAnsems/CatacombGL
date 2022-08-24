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
// SavedGameConverterAbyss
//
// Conversion of saved games from DOS format to CatacombGL; specifically for Catacomb Abyss
//
#pragma once

#include "../Engine/ISavedGameConverter.h"

class SavedGameConverterAbyss : public ISavedGameConverter
{
public:
    SavedGameConverterAbyss(const uint8_t gameId);
    ~SavedGameConverterAbyss();
    const uint16_t GetActorId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const override;
    const DecorateStateId GetDecorateStateId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const override;
    const uint16_t GetAnimationFrame(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const override;
    const bool IsInertObject(const uint16_t obclass) const override;

private:
    const uint16_t GetActorIdOfBonus(const uint16_t state16, const int16_t temp1) const;
    const uint16_t GetActorIdOfSolid(const uint16_t state16) const;
    const uint16_t GetActorIdOfGate(const uint16_t state16, const int16_t temp1) const;
    const uint8_t GetGameIndex() const;

    const uint8_t m_gameId;
};


