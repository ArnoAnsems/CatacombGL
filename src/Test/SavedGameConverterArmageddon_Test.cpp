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

#include "SavedGameConverterArmageddon_Test.h"
#include "../Armageddon/SavedGameConverterArmageddon.h"
#include "../Armageddon/DecorateAll.h"

void SavedGameConverterArmageddon_Test::CheckDosObjectIsConvertible(const SavedGameInDosFormat::ObjectInDosFormat& dosObject)
{

    SavedGameConverterArmageddon converter(m_farPointerOffset);
    const uint16_t actorId = converter.GetActorId(dosObject);
    const auto actorIt = decorateArmageddonAll.find(actorId);
    ASSERT_TRUE(actorIt != decorateArmageddonAll.end());

    const DecorateStateId stateId = converter.GetDecorateStateId(dosObject);
    const auto stateIt = actorIt->second.states.find(stateId);
    ASSERT_TRUE(stateIt != actorIt->second.states.end());

    const uint16_t animationFrame = converter.GetAnimationFrame(dosObject);
    EXPECT_LE(animationFrame, stateIt->second.animation.size() - 1);
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertBonus)
{
    constexpr uint16_t obclassBonus = 2;
    constexpr uint32_t allState32[] = {
        0x1A5A0000 + m_farPointerOffset, 0x1A5B0000 + m_farPointerOffset, 0x1A5C0000 + m_farPointerOffset,
        0x1A5D0000 + m_farPointerOffset, 0x1A5E0000 + m_farPointerOffset, 0x1A5F0000 + m_farPointerOffset,
        0x1A600000 + m_farPointerOffset, 0x1A610000 + m_farPointerOffset, 0x1A620000 + m_farPointerOffset,
        0x1A630000 + m_farPointerOffset, 0x1A640000 + m_farPointerOffset, 0x1A650000 + m_farPointerOffset,
        0x1A660000 + m_farPointerOffset, 0x1A670000 + m_farPointerOffset, 0x1A680000 + m_farPointerOffset,
        0x1A690000 + m_farPointerOffset, 0x1A6A0000 + m_farPointerOffset, 0x1A6B0000 + m_farPointerOffset,
        0x1A6C0000 + m_farPointerOffset, 0x1A6D0000 + m_farPointerOffset, 0x1A6E0000 + m_farPointerOffset,
        0x1A6F0000 + m_farPointerOffset, 0x1A700000 + m_farPointerOffset, 0x1A710000 + m_farPointerOffset,
        0x1A720000 + m_farPointerOffset, 0x1A760000 + m_farPointerOffset, 0x1A770000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBonus;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}
