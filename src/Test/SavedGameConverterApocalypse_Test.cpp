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

#include <gtest/gtest.h>
#include "../Apocalypse/SavedGameConverterApocalypse.h"
#include "../Apocalypse/DecorateAll.h"

class SavedGameConverterApocalypse_Test : public ::testing::Test
{
protected:
    void CheckDosObjectIsConvertible(const SavedGameInDosFormat::ObjectInDosFormat& dosObject)
    {
        SavedGameConverterApocalypse converter;
        converter.SetFarPointerOffset(m_farPointerOffset + 0x1A6F0000);
        const uint16_t actorId = converter.GetActorId(dosObject);
        const auto actorIt = decorateApocalypseAll.find(actorId);
        ASSERT_TRUE(actorIt != decorateApocalypseAll.end());

        const DecorateStateId stateId = converter.GetDecorateStateId(dosObject);
        const auto stateIt = actorIt->second.states.find(stateId);
        ASSERT_TRUE(stateIt != actorIt->second.states.end());

        const uint16_t animationFrame = converter.GetAnimationFrame(dosObject);
        EXPECT_LE(animationFrame, stateIt->second.animation.size() - 1);
    }

    static constexpr uint32_t m_farPointerOffset = 0x1A20000;
};


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

TEST_F(SavedGameConverterApocalypse_Test, ConvertTroll)
{
    constexpr uint16_t obclassTroll = 5;
    constexpr uint32_t allState32[] = {
        0x1B460000 + m_farPointerOffset, 0x1B470000 + m_farPointerOffset, 0x1B480000 + m_farPointerOffset,
        0x1B490000 + m_farPointerOffset, 0x1B4A0000 + m_farPointerOffset, 0x1B4B0000 + m_farPointerOffset,
        0x1B4C0000 + m_farPointerOffset, 0x1B4D0000 + m_farPointerOffset, 0x1B4E0000 + m_farPointerOffset,
        0x1B4F0000 + m_farPointerOffset, 0x1B500000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassTroll;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertWizardShot)
{
    constexpr uint16_t obclassWizardShot = 6;
    constexpr uint32_t allState32[] = {
        0x1AB80000 + m_farPointerOffset, 0x1AB90000 + m_farPointerOffset,
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassWizardShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertBlob)
{
    constexpr uint16_t obclassBlob = 7;
    constexpr uint32_t allState32[] = {
        0x1AC80000 + m_farPointerOffset, 0x1AC90000 + m_farPointerOffset, 0x1ACA0000 + m_farPointerOffset,
        0x1ACB0000 + m_farPointerOffset, 0x1ACC0000 + m_farPointerOffset, 0x1ACD0000 + m_farPointerOffset,
        0x1ACE0000 + m_farPointerOffset, 0x1ACF0000 + m_farPointerOffset, 0x1AD00000 + m_farPointerOffset,
        0x1AD10000 + m_farPointerOffset, 0x1AD20000 + m_farPointerOffset, 0x1AD30000 + m_farPointerOffset,
        0x1AD40000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBlob;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertBlobShot)
{
    constexpr uint16_t obclassBlobShot = 8;
    constexpr uint32_t allState32[] = {
        0x1AD50000 + m_farPointerOffset, 0x1AD60000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBlobShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertFutureMage)
{
    constexpr uint16_t obclassFutureMage = 9;
    constexpr uint32_t allState32[] = {
        0x1AE50000 + m_farPointerOffset, 0x1AE60000 + m_farPointerOffset, 0x1AE70000 + m_farPointerOffset,
        0x1AE80000 + m_farPointerOffset, 0x1AE90000 + m_farPointerOffset, 0x1AEA0000 + m_farPointerOffset,
        0x1AEB0000 + m_farPointerOffset, 0x1AEC0000 + m_farPointerOffset, 0x1AED0000 + m_farPointerOffset,
        0x1AEE0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassFutureMage;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertRambone)
{
    constexpr uint16_t obclassRambone = 10;
    constexpr uint32_t allState32[] = {
        0x1AD70000 + m_farPointerOffset, 0x1AD80000 + m_farPointerOffset, 0x1AD90000 + m_farPointerOffset,
        0x1ADA0000 + m_farPointerOffset, 0x1ADB0000 + m_farPointerOffset, 0x1ADC0000 + m_farPointerOffset,
        0x1ADD0000 + m_farPointerOffset, 0x1ADE0000 + m_farPointerOffset, 0x1ADF0000 + m_farPointerOffset,
        0x1AE00000 + m_farPointerOffset, 0x1AE10000 + m_farPointerOffset, 0x1AE20000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassRambone;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertRobotank)
{
    constexpr uint16_t obclassRobotank = 11;
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
        dosObject.obclass = obclassRobotank;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertRobotankShot)
{
    constexpr uint16_t obclassRobotankShot = 12;
    constexpr uint32_t allState32[] = {
        0x1AFE0000 + m_farPointerOffset, 0x1AFF0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassRobotankShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertStompy)
{
    constexpr uint16_t obclassStompy = 13;
    constexpr uint32_t allState32[] = {
        0x1B000000 + m_farPointerOffset, 0x1B010000 + m_farPointerOffset, 0x1B020000 + m_farPointerOffset,
        0x1B030000 + m_farPointerOffset, 0x1B040000 + m_farPointerOffset, 0x1B050000 + m_farPointerOffset,
        0x1B060000 + m_farPointerOffset, 0x1B070000 + m_farPointerOffset, 0x1B080000 + m_farPointerOffset,
        0x1B090000 + m_farPointerOffset, 0x1B0A0000 + m_farPointerOffset, 0x1B0B0000 + m_farPointerOffset,
        0x1B0C0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassStompy;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertStompyShot)
{
    constexpr uint16_t obclassStompyShot = 14;
    constexpr uint32_t allState32[] = {
        0x1B0D0000 + m_farPointerOffset, 0x1B0E0000 + m_farPointerOffset, 0x1B0F0000 + m_farPointerOffset,
        0x1B100000 + m_farPointerOffset, 0x1B110000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassStompyShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertAndroidMageShot)
{
    constexpr uint16_t obclassAndroidMageShot = 15;
    constexpr uint32_t allState32[] = {
        0x1AEF0000 + m_farPointerOffset, 0x1AF00000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassAndroidMageShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertRoboTankShot)
{
    constexpr uint16_t obclassRoboTankShot = 16;
    constexpr uint32_t allState32[] = {
        0x1AFE0000 + m_farPointerOffset, 0x1AFF0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassRoboTankShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertBug)
{
    constexpr uint16_t obclassBug = 17;
    constexpr uint32_t allState32[] = {
        0x1B120000 + m_farPointerOffset, 0x1B130000 + m_farPointerOffset, 0x1B140000 + m_farPointerOffset,
        0x1B150000 + m_farPointerOffset, 0x1B160000 + m_farPointerOffset, 0x1B170000 + m_farPointerOffset,
        0x1B180000 + m_farPointerOffset, 0x1B190000 + m_farPointerOffset,
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBug;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertBugShot)
{
    constexpr uint16_t obclassBugShot = 18;
    constexpr uint32_t allState32[] = {
        0x1B1C0000 + m_farPointerOffset, 0x1B1D0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBugShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertCyborgDemon)
{
    constexpr uint16_t obclassCyborgDemon = 19;
    constexpr uint32_t allState32[] = {
        0x1B510000 + m_farPointerOffset, 0x1B520000 + m_farPointerOffset, 0x1B530000 + m_farPointerOffset,
        0x1B540000 + m_farPointerOffset, 0x1B550000 + m_farPointerOffset, 0x1B560000 + m_farPointerOffset,
        0x1B570000 + m_farPointerOffset,
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassCyborgDemon;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertInvisDude)
{
    constexpr uint16_t obclassInvisDude = 20;
    constexpr uint32_t allState32[] = {
        0x1B5C0000 + m_farPointerOffset, 0x1B5D0000 + m_farPointerOffset, 0x1B5E0000 + m_farPointerOffset,
        0x1B5F0000 + m_farPointerOffset, 0x1B600000 + m_farPointerOffset, 0x1B610000 + m_farPointerOffset,
        0x1B620000 + m_farPointerOffset, 0x1B630000 + m_farPointerOffset, 0x1B640000 + m_farPointerOffset,
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassInvisDude;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertBounce)
{
    constexpr uint16_t obclassBounce = 21;
    constexpr uint32_t allState32[] = {
        0x1B680000 + m_farPointerOffset, 0x1B690000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBounce;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertEye)
{
    constexpr uint16_t obclassEye = 22;
    constexpr uint32_t allState32[] = {
        0x1B1F0000 + m_farPointerOffset, 0x1B200000 + m_farPointerOffset, 0x1B210000 + m_farPointerOffset,
        0x1B220000 + m_farPointerOffset, 0x1B230000 + m_farPointerOffset, 0x1B240000 + m_farPointerOffset,
        0x1B250000 + m_farPointerOffset, 0x1B260000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassEye;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertRunningEye)
{
    constexpr uint16_t obclassRunningEye = 23;
    constexpr uint32_t allState32[] = {
        0x1B2D0000 + m_farPointerOffset, 0x1B2E0000 + m_farPointerOffset, 0x1B2F0000 + m_farPointerOffset,
        0x1B300000 + m_farPointerOffset, 0x1B310000 + m_farPointerOffset, 0x1B320000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassRunningEye;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertRay)
{
    constexpr uint16_t obclassRay = 24;
    constexpr uint32_t allState32[] = {
        0x1ABA0000 + m_farPointerOffset, 0x1ABB0000 + m_farPointerOffset, 0x1ABC0000 + m_farPointerOffset,
        0x1ABD0000 + m_farPointerOffset, 0x1ABE0000 + m_farPointerOffset, 0x1ABF0000 + m_farPointerOffset,
        0x1AC00000 + m_farPointerOffset, 0x1AC10000 + m_farPointerOffset, 0x1AC20000 + m_farPointerOffset,
        0x1AC30000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassRay;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertRayShot)
{
    constexpr uint16_t obclassRayShot = 25;
    constexpr uint32_t allState32[] = {
        0x1AC60000 + m_farPointerOffset, 0x1AC70000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassRayShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertTimeLord)
{
    constexpr uint16_t obclassTimeLord = 26;
    SavedGameInDosFormat::ObjectInDosFormat dosObject;
    dosObject.obclass = obclassTimeLord;
    dosObject.state32 = 0x1B370000 + m_farPointerOffset;

    CheckDosObjectIsConvertible(dosObject);
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertTimeLordShot)
{
    constexpr uint16_t obclassTimeLordShot = 27;
    constexpr uint32_t allState32[] = {
        0x1B380000 + m_farPointerOffset, 0x1B390000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassTimeLordShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertDemon)
{
    constexpr uint16_t obclassDemon = 28;
    constexpr uint32_t allState32[] = {
        0x1B3A0000 + m_farPointerOffset, 0x1B3B0000 + m_farPointerOffset, 0x1B3C0000 + m_farPointerOffset,
        0x1B3D0000 + m_farPointerOffset, 0x1B3E0000 + m_farPointerOffset, 0x1B3F0000 + m_farPointerOffset,
        0x1B400000 + m_farPointerOffset, 0x1B410000 + m_farPointerOffset, 0x1B420000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassDemon;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertPlayerShot)
{
    constexpr uint16_t obclassPlayerShot = 29;
    constexpr uint32_t allState32[] = {
        0x1A6A0000 + m_farPointerOffset, 0x1A6B0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassPlayerShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertNemesis)
{
    constexpr uint16_t obclassNemesis = 30;
    constexpr uint32_t allState32[] = {
        0x1B6A0000 + m_farPointerOffset, 0x1B6B0000 + m_farPointerOffset, 0x1B6C0000 + m_farPointerOffset,
        0x1B6D0000 + m_farPointerOffset, 0x1B6E0000 + m_farPointerOffset,
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassNemesis;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertFreeze)
{
    constexpr uint16_t obclassFreeze= 31;
    constexpr uint32_t allState32[] = {
        0x1A870000 + m_farPointerOffset, 0x1A880000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassFreeze;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertSolid)
{
    constexpr uint16_t obclassSolid = 32;
    constexpr uint32_t allState32[] = {
        0x1B810000 + m_farPointerOffset, 0x1B820000 + m_farPointerOffset, 0x1B830000 + m_farPointerOffset,
        0x1B840000 + m_farPointerOffset, 0x1B870000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassSolid;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertInert)
{
    constexpr uint16_t obclassInert = 33;
    constexpr uint32_t allState32[] = {
        0x1A6C0000 + m_farPointerOffset, 0x1A6D0000 + m_farPointerOffset, 0x1A6E0000 + m_farPointerOffset,
        0x1A860000 + m_farPointerOffset, 0x1A890000 + m_farPointerOffset, 0x1A8A0000 + m_farPointerOffset,
        0x1A8B0000 + m_farPointerOffset, 0x1A8C0000 + m_farPointerOffset, 0x1A8D0000 + m_farPointerOffset,
        0x1A8E0000 + m_farPointerOffset, 0x1AA30000 + m_farPointerOffset, 0x1AA40000 + m_farPointerOffset,
        0x1AA50000 + m_farPointerOffset, 0x1AA60000 + m_farPointerOffset, 0x1AA70000 + m_farPointerOffset,
        0x1AA80000 + m_farPointerOffset, 0x1AA90000 + m_farPointerOffset, 0x1AB10000 + m_farPointerOffset,
        0x1AB20000 + m_farPointerOffset, 0x1AB30000 + m_farPointerOffset, 0x1AB40000 + m_farPointerOffset,
        0x1AC40000 + m_farPointerOffset, 0x1AC50000 + m_farPointerOffset, 0x1AD20000 + m_farPointerOffset,
        0x1AD30000 + m_farPointerOffset, 0x1AD40000 + m_farPointerOffset, 0x1AE00000 + m_farPointerOffset,
        0x1AE10000 + m_farPointerOffset, 0x1AE20000 + m_farPointerOffset, 0x1AEC0000 + m_farPointerOffset,
        0x1AED0000 + m_farPointerOffset, 0x1AEE0000 + m_farPointerOffset, 0x1AF90000 + m_farPointerOffset,
        0x1AFA0000 + m_farPointerOffset, 0x1AFB0000 + m_farPointerOffset, 0x1AFC0000 + m_farPointerOffset,
        0x1AFD0000 + m_farPointerOffset, 0x1B090000 + m_farPointerOffset, 0x1B0A0000 + m_farPointerOffset,
        0x1B0B0000 + m_farPointerOffset, 0x1B0C0000 + m_farPointerOffset, 0x1B1A0000 + m_farPointerOffset,
        0x1B1B0000 + m_farPointerOffset, 0x1B270000 + m_farPointerOffset, 0x1B280000 + m_farPointerOffset,
        0x1B290000 + m_farPointerOffset, 0x1B2A0000 + m_farPointerOffset, 0x1B330000 + m_farPointerOffset,
        0x1B340000 + m_farPointerOffset, 0x1B350000 + m_farPointerOffset, 0x1B360000 + m_farPointerOffset,
        0x1B430000 + m_farPointerOffset, 0x1B440000 + m_farPointerOffset, 0x1B450000 + m_farPointerOffset,
        0x1B4E0000 + m_farPointerOffset, 0x1B4F0000 + m_farPointerOffset, 0x1B500000 + m_farPointerOffset,
        0x1B590000 + m_farPointerOffset, 0x1B5A0000 + m_farPointerOffset, 0x1B5B0000 + m_farPointerOffset,
        0x1B650000 + m_farPointerOffset, 0x1B660000 + m_farPointerOffset, 0x1B670000 + m_farPointerOffset,
        0x1B6F0000 + m_farPointerOffset, 0x1B700000 + m_farPointerOffset, 0x1B710000 + m_farPointerOffset,
        0x1B720000 + m_farPointerOffset, 0x1B730000 + m_farPointerOffset, 0x1B740000 + m_farPointerOffset,
        0x1B750000 + m_farPointerOffset, 0x1B850000 + m_farPointerOffset, 0x1B860000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassInert;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertGate)
{
    constexpr uint16_t obclassGate = 34;
    constexpr uint32_t allState32[] = {
         0x1A8F0000 + m_farPointerOffset, 0x1A900000 + m_farPointerOffset, 0x1A910000 + m_farPointerOffset,
        0x1A920000 + m_farPointerOffset, 0x1A930000 + m_farPointerOffset, 0x1A940000 + m_farPointerOffset,
        0x1A950000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassGate;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertRealSolid)
{
    constexpr uint16_t obclassRealSolid = 35;
    constexpr uint32_t allState32[] = {
        0x1B370000 + m_farPointerOffset, 0x1B770000 + m_farPointerOffset, 0x1B780000 + m_farPointerOffset,
        0x1B790000 + m_farPointerOffset, 0x1B7A0000 + m_farPointerOffset, 0x1B7B0000 + m_farPointerOffset,
        0x1B7C0000 + m_farPointerOffset, 0x1B7D0000 + m_farPointerOffset, 0x1B7E0000 + m_farPointerOffset,
        0x1B7F0000 + m_farPointerOffset, 0x1B800000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassRealSolid;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertExplosion)
{
    constexpr uint16_t obclassExplosion = 36;
    constexpr uint32_t allState32[] = {
        0x1A6C0000 + m_farPointerOffset, 0x1A6D0000 + m_farPointerOffset, 0x1A6E0000 + m_farPointerOffset,
        0x1A700000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassExplosion;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertBigPlayerShot)
{
    constexpr uint16_t obclassBigPlayerShot = 37;
    constexpr uint32_t allState32[] = {
        0x1A6A0000 + m_farPointerOffset, 0x1A6B0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBigPlayerShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertEyeShot)
{
    constexpr uint16_t obclassEyeShot = 38;
    constexpr uint32_t allState32[] = {
        0x1B2B0000 + m_farPointerOffset, 0x1B2C0000 + m_farPointerOffset
    };
    for (uint32_t state32 : allState32)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassEyeShot;
        dosObject.state32 = state32;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST_F(SavedGameConverterApocalypse_Test, ConvertNemesisShot)
{
    constexpr uint16_t obclassNemesisShot = 39;
    SavedGameInDosFormat::ObjectInDosFormat dosObject;
    dosObject.obclass = obclassNemesisShot;
    dosObject.state32 = 0x1B760000 + m_farPointerOffset;

    CheckDosObjectIsConvertible(dosObject);
}