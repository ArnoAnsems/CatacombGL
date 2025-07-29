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

#include "SavedGameConverterApocalypse_Test.h"
#include "../Apocalypse/SavedGameConverterApocalypse.h"
#include "../Apocalypse/DecorateAll.h"

void SavedGameConverterApocalypse_Test::CheckDosObjectIsConvertible(const SavedGameInDosFormat::ObjectInDosFormat& dosObject)
{

    SavedGameConverterApocalypse converter(m_farPointerOffset);
    const uint16_t actorId = converter.GetActorId(dosObject);
    const auto actorIt = decorateApocalypseAll.find(actorId);
    ASSERT_TRUE(actorIt != decorateApocalypseAll.end());

    const DecorateStateId stateId = converter.GetDecorateStateId(dosObject);
    const auto stateIt = actorIt->second.states.find(stateId);
    ASSERT_TRUE(stateIt != actorIt->second.states.end());

    const uint16_t animationFrame = converter.GetAnimationFrame(dosObject);
    EXPECT_LE(animationFrame, stateIt->second.animation.size() - 1);
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertBonus)
{
    constexpr uint16_t obclassBonus = 2;
    constexpr uint32_t allState32[] = {
        0x1A710000 + m_farPointerOffset, 0x1A720000 + m_farPointerOffset, 0x1A730000 + m_farPointerOffset,
        0x1A740000 + m_farPointerOffset, 0x1A750000 + m_farPointerOffset, 0x1A760000 + m_farPointerOffset,
        0x1A770000 + m_farPointerOffset, 0x1A780000 + m_farPointerOffset, 0x1A790000 + m_farPointerOffset,
        0x1A7A0000 + m_farPointerOffset, 0x1A7B0000 + m_farPointerOffset, 0x1A7C0000 + m_farPointerOffset,
        0x1A7D0000 + m_farPointerOffset, 0x1A7E0000 + m_farPointerOffset, 0x1A7F0000 + m_farPointerOffset,
        0x1A800000 + m_farPointerOffset, 0x1A810000 + m_farPointerOffset, 0x1A820000 + m_farPointerOffset,
        0x1A830000 + m_farPointerOffset, 0x1A840000 + m_farPointerOffset, 0x1A850000 + m_farPointerOffset,
        0x1A860000 + m_farPointerOffset, 0x1A870000 + m_farPointerOffset, 0x1A880000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBonus;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertAquaMan)
{
    constexpr uint16_t obclassAquaMan = 3;
    constexpr uint32_t allState32[] = {
        0x1A960000 + m_farPointerOffset, 0x1A970000 + m_farPointerOffset, 0x1A980000 + m_farPointerOffset,
        0x1A990000 + m_farPointerOffset, 0x1A9A0000 + m_farPointerOffset, 0x1A9B0000 + m_farPointerOffset,
        0x1A9C0000 + m_farPointerOffset, 0x1A9D0000 + m_farPointerOffset, 0x1A9E0000 + m_farPointerOffset,
        0x1A9F0000 + m_farPointerOffset, 0x1AA00000 + m_farPointerOffset, 0x1AA10000 + m_farPointerOffset,
        0x1AA20000 + m_farPointerOffset, 0x1AA30000 + m_farPointerOffset, 0x1AA40000 + m_farPointerOffset,
        0x1AA50000 + m_farPointerOffset, 0x1AA60000 + m_farPointerOffset, 0x1AA70000 + m_farPointerOffset,
        0x1AA80000 + m_farPointerOffset, 0x1AA90000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassAquaMan;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertWizard)
{
    constexpr uint16_t obclassWizard = 4;
    constexpr uint32_t allState32[] = {
        0x1AAA0000 + m_farPointerOffset, 0x1AAB0000 + m_farPointerOffset, 0x1AAC0000 + m_farPointerOffset,
        0x1AAD0000 + m_farPointerOffset, 0x1AAE0000 + m_farPointerOffset, 0x1AAF0000 + m_farPointerOffset,
        0x1AB00000 + m_farPointerOffset, 0x1AB10000 + m_farPointerOffset, 0x1AB20000 + m_farPointerOffset,
        0x1AB30000 + m_farPointerOffset, 0x1AB40000 + m_farPointerOffset, 0x1AB50000 + m_farPointerOffset,
        0x1AB60000 + m_farPointerOffset, 0x1AB70000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassWizard;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}