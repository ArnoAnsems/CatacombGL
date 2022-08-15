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
// SavedGameConverterApocalypse
//
// Conversion of saved games from DOS format to CatacombGL; specifically for Catacomb Apocalypse
//
#pragma once

#include "../Engine/ISavedGameConverter.h"

class SavedGameConverterApocalypse : public ISavedGameConverter
{
public:
    SavedGameConverterApocalypse();
    ~SavedGameConverterApocalypse();
    const DosFormatConfig& GetDosFormatConfig() const override;
    const uint16_t GetActorId(
        const uint16_t obclass,
        const uint16_t state16,
        const uint32_t state32,
        const int16_t temp1) const override;
};



