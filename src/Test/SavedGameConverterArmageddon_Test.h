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

#include <gtest/gtest.h>
#include "../Engine/SavedGameInDosFormat.h"

class SavedGameConverterArmageddon_Test : public ::testing::Test
{
public:
    SavedGameConverterArmageddon_Test() = default;
    virtual ~SavedGameConverterArmageddon_Test() = default;

    void CheckDosObjectIsConvertible(const SavedGameInDosFormat::ObjectInDosFormat& dosObject);

    static constexpr uint32_t m_farPointerOffset = 0x1A20000;
};