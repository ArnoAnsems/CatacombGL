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
// SavedGameConverterCatacomb3D
//
// Conversion of saved games from DOS format to CatacombGL; specifically for Catacomb 3-D
//
#pragma once

#include "../Engine/ISavedGameConverter.h"
#include "../Engine/GameId.h"

class SavedGameConverterCatacomb3D : public ISavedGameConverter
{
public:
    SavedGameConverterCatacomb3D(const GameId gameId);
    ~SavedGameConverterCatacomb3D();
    const uint16_t GetActorId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const override;
    const DecorateStateId GetDecorateStateId(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const override;
    const uint16_t GetAnimationFrame(const SavedGameInDosFormat::ObjectInDosFormat& dosObject) const override;
    const bool IsInertObject(const uint16_t obclass) const override;
    void SetFarPointerOffset(const uint32_t playerState32) override;
private:
    const uint16_t GetActorIdOfBonus(const uint16_t state16, const int16_t temp1) const;
    const uint16_t GetActorIdOfGate(const uint16_t state16, const int16_t temp1) const;
    const uint16_t GetActorIdOfInert(const uint16_t state16) const;
    const uint8_t GetGameIndex() const;

    const GameId m_gameId;
};

