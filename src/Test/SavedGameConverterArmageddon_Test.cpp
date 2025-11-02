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
    SavedGameConverterArmageddon converter;
    converter.SetFarPointerOffset(m_farPointerOffset + 0x1A580000);
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

TEST_F(SavedGameConverterArmageddon_Test, ConvertSuccubus)
{
    constexpr uint16_t obClassSuccubus = 3;
    constexpr uint32_t allState32[] = {
        0x1ACB0000 + m_farPointerOffset, 0x1ACC0000 + m_farPointerOffset, 0x1ACD0000 + m_farPointerOffset,
        0x1ACE0000 + m_farPointerOffset, 0x1ACF0000 + m_farPointerOffset, 0x1AD00000 + m_farPointerOffset,
        0x1AD10000 + m_farPointerOffset, 0x1AD20000 + m_farPointerOffset, 0x1AD30000 + m_farPointerOffset,
        0x1AD40000 + m_farPointerOffset, 0x1AD50000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassSuccubus;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertBat)
{
    constexpr uint16_t obClassBat = 4;
    constexpr uint32_t allState32[] = {
        0x1B0B0000 + m_farPointerOffset, 0x1B0C0000 + m_farPointerOffset, 0x1B0D0000 + m_farPointerOffset,
        0x1B0E0000 + m_farPointerOffset, 0x1B0F0000 + m_farPointerOffset, 0x1B100000 + m_farPointerOffset,
        0x1B110000 + m_farPointerOffset

    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassBat;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertSkeleton)
{
    constexpr uint16_t obClassSkeleton = 5;
    constexpr uint32_t allState32[] = {
        0x1AB10000 + m_farPointerOffset, 0x1AB20000 + m_farPointerOffset, 0x1AB30000 + m_farPointerOffset,
        0x1AB40000 + m_farPointerOffset, 0x1AB50000 + m_farPointerOffset, 0x1AB60000 + m_farPointerOffset,
        0x1AB70000 + m_farPointerOffset, 0x1AB80000 + m_farPointerOffset, 0x1AB90000 + m_farPointerOffset,
        0x1ABA0000 + m_farPointerOffset, 0x1ABB0000 + m_farPointerOffset, 0x1ABC0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassSkeleton;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertFatDemon)
{
    constexpr uint16_t obClassFatDemon = 6;
    constexpr uint32_t allState32[] = {
        0x1A830000 + m_farPointerOffset, 0x1A840000 + m_farPointerOffset, 0x1A850000 + m_farPointerOffset,
        0x1A860000 + m_farPointerOffset, 0x1A870000 + m_farPointerOffset, 0x1A880000 + m_farPointerOffset,
        0x1A890000 + m_farPointerOffset, 0x1A8A0000 + m_farPointerOffset, 0x1A8B0000 + m_farPointerOffset,
        0x1A8C0000 + m_farPointerOffset, 0x1A8D0000 + m_farPointerOffset, 0x1A8E0000 + m_farPointerOffset,
        0x1A8F0000 + m_farPointerOffset, 0x1A900000 + m_farPointerOffset, 0x1A910000 + m_farPointerOffset,
        0x1A920000 + m_farPointerOffset, 0x1A930000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassFatDemon;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertGodess)
{
    constexpr uint16_t obClassGodess = 7;
    constexpr uint32_t allState32[] = {
        0x1B120000 + m_farPointerOffset, 0x1B130000 + m_farPointerOffset, 0x1B140000 + m_farPointerOffset,
        0x1B150000 + m_farPointerOffset, 0x1B160000 + m_farPointerOffset, 0x1B170000 + m_farPointerOffset,
        0x1B180000 + m_farPointerOffset, 0x1B190000 + m_farPointerOffset, 0x1B1A0000 + m_farPointerOffset,
        0x1B1B0000 + m_farPointerOffset, 0x1B1C0000 + m_farPointerOffset, 0x1B1D0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassGodess;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertMage)
{
    constexpr uint16_t obClassMage = 8;
    constexpr uint32_t allState32[] = {
        0x1AD70000 + m_farPointerOffset, 0x1AD80000 + m_farPointerOffset, 0x1AD90000 + m_farPointerOffset,
        0x1ADA0000 + m_farPointerOffset, 0x1ADB0000 + m_farPointerOffset,
        0x1ADC0000 + m_farPointerOffset, 0x1ADD0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassMage;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertPlayerShot)
{
    constexpr uint16_t obClassPlayerShot = 9;
    constexpr uint32_t allState32[] = {
        0x1A530000 + m_farPointerOffset, 0x1A540000 + m_farPointerOffset, 0x1A550000 + m_farPointerOffset,
        0x1A560000 + m_farPointerOffset, 0x1A570000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassPlayerShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertPlayerBigShot)
{
    constexpr uint16_t obClassPlayerBigShot = 10;
    constexpr uint32_t allState32[] = {
        0x1A530000 + m_farPointerOffset, 0x1A540000 + m_farPointerOffset, 0x1A550000 + m_farPointerOffset,
        0x1A560000 + m_farPointerOffset, 0x1A570000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassPlayerBigShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertMageShot)
{
    constexpr uint16_t obClassMageShot = 11;
    constexpr uint32_t allState32[] = {
        0x1ADE0000 + m_farPointerOffset, 0x1ADF0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassMageShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertInert)
{
    constexpr uint16_t obClassInert = 12;
    constexpr uint32_t allState32[] = {
        0x1A550000 + m_farPointerOffset, 0x1A560000 + m_farPointerOffset, 0x1A570000 + m_farPointerOffset,
        0x1A720000 + m_farPointerOffset, 0x1A780000 + m_farPointerOffset, 0x1A790000 + m_farPointerOffset,
        0x1A7A0000 + m_farPointerOffset, 0x1A7B0000 + m_farPointerOffset, 0x1A7C0000 + m_farPointerOffset,
        0x1A7D0000 + m_farPointerOffset, 0x1A8B0000 + m_farPointerOffset, 0x1A8C0000 + m_farPointerOffset,
        0x1A8D0000 + m_farPointerOffset, 0x1A8E0000 + m_farPointerOffset, 0x1A8F0000 + m_farPointerOffset,
        0x1A900000 + m_farPointerOffset, 0x1A910000 + m_farPointerOffset, 0x1A920000 + m_farPointerOffset,
        0x1A930000 + m_farPointerOffset, 0x1AA80000 + m_farPointerOffset, 0x1AA90000 + m_farPointerOffset,
        0x1AAA0000 + m_farPointerOffset, 0x1AAB0000 + m_farPointerOffset, 0x1AAC0000 + m_farPointerOffset,
        0x1ABA0000 + m_farPointerOffset, 0x1ABB0000 + m_farPointerOffset, 0x1ABC0000 + m_farPointerOffset,
        0x1AC60000 + m_farPointerOffset, 0x1AC70000 + m_farPointerOffset, 0x1AC80000 + m_farPointerOffset,
        0x1AD40000 + m_farPointerOffset, 0x1AD50000 + m_farPointerOffset, 0x1ADC0000 + m_farPointerOffset,
        0x1ADD0000 + m_farPointerOffset, 0x1AEE0000 + m_farPointerOffset, 0x1AEF0000 + m_farPointerOffset,
        0x1AF00000 + m_farPointerOffset, 0x1AFA0000 + m_farPointerOffset, 0x1AFB0000 + m_farPointerOffset,
        0x1AFC0000 + m_farPointerOffset, 0x1AFD0000 + m_farPointerOffset, 0x1B030000 + m_farPointerOffset,
        0x1B040000 + m_farPointerOffset, 0x1B050000 + m_farPointerOffset, 0x1B060000 + m_farPointerOffset,
        0x1B070000 + m_farPointerOffset, 0x1B080000 + m_farPointerOffset, 0x1B090000 + m_farPointerOffset,
        0x1B100000 + m_farPointerOffset, 0x1B110000 + m_farPointerOffset, 0x1B1C0000 + m_farPointerOffset,
        0x1B1D0000 + m_farPointerOffset, 0x1B260000 + m_farPointerOffset, 0x1B270000 + m_farPointerOffset,
        0x1B280000 + m_farPointerOffset, 0x1B330000 + m_farPointerOffset, 0x1B340000 + m_farPointerOffset,
        0x1B350000 + m_farPointerOffset, 0x1B3E0000 + m_farPointerOffset, 0x1B3F0000 + m_farPointerOffset,
        0x1B400000 + m_farPointerOffset, 0x1B410000 + m_farPointerOffset, 0x1B420000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassInert;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertNemesis)
{
    constexpr uint16_t obClassNemesis = 14;
    constexpr uint32_t allState32[] = {
        0x1AFE0000 + m_farPointerOffset, 0x1AFF0000 + m_farPointerOffset, 0x1B000000 + m_farPointerOffset,
        0x1B010000 + m_farPointerOffset, 0x1B020000 + m_farPointerOffset, 0x1B030000 + m_farPointerOffset,
        0x1B040000 + m_farPointerOffset, 0x1B050000 + m_farPointerOffset, 0x1B060000 + m_farPointerOffset,
        0x1B070000 + m_farPointerOffset, 0x1B080000 + m_farPointerOffset, 0x1B090000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassNemesis;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertSuccubusShot)
{
    constexpr uint16_t obClassSuccubusShot = 15;
    SavedGameInDosFormat::ObjectInDosFormat dosObject;
    dosObject.obclass = obClassSuccubusShot;
    dosObject.state32 = 0x1AD60000 + m_farPointerOffset;
    CheckDosObjectIsConvertible(dosObject);
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertGate)
{
    constexpr uint16_t obClassGate = 16;
    constexpr uint32_t allState32[] = {
        0x1A7E0000 + m_farPointerOffset, 0x1A7F0000 + m_farPointerOffset, 0x1A800000 + m_farPointerOffset,
        0x1A810000 + m_farPointerOffset, 0x1A820000 + m_farPointerOffset,
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassGate;
        dosObject.state32 = state32;
        dosObject.temp1 = 1;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertZombie)
{
    constexpr uint16_t obClassZombie = 17;
    constexpr uint32_t allState32[] = {
        0x1B290000 + m_farPointerOffset, 0x1B2A0000 + m_farPointerOffset, 0x1B2B0000 + m_farPointerOffset,
        0x1B2C0000 + m_farPointerOffset, 0x1B2D0000 + m_farPointerOffset, 0x1B2E0000 + m_farPointerOffset,
        0x1B2F0000 + m_farPointerOffset, 0x1B300000 + m_farPointerOffset, 0x1B310000 + m_farPointerOffset,
        0x1B320000 + m_farPointerOffset, 0x1B330000 + m_farPointerOffset, 0x1B340000 + m_farPointerOffset,
        0x1B350000 + m_farPointerOffset, 0x1B360000 + m_farPointerOffset, 0x1B370000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassZombie;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertAnt)
{
    constexpr uint16_t obClassAnt = 18;
    constexpr uint32_t allState32[] = {
        0x1B1E0000 + m_farPointerOffset, 0x1B1F0000 + m_farPointerOffset, 0x1B200000 + m_farPointerOffset,
        0x1B210000 + m_farPointerOffset, 0x1B220000 + m_farPointerOffset, 0x1B230000 + m_farPointerOffset,
        0x1B240000 + m_farPointerOffset, 0x1B250000 + m_farPointerOffset, 0x1B260000 + m_farPointerOffset,
        0x1B270000 + m_farPointerOffset, 0x1B280000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassAnt;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertDragon)
{
    constexpr uint16_t obClassDragon = 19;
    constexpr uint32_t allState32[] = {
        0x1A940000 + m_farPointerOffset, 0x1A950000 + m_farPointerOffset, 0x1A960000 + m_farPointerOffset,
        0x1A970000 + m_farPointerOffset, 0x1A980000 + m_farPointerOffset, 0x1A990000 + m_farPointerOffset,
        0x1A9A0000 + m_farPointerOffset, 0x1A9B0000 + m_farPointerOffset, 0x1A9C0000 + m_farPointerOffset,
        0x1A9D0000 + m_farPointerOffset, 0x1A9E0000 + m_farPointerOffset, 0x1A9F0000 + m_farPointerOffset,
        0x1AA00000 + m_farPointerOffset, 0x1AA10000 + m_farPointerOffset, 0x1AA20000 + m_farPointerOffset,
        0x1AA30000 + m_farPointerOffset, 0x1AA40000 + m_farPointerOffset, 0x1AA50000 + m_farPointerOffset,
        0x1AA60000 + m_farPointerOffset, 0x1AA70000 + m_farPointerOffset, 0x1AA80000 + m_farPointerOffset,
        0x1AA90000 + m_farPointerOffset, 0x1AAA0000 + m_farPointerOffset, 0x1AAB0000 + m_farPointerOffset,
        0x1AAC0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassDragon;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertExplosion)
{
    constexpr uint16_t obClassExplosion = 20;
    constexpr uint32_t allState32[] = {
        0x1A550000 + m_farPointerOffset, 0x1A560000 + m_farPointerOffset, 0x1A570000 + m_farPointerOffset,
        0x1A590000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassExplosion;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertEye)
{
    constexpr uint16_t obClassEye = 21;
    constexpr uint32_t allState32[] = {
        0x1ABD0000 + m_farPointerOffset, 0x1ABE0000 + m_farPointerOffset, 0x1ABF0000 + m_farPointerOffset,
        0x1AC00000 + m_farPointerOffset, 0x1AC10000 + m_farPointerOffset, 0x1AC20000 + m_farPointerOffset,
        0x1AC30000 + m_farPointerOffset, 0x1AC40000 + m_farPointerOffset, 0x1AC50000 + m_farPointerOffset,
        0x1AC60000 + m_farPointerOffset, 0x1AC70000 + m_farPointerOffset, 0x1AC80000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassEye;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertWallSkeleton)
{
    constexpr uint16_t obClassWallSkeleton = 22;
    constexpr uint32_t allState32[] = {
        0x1AAF0000 + m_farPointerOffset, 0x1AB00000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassWallSkeleton;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertEyeShot)
{
    constexpr uint16_t obClassEyeShot = 23;
    constexpr uint32_t allState32[] = {
        0x1AC90000 + m_farPointerOffset, 0x1ACA0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassEyeShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertTree)
{
    constexpr uint16_t obClassTree = 24;
    constexpr uint32_t allState32[] = {
        0x1B380000 + m_farPointerOffset, 0x1B390000 + m_farPointerOffset, 0x1B3A0000 + m_farPointerOffset,
        0x1B3B0000 + m_farPointerOffset, 0x1B3C0000 + m_farPointerOffset, 0x1B3D0000 + m_farPointerOffset,
        0x1B3E0000 + m_farPointerOffset, 0x1B3F0000 + m_farPointerOffset, 0x1B400000 + m_farPointerOffset,
        0x1B410000 + m_farPointerOffset, 0x1B420000 + m_farPointerOffset, 0x1B430000 + m_farPointerOffset,
        0x1B440000 + m_farPointerOffset, 0x1B450000 + m_farPointerOffset, 0x1B460000 + m_farPointerOffset

    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassTree;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertNemesisShot)
{
    constexpr uint16_t obClassNemesisShot = 25;
    SavedGameInDosFormat::ObjectInDosFormat dosObject;
    dosObject.obclass = obClassNemesisShot;
    dosObject.state32 = 0x1B0A0000 + m_farPointerOffset;
    CheckDosObjectIsConvertible(dosObject);
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertRedDemon)
{
    constexpr uint16_t obClassRedDemon = 26;
    constexpr uint32_t allState32[] = {
        0x1AF10000 + m_farPointerOffset, 0x1AF20000 + m_farPointerOffset, 0x1AF30000 + m_farPointerOffset,
        0x1AF40000 + m_farPointerOffset, 0x1AF50000 + m_farPointerOffset, 0x1AF60000 + m_farPointerOffset,
        0x1AF70000 + m_farPointerOffset, 0x1AF80000 + m_farPointerOffset, 0x1AF90000 + m_farPointerOffset,
        0x1AFA0000 + m_farPointerOffset, 0x1AFB0000 + m_farPointerOffset, 0x1AFC0000 + m_farPointerOffset,
        0x1AFD0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassRedDemon;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertFreezeTime)
{
    constexpr uint16_t obClassFreezeTime = 27;
    constexpr uint32_t allState32[] = {
        0x1A760000 + m_farPointerOffset, 0x1A770000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassFreezeTime;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertSolid)
{
    constexpr uint16_t obClassSolid = 28;
    constexpr uint32_t allState32[] = {
        0x1B5B0000 + m_farPointerOffset, 0x1B5C0000 + m_farPointerOffset, 0x1B5D0000 + m_farPointerOffset,
        0x1B5E0000 + m_farPointerOffset, 0x1B5F0000 + m_farPointerOffset, 0x1B600000 + m_farPointerOffset,
        0x1B610000 + m_farPointerOffset,
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassSolid;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertDragonShot)
{
    constexpr uint16_t obClassDragonShot = 30;
    constexpr uint32_t allState32[] = {
        0x1AAD0000 + m_farPointerOffset, 0x1AAE0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassDragonShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertBunny)
{
    constexpr uint16_t obClassBunny = 32;
    constexpr uint32_t allState32[] = {
        0x1AE00000 + m_farPointerOffset, 0x1AE10000 + m_farPointerOffset, 0x1AE20000 + m_farPointerOffset,
        0x1AE30000 + m_farPointerOffset, 0x1AE40000 + m_farPointerOffset, 0x1AE50000 + m_farPointerOffset,
        0x1AE60000 + m_farPointerOffset, 0x1AE70000 + m_farPointerOffset, 0x1AE80000 + m_farPointerOffset,
        0x1AE90000 + m_farPointerOffset, 0x1AEA0000 + m_farPointerOffset, 0x1AEB0000 + m_farPointerOffset,
        0x1AEC0000 + m_farPointerOffset, 0x1AED0000 + m_farPointerOffset, 0x1AEE0000 + m_farPointerOffset,
        0x1AEF0000 + m_farPointerOffset, 0x1AF00000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassBunny;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterArmageddon_Test, ConvertRealSolid)
{
    constexpr uint16_t obClassRealSolid = 33;
    constexpr uint32_t allState32[] = {
        0x1B130000 + m_farPointerOffset, 0x1B140000 + m_farPointerOffset, 0x1B390000 + m_farPointerOffset,
        0x1B540000 + m_farPointerOffset, 0x1B550000 + m_farPointerOffset, 0x1B560000 + m_farPointerOffset,
        0x1B570000 + m_farPointerOffset, 0x1B580000 + m_farPointerOffset, 0x1B590000 + m_farPointerOffset,
        0x1B5A0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obClassRealSolid;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}