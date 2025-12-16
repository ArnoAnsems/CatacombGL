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

#include "SavedGameConverterCatacomb3D_Test.h"
#include "../Catacomb3D/SavedGameConverterCatacomb3D.h"
#include "../Catacomb3D/DecorateAll.h"

SavedGameConverterCatacomb3D_Test::SavedGameConverterCatacomb3D_Test()
{

}

SavedGameConverterCatacomb3D_Test::~SavedGameConverterCatacomb3D_Test()
{

}

void CheckDosObjectIsConvertible(const SavedGameInDosFormat::ObjectInDosFormat& dosObject)
{
    SavedGameConverterCatacomb3D converter(GameId::Catacomb3Dv122);
    const uint16_t actorId = converter.GetActorId(dosObject);
    const auto actorIt = decorateCatacomb3DAll.find(actorId);
    ASSERT_TRUE(actorIt != decorateCatacomb3DAll.end());

    const DecorateStateId stateId = converter.GetDecorateStateId(dosObject);
    const auto stateIt = actorIt->second.states.find(stateId);
    ASSERT_TRUE(stateIt != actorIt->second.states.end());

    const uint16_t animationFrame = converter.GetAnimationFrame(dosObject);
    EXPECT_LE(animationFrame, stateIt->second.animation.size() - 1);
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertBonus)
{
    const uint16_t obclassBonus = 2;
    const uint16_t allState16[] = { 0x17C4, 0x17CE, 0x17D8, 0x17E2, 0x17EC, 0x17F6, 0x1800, 0x180A, 0x1814, 0x181E, 0x1828, 0x1832 };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBonus;
        dosObject.state16 = state16;
        dosObject.temp1 = 7;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertOrc)
{
    const uint16_t obclassOrc = 3;
    const uint16_t allState16[] = { 0x190E, 0x1940, 0x194A, 0x1954, 0x195E, 0x1968, 0x1972, 0x197C, 0x1986, 0x1990, 0x199A, 0x19A4, 0x19AE };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassOrc;
        dosObject.state16 = state16;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertBat)
{
    constexpr uint16_t obclassBat = 4;
    const uint16_t allState16[] = { 0x1B20, 0x1B2A, 0x1B34, 0x1B3E, 0x1B48, 0x1B52, 0x1B5C, 0x1B66, 0x1B70 };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBat;
        dosObject.state16 = state16;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertTroll)
{
    constexpr uint16_t obclassTroll = 6;
    const uint16_t allState16[] = { 0x18C8, 0x18D2, 0x18DC, 0x18E6, 0x18F0, 0x18FA, 0x1904, 0x190E, 0x1918, 0x1922, 0x192C, 0x1936 };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassTroll;
        dosObject.state16 = state16;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertDemon)
{
    constexpr uint16_t obclassDemon = 7;
    const uint16_t allState16[] = { 0x19B8,  0x19C2, 0x19CC, 0x19D6, 0x19E0, 0x19EA, 0x19F4, 0x19FE, 0x1A08, 0x1A12, 0x1A1C, 0x1A26 };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassDemon;
        dosObject.state16 = state16;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertMage)
{
    constexpr uint16_t obclassMage = 8;
    const uint16_t allState16[] = { 0x1A44, 0x1A4E, 0x1A58, 0x1A62, 0x1A6C, 0x1A76, 0x1A80, 0x1A8A, 0x1A94 };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassMage;
        dosObject.state16 = state16;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertPlayerShot)
{
    constexpr uint16_t obclassPlayerShot = 9;
    const uint16_t allState16[] = { 0x1776, 0x1780 };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassPlayerShot;
        dosObject.state16 = state16;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertPlayerBigShot)
{
    constexpr uint16_t obclassBigPlayerShot = 10;
    const uint16_t allState16[] = { 0x1794, 0x179E };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBigPlayerShot;
        dosObject.state16 = state16;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertMageShot)
{
    constexpr uint16_t obclassMageShot = 11;
    const uint16_t allState16[] = { 0x1A30, 0x1A3A };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassMageShot;
        dosObject.state16 = state16;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertInert)
{
    constexpr uint16_t obclassInert = 12;
    const uint16_t allState16[] = {
        0x183C, 0x1846, 0x1850, 0x185A, 0x1864, 0x186E, 0x1922, 0x192C, 0x1936, 0x199A, 0x19A4, 0x19AE, 0x1A12, 0x1A1C, 0x1A26,
        0x1A8A, 0x1A94, 0x1AE4, 0x1AEE, 0x1AF8, 0x1B02, 0x1B0C, 0x1B16, 0x1B52, 0x1B5C };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassInert;
        dosObject.state16 = state16;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertBounce)
{
    constexpr uint16_t obclassBounce = 13;
    const uint16_t allState16[] = { 0x1B66, 0x1B70 };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBounce;
        dosObject.state16 = state16;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertNemesis)
{
    constexpr uint16_t obclassGrelminar = 14;
    const uint16_t allState16[] = { 0x1A9E, 0x1AA8, 0x1AB2, 0x1ABC, 0x1AC6, 0x1AD0, 0x1ADA, 0x1AE4, 0x1AEE, 0x1AF8, 0x1B02, 0x1B0C, 0x1B16 };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassGrelminar;
        dosObject.state16 = state16;

        CheckDosObjectIsConvertible(dosObject);
    }
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertGate)
{
    constexpr uint16_t obclassGate = 15;
    const uint16_t allState16[] = { 0x1878, 0x1882, 0x188C, 0x1896, 0x18A0, 0x18AA, 0x18B4, 0x18BE };
    for (uint16_t state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassGate;
        dosObject.state16 = state16;

        CheckDosObjectIsConvertible(dosObject);
    }
}