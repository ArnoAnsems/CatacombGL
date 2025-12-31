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
#include "../Catacomb3D/SavedGameConverterCatacomb3D.h"
#include "../Catacomb3D/DecorateAll.h"

class SavedGameConverterCatacomb3D_Test : public ::testing::Test
{
protected:
    void CheckDosObjectIsConvertible(const GameId gameId, const SavedGameInDosFormat::ObjectInDosFormat& dosObject)
    {
        const SavedGameConverterCatacomb3D converter(gameId);
        const uint16_t actorId = converter.GetActorId(dosObject);
        const auto actorIt = decorateCatacomb3DAll.find(actorId);
        ASSERT_TRUE(actorIt != decorateCatacomb3DAll.end());

        const DecorateStateId stateId = converter.GetDecorateStateId(dosObject);
        const auto stateIt = actorIt->second.states.find(stateId);
        ASSERT_TRUE(stateIt != actorIt->second.states.end());

        const uint16_t animationFrame = converter.GetAnimationFrame(dosObject);
        EXPECT_LE(animationFrame, stateIt->second.animation.size() - 1);
    }
};

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertBonus)
{
    constexpr uint16_t obclassBonus = 2;
    constexpr uint16_t allState16[] = { 0x17C4, 0x17CE, 0x17D8, 0x17E2, 0x17EC, 0x17F6, 0x1800, 0x180A, 0x1814, 0x181E, 0x1828, 0x1832 };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBonus;
        dosObject.state16 = state16;
        dosObject.temp1 = 7;

        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertOrc)
{
    constexpr uint16_t obclassOrc = 3;
    constexpr uint16_t allState16[][2] =
    {
        { 0x1664, 0x1940 }, { 0x166E, 0x194A }, { 0x1678, 0x1954 }, { 0x1682, 0x195E }, { 0x168C, 0x1968 },
        { 0x1696, 0x1972 }, { 0x16A0, 0x197C }, { 0x16AA, 0x1986 }, { 0x16B4, 0x1990 }, { 0x16BE, 0x199A },
        { 0x16C8, 0x19A4 }, { 0x19AE, 0x19AE }
    };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassOrc;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertBat)
{
    constexpr uint16_t obclassBat = 4;
    constexpr uint16_t allState16[][2] =
    {
        { 0x1844, 0x1B20 }, { 0x184E, 0x1B2A }, { 0x1858, 0x1B34 }, { 0x1862, 0x1B3E }, { 0x186C, 0x1B48 },
        { 0x1876, 0x1B52 }, { 0x1880, 0x1B5C }
    };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBat;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertTroll)
{
    constexpr uint16_t obclassTroll = 6;
    constexpr uint16_t allState16[][2] =
    {
        { 0x15EC, 0x18C8 }, { 0x15F6, 0x18D2 }, { 0x1600, 0x18DC }, { 0x160A, 0x18E6 }, { 0x1614, 0x18F0 },
        { 0x161E, 0x18FA }, { 0x1628, 0x1904 }, { 0x1632, 0x190E }, { 0x163C, 0x1918 }, { 0x1646, 0x1922 },
        { 0x1650, 0x192C }, { 0x1936, 0x1936 }
    };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassTroll;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertDemon)
{
    constexpr uint16_t obclassDemon = 7;
    constexpr uint16_t allState16[][2] =
    {
        { 0x16DC, 0x19B8 }, { 0x16E6, 0x19C2 }, { 0x16F0, 0x19CC }, { 0x16FA, 0x19D6 }, { 0x1704, 0x19E0 },
        { 0x170E, 0x19EA }, { 0x1718, 0x19F4 }, { 0x1722, 0x19FE }, { 0x172C, 0x1A08 }, { 0x1736, 0x1A12 },
        { 0x1740, 0x1A1C }, { 0x1A26, 0x1A26 }
    };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassDemon;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertMage)
{
    constexpr uint16_t obclassMage = 8;
    constexpr uint16_t allState16[][2] =
    {
        { 0x1768, 0x1A44 }, { 0x1772, 0x1A4E }, { 0x177C, 0x1A58 }, { 0x1786, 0x1A62 }, { 0x1790, 0x1A6C },
        { 0x179A, 0x1A76 }, { 0x17A4, 0x1A80 }, { 0x17AE, 0x1A8A }, { 0x1A94, 0x1A94 }
    };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassMage;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertPlayerShot)
{
    constexpr uint16_t obclassPlayerShot = 9;
    constexpr uint16_t allState16[][2] = { { 0x149A, 0x1776 }, { 0x14A4, 0x1780 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassPlayerShot;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertPlayerBigShot)
{
    constexpr uint16_t obclassBigPlayerShot = 10;
    constexpr uint16_t allState16[][2] = { {0x14B8, 0x1794}, {0x14C2, 0x179E} };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBigPlayerShot;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertMageShot)
{
    constexpr uint16_t obclassMageShot = 11;
    const uint16_t allState16[][2] = {{0x1754, 0x1A30}, {0x175E, 0x1A3A}};
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassMageShot;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertInert)
{
    constexpr uint16_t obclassInert = 12;
    constexpr uint16_t allState16[][2] =
    {
        { 0x183C, 0x183C }, { 0x1846, 0x1846 }, { 0x1850, 0x1850 }, { 0x185A, 0x185A }, { 0x1864, 0x1864 },
        { 0x186E, 0x186E }, { 0x1646, 0x1922 }, { 0x1650, 0x192C }, { 0x1936, 0x1936 }, { 0x16BE, 0x199A },
        { 0x16C8, 0x19A4 }, { 0x19AE, 0x19AE }, { 0x1736, 0x1A12 }, { 0x1740, 0x1A1C }, { 0x1A26, 0x1A26 },
        { 0x17AE, 0x1A8A }, { 0x1A94, 0x1A94 }, { 0x1808, 0x1AE4 }, { 0x1812, 0x1AEE }, { 0x181C, 0x1AF8 },
        { 0x1826, 0x1B02 }, { 0x1830, 0x1B0C }, { 0x1B16, 0x1B16 }, { 0x1876, 0x1B52 }, { 0x1880, 0x1B5C }
    };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassInert;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertBounce)
{
    constexpr uint16_t obclassBounce = 13;
    constexpr uint16_t allState16[][2] = { { 0x188A, 0x1B66 }, { 0x1894, 0x1B70 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBounce;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertNemesis)
{
    constexpr uint16_t obclassGrelminar = 14;
    constexpr uint16_t allState16[][2] =
    {
        { 0x17C2, 0x1A9E }, { 0x17CC, 0x1AA8 }, { 0x17D6, 0x1AB2 }, { 0x17E0, 0x1ABC }, { 0x17EA, 0x1AC6 },
        { 0x17F4, 0x1AD0 }, { 0x17FE, 0x1ADA }, { 0x1808, 0x1AE4 }, { 0x1812, 0x1AEE }, { 0x181C, 0x1AF8 },
        { 0x1826, 0x1B02 }, { 0x1830, 0x1B0C }, { 0x1B16, 0x1B16 }
    };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassGrelminar;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}

TEST_F(SavedGameConverterCatacomb3D_Test, ConvertGate)
{
    constexpr uint16_t obclassGate = 15;
    constexpr uint16_t allState16[][2] =
    {
        { 0x159C, 0x1878 }, { 0x15A6, 0x1882 }, { 0x15B0, 0x188C }, { 0x15BA, 0x1896 }, { 0x15C4, 0x18A0 },
        { 0x15CE, 0x18AA }, { 0x15D8, 0x18B4 }, { 0x15E2, 0x18BE }
    };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassGate;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv100, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::Catacomb3Dv122, dosObject);
    }
}