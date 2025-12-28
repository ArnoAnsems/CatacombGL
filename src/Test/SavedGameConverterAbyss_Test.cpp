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
#include "../Abyss/SavedGameConverterAbyss.h"
#include "../Abyss/DecorateAll.h"

class SavedGameConverterAbyss_Test : public ::testing::Test
{
protected:
    void CheckDosObjectIsConvertible(const GameId gameId, const SavedGameInDosFormat::ObjectInDosFormat& dosObject)
    {
        const SavedGameConverterAbyss converter(gameId);
        const uint16_t actorId = converter.GetActorId(dosObject);
        const auto actorIt = decorateAbyssAll.find(actorId);
        ASSERT_TRUE(actorIt != decorateAbyssAll.end());

        const DecorateStateId stateId = converter.GetDecorateStateId(dosObject);
        const auto stateIt = actorIt->second.states.find(stateId);
        ASSERT_TRUE(stateIt != actorIt->second.states.end());

        const uint16_t animationFrame = converter.GetAnimationFrame(dosObject);
        EXPECT_LE(animationFrame, stateIt->second.animation.size() - 1);
    }
};

TEST_F(SavedGameConverterAbyss_Test, ConvertBonus)
{
    const uint16_t obclassBonus = 2;
    const uint16_t allState16[][2] = {
        { 0x2060, 0x201A }, { 0x206A, 0x2024 }, { 0x2074, 0x202E }, { 0x207E, 0x2038 }, { 0x2088, 0x2042 }, { 0x2092, 0x204C },
        { 0x209C, 0x2056 }, { 0x20A6, 0x2060 }, { 0x20B0, 0x206A }, { 0x20BA, 0x2074 }, { 0x20C4, 0x207E }, { 0x20CE, 0x2088 },
        { 0x20D8, 0x2092 }, { 0x20E2, 0x209C }, { 0x20EC, 0x20A6 }, { 0x20F6, 0x20B0 }, { 0x2100, 0x20BA }, { 0x210A, 0x20C4 },
        { 0x2114, 0x20CE }, { 0x211E, 0x20D8 }, { 0x2128, 0x20E2 }, { 0x2132, 0x20EC }, { 0x213C, 0x20F6 }, { 0x2146, 0x2100 },
        { 0x2150, 0x210A }, { 0x215A, 0x2114 }, { 0x2164, 0x211E }, { 0x218C, 0x2146 }, { 0x2196, 0x2150 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBonus;
        dosObject.temp1 = 7; // Make it a valid scroll object
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertOrc)
{
    const uint16_t obclassOrc = 3;
    const uint16_t allState16[][2] = {
        { 0x25EA, 0x25A4 }, { 0x25F4, 0x25AE }, { 0x25FE, 0x25B8 }, { 0x2608, 0x25C2 }, { 0x2612, 0x25CC }, { 0x261C, 0x25D6 },
        { 0x2626, 0x25E0 }, { 0x2630, 0x25EA }, { 0x263A, 0x25F4 }, { 0x2644, 0x25FE }, { 0x264E, 0x2608 }, { 0x2658, 0x2612 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassOrc;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertBat)
{
    const uint16_t obclassBat = 4;
    const uint16_t allState16[][2] = {
        { 0x282E, 0x27E8 }, { 0x2838, 0x27F2 }, { 0x2842, 0x27FC }, { 0x284C, 0x2806 }, { 0x2856, 0x2810 }, { 0x2860, 0x281A },
        { 0x286A, 0x2824 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBat;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertSkeleton)
{
    const uint16_t obclassSkeleton = 5;
    const uint16_t allState16[][2] = {
        { 0x24FA, 0x24B4 }, { 0x2504, 0x24BE }, { 0x250E, 0x24C8 }, { 0x2518, 0x24D2 }, { 0x2522, 0x24DC }, { 0x252C, 0x24E6 },
        { 0x2536, 0x24F0 }, { 0x2540, 0x24FA }, { 0x254A, 0x2504 }, { 0x2554, 0x250E }, { 0x255E, 0x2518 }, { 0x2568, 0x2522 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassSkeleton;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertTroll)
{
    const uint16_t obclassTroll = 6;
    const uint16_t allState16[][2] = {
        { 0x220E, 0x21C8 }, { 0x2218, 0x21D2 }, { 0x2222, 0x21DC }, { 0x222C, 0x21E6 }, { 0x2236, 0x21F0 }, { 0x2240, 0x21FA },
        { 0x224A, 0x2204 }, { 0x2254, 0x220E }, { 0x225E, 0x2218 }, { 0x2268, 0x2222 }, { 0x2272, 0x222C }, { 0x227C, 0x2236 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassTroll;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertDemon)
{
    const uint16_t obclassDemon = 7;
    const uint16_t allState16[][2] = {
        { 0x2662, 0x261C }, { 0x266C, 0x2626 }, { 0x2676, 0x2630 }, { 0x2680, 0x263A }, { 0x268A, 0x2644 }, { 0x2694, 0x264E },
        { 0x269E, 0x2658 }, { 0x26A8, 0x2662 }, { 0x26B2, 0x266C }, { 0x26BC, 0x2676 }, { 0x26C6, 0x2680 }, { 0x26D0, 0x268A } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassDemon;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertMage)
{
    const uint16_t obclassMage = 8;
    const uint16_t allState16[][2] = {
        { 0x26DA, 0x2694 }, { 0x26E4, 0x269E }, { 0x26EE, 0x26A8 }, { 0x26F8, 0x26B2 }, { 0x2702, 0x26BC }, { 0x270C, 0x26C6 },
        { 0x2716, 0x26D0 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassMage;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertPlayerShot)
{
    const uint16_t obclassPlayerShot = 9;
    const uint16_t allState16[][2] = {
        { 0x1F0A, 0x1EC4 }, { 0x1F14, 0x1ECE } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassPlayerShot;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertBigPlayerShot)
{
    const uint16_t obclassBigPlayerShot = 10;
    const uint16_t allState16[][2] = {
        { 0x1F0A, 0x1EC4 }, { 0x1F14, 0x1ECE } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBigPlayerShot;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertMageShot)
{
    const uint16_t obclassMageShot = 11;
    const uint16_t allState16[][2] = {
        { 0x2720, 0x26DA }, { 0x272A, 0x26E4 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassMageShot;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertInert)
{
    const uint16_t obclassInert = 12;
    const uint16_t allState16[][2] = {
        { 0x21A0, 0x215A }, { 0x21AA, 0x2164 }, { 0x21B4, 0x216E }, { 0x21BE, 0x2178 }, { 0x21C8, 0x2182 }, { 0x21D2, 0x218C },
        { 0x2268, 0x2222 }, { 0x2272, 0x222C }, { 0x227C, 0x2236 }, { 0x234E, 0x2308 }, { 0x2358, 0x2312 }, { 0x2362, 0x231C },
        { 0x236C, 0x2326 }, { 0x2376, 0x2330 }, { 0x23E4, 0x239E }, { 0x23EE, 0x23A8 }, { 0x23F8, 0x23B2 }, { 0x24A2, 0x245C },
        { 0x24AC, 0x2466 }, { 0x24B6, 0x2470 }, { 0x24C0, 0x247A }, { 0x24CA, 0x2484 }, { 0x24D4, 0x248E }, { 0x2554, 0x250E },
        { 0x255E, 0x2518 }, { 0x2568, 0x2522 }, { 0x25B8, 0x2572 }, { 0x25C2, 0x257C }, { 0x25CC, 0x2586 }, { 0x2644, 0x25FE },
        { 0x264E, 0x2608 }, { 0x2658, 0x2612 }, { 0x26BC, 0x2676 }, { 0x26C6, 0x2680 }, { 0x26D0, 0x268A }, { 0x270C, 0x26C6 },
        { 0x2716, 0x26D0 }, { 0x278E, 0x2748 }, { 0x2798, 0x2752 }, { 0x27A2, 0x275C }, { 0x27DE, 0x2798 }, { 0x27E8, 0x27A2 },
        { 0x27F2, 0x27AC }, { 0x27FC, 0x27B6 }, { 0x2806, 0x27C0 }, { 0x2810, 0x27CA }, { 0x281A, 0x27D4 }, { 0x2860, 0x281A },
        { 0x286A, 0x2824 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassInert;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertNemesis)
{
    const uint16_t obclassGrelminar = 14;
    const uint16_t allState16[][2] = {
        { 0x27AC, 0x2766 }, { 0x27B6, 0x2770 }, { 0x27C0, 0x277A }, { 0x27CA, 0x2784 }, { 0x27D4, 0x278E }, { 0x27DE, 0x2798 },
        { 0x27E8, 0x27A2 }, { 0x27F2, 0x27AC }, { 0x27FC, 0x27B6 }, { 0x2806, 0x27C0 }, { 0x2810, 0x27CA }, { 0x281A, 0x27D4 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassGrelminar;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertGate)
{
    const uint16_t obclassGate = 15;
    const uint16_t allState16[][2] = {
        { 0x21DC, 0x2196 }, { 0x21E6, 0x21A0 }, { 0x21F0, 0x21AA }, { 0x21FA, 0x21B4 }, { 0x2204, 0x21BE } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassGate;
        dosObject.temp1 = 1; // Make it a valid gate object
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertZombie)
{
    const uint16_t obclassZombie = 16;
    const uint16_t allState16[][2] = {
        { 0x2380, 0x233A }, { 0x238A, 0x2344 }, { 0x2394, 0x234E }, { 0x239E, 0x2358 }, { 0x23A8, 0x2362 }, { 0x23B2, 0x236C },
        { 0x23BC, 0x2376 }, { 0x23C6, 0x2380 }, { 0x23D0, 0x238A }, { 0x23DA, 0x2394 }, { 0x23E4, 0x239E }, { 0x23EE, 0x23A8 },
        { 0x23F8, 0x23B2 }, { 0x2402, 0x23BC }, { 0x240C, 0x23C6 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassZombie;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertSpook)
{
    const uint16_t obclassSpook = 17;
    const uint16_t allState16[][2] = {
        { 0x2416, 0x23D0 }, { 0x2420, 0x23DA }, { 0x242A, 0x23E4 }, { 0x2434, 0x23EE }, { 0x243E, 0x23F8 }, { 0x2448, 0x2402 },
        { 0x2452, 0x240C }, { 0x245C, 0x2416 }, { 0x2466, 0x2420 }, { 0x2470, 0x242A }, { 0x247A, 0x2434 }, { 0x2484, 0x243E },
        { 0x248E, 0x2448 }, { 0x2498, 0x2452 }, { 0x24A2, 0x245C }, { 0x24AC, 0x2466 }, { 0x24B6, 0x2470 }, { 0x24C0, 0x247A },
        { 0x24CA, 0x2484 }, { 0x24D4, 0x248E } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassSpook;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertWetMan)
{
    const uint16_t obclassWet = 18;
    const uint16_t allState16[][2] = {
        { 0x2286, 0x2240 }, { 0x2290, 0x224A }, { 0x229A, 0x2254 }, { 0x22A4, 0x225E }, { 0x22AE, 0x2268 }, { 0x22B8, 0x2272 },
        { 0x22C2, 0x227C }, { 0x22CC, 0x2286 }, { 0x22D6, 0x2290 }, { 0x22E0, 0x229A }, { 0x22EA, 0x22A4 }, { 0x22F4, 0x22AE },
        { 0x22FE, 0x22B8 }, { 0x2308, 0x22C2 }, { 0x2312, 0x22CC }, { 0x231C, 0x22D6 }, { 0x2326, 0x22E0 }, { 0x2330, 0x22EA },
        { 0x233A, 0x22F4 }, { 0x2344, 0x22FE }, { 0x234E, 0x2308 }, { 0x2358, 0x2312 }, { 0x2362, 0x231C }, { 0x236C, 0x2326 },
        { 0x2376, 0x2330 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassWet;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertExplosion)
{
    const uint16_t obclassExp = 19;
    const uint16_t allState16[][2] = {
        { 0x1F52, 0x1F0C } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassExp;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertEye)
{
    const uint16_t obclassEye = 20;
    const uint16_t allState16[][2] = {
        { 0x2572, 0x252C }, { 0x257C, 0x2536 }, { 0x2586, 0x2540 }, { 0x2590, 0x254A }, { 0x259A, 0x2554 }, { 0x25A4, 0x255E },
        { 0x25AE, 0x2568 }, { 0x25B8, 0x2572 }, { 0x25C2, 0x257C }, { 0x25CC, 0x2586 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassEye;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertWallSkeleton)
{
    const uint16_t obclassWallSkeleton = 21;
    const uint16_t allState16[][2] = {
        { 0x24DE, 0x2498 }, { 0x24E8, 0x24A2 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassWallSkeleton;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertEyeShot)
{
    const uint16_t obclassEyeShot = 22;
    const uint16_t allState16[][2] = {
        { 0x25D6, 0x2590 }, { 0x25E0, 0x259A } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassEyeShot;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertNemesisShot)
{
    const uint16_t obclassGrelminarShot = 23;
    const uint16_t allState16[][2] = {
        { 0x2824, 0x27DE } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassGrelminarShot;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertRedDemon)
{
    const uint16_t obclassRedDemon = 24;
    const uint16_t allState16[][2] = {
        { 0x2734, 0x26EE }, { 0x273E, 0x26F8 }, { 0x2748, 0x2702 }, { 0x2752, 0x270C }, { 0x275C, 0x2716 }, { 0x2766, 0x2720 },
        { 0x2770, 0x272A }, { 0x277A, 0x2734 }, { 0x2784, 0x273E }, { 0x278E, 0x2748 }, { 0x2798, 0x2752 }, { 0x27A2, 0x275C } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassRedDemon;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertFreeze)
{
    const uint16_t obclassFreeze = 25;
    const uint16_t allState16[][2] = {
        { 0x218C, 0x2146 }, { 0x2196, 0x2150 } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassFreeze;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}

TEST_F(SavedGameConverterAbyss_Test, ConvertSolid)
{
    const uint16_t obclassSolid = 26;
    const uint16_t allState16[][2] = {
        { 0x216E, 0x2128 }, { 0x2178, 0x2132 }, { 0x2182, 0x213C } };
    for (auto state16 : allState16)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassSolid;
        dosObject.state16 = state16[0];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv113, dosObject);

        dosObject.state16 = state16[1];
        CheckDosObjectIsConvertible(GameId::CatacombAbyssv124, dosObject);
    }
}