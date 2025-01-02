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

#include "SavedGameConverterAbyss_Test.h"
#include "../Abyss/SavedGameConverterAbyss.h"
#include "../Abyss/DecorateAll.h"

SavedGameConverterAbyss_Test::SavedGameConverterAbyss_Test()
{

}

SavedGameConverterAbyss_Test::~SavedGameConverterAbyss_Test()
{

}

void CheckAnimationFrameExistsAbyss(const uint16_t actorId, const DecorateStateId stateId, const uint16_t frameIndex)
{
    const auto actorIt = decorateAbyssAll.find(actorId);
    ASSERT_TRUE(actorIt != decorateAbyssAll.end());
    const auto stateIt = actorIt->second.states.find(stateId);
    ASSERT_TRUE(stateIt != actorIt->second.states.end());
    EXPECT_LE(frameIndex, stateIt->second.animation.size() - 1);
}

TEST(SavedGameConverterAbyss_Test, ConvertHidingZombie)
{
    constexpr uint16_t obclassZombie = 16;
    constexpr uint16_t s_zombie_inground[] = { 0x2394, 0x234E };

    for (uint8_t versionIndex = 0; versionIndex < 2; versionIndex++)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassZombie;
        dosObject.state16 = s_zombie_inground[versionIndex];

        const GameId gameId = (versionIndex == 0) ? GameId::CatacombAbyssv113 : GameId::CatacombAbyssv124;
        SavedGameConverterAbyss converter(gameId);
        EXPECT_EQ(actorIdMonsterZombie, converter.GetActorId(dosObject));
        EXPECT_EQ(StateIdHidden, converter.GetDecorateStateId(dosObject));
        EXPECT_EQ(0, converter.GetAnimationFrame(dosObject));

        CheckAnimationFrameExistsAbyss(actorIdMonsterZombie, StateIdHidden, 0u);
    }
}

TEST(SavedGameConverterAbyss_Test, ConvertRisingWetman)
{
    constexpr uint16_t obclassWet = 18;
    constexpr uint16_t s_wet_rise5[] = { 0x22CC, 0x2286 };

    for (uint8_t versionIndex = 0; versionIndex < 2; versionIndex++)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassWet;
        dosObject.state16 = s_wet_rise5[versionIndex];

        const GameId gameId = (versionIndex == 0) ? GameId::CatacombAbyssv113 : GameId::CatacombAbyssv124;
        SavedGameConverterAbyss converter(gameId);
        EXPECT_EQ(actorIdMonsterWetMan, converter.GetActorId(dosObject));
        EXPECT_EQ(StateIdRise, converter.GetDecorateStateId(dosObject));
        EXPECT_EQ(3u, converter.GetAnimationFrame(dosObject));

        CheckAnimationFrameExistsAbyss(actorIdMonsterWetMan, StateIdRise, 3u);
    }
}

TEST(SavedGameConverterAbyss_Test, ConvertGate)
{
    constexpr uint16_t obclassGate = 15;
    constexpr uint16_t s_obj_gate4[] = { 0x21FA, 0x21B4 };

    for (uint8_t versionIndex = 0; versionIndex < 2; versionIndex++)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassGate;
        dosObject.state16 = s_obj_gate4[versionIndex];
        dosObject.temp1 = 5;

        const GameId gameId = (versionIndex == 0) ? GameId::CatacombAbyssv113 : GameId::CatacombAbyssv124;
        SavedGameConverterAbyss converter(gameId);
        EXPECT_EQ(actorIdWarpPortal5, converter.GetActorId(dosObject));
        EXPECT_EQ(StateIdWaitForPickup, converter.GetDecorateStateId(dosObject));
        EXPECT_EQ(3u, converter.GetAnimationFrame(dosObject));

        CheckAnimationFrameExistsAbyss(actorIdWarpPortal5, StateIdWaitForPickup, 3u);
    }
}

TEST(SavedGameConverterAbyss_Test, ConvertScroll)
{
    constexpr uint16_t obclassBonus = 2;
    constexpr uint16_t s_scrollbonus[] = { 0x20D8, 0x2092 };

    for (uint8_t versionIndex = 0; versionIndex < 2; versionIndex++)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassBonus;
        dosObject.state16 = s_scrollbonus[versionIndex];
        dosObject.temp1 = 14;

        const GameId gameId = (versionIndex == 0) ? GameId::CatacombAbyssv113 : GameId::CatacombAbyssv124;
        SavedGameConverterAbyss converter(gameId);
        EXPECT_EQ(actorIdBonusScroll8, converter.GetActorId(dosObject));
        EXPECT_EQ(StateIdWaitForPickup, converter.GetDecorateStateId(dosObject));
        EXPECT_EQ(0u, converter.GetAnimationFrame(dosObject));

        CheckAnimationFrameExistsAbyss(actorIdBonusScroll8, StateIdWaitForPickup, 0u);
    }
}

TEST(SavedGameConverterAbyss_Test, ConvertTomb)
{
    constexpr uint16_t obclassSolid = 26;
    constexpr uint16_t s_tombs2[] = { 0x2182, 0x213C };

    for (uint8_t versionIndex = 0; versionIndex < 2; versionIndex++)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassSolid;
        dosObject.state16 = s_tombs2[versionIndex];

        const GameId gameId = (versionIndex == 0) ? GameId::CatacombAbyssv113 : GameId::CatacombAbyssv124;
        SavedGameConverterAbyss converter(gameId);
        EXPECT_EQ(actorIdTomb3, converter.GetActorId(dosObject));
        EXPECT_EQ(StateIdDecoration, converter.GetDecorateStateId(dosObject));
        EXPECT_EQ(0u, converter.GetAnimationFrame(dosObject));

        CheckAnimationFrameExistsAbyss(actorIdTomb3, StateIdDecoration, 0u);
    }
}

TEST(SavedGameConverterAbyss_Test, ConvertDyingSkeleton)
{
    constexpr uint16_t obclassInert = 12;
    constexpr uint16_t s_skel_die2[] = { 0x255E, 0x2518 };

    for (uint8_t versionIndex = 0; versionIndex < 2; versionIndex++)
    {
        SavedGameInDosFormat::ObjectInDosFormat dosObject;
        dosObject.obclass = obclassInert;
        dosObject.state16 = s_skel_die2[versionIndex];

        const GameId gameId = (versionIndex == 0) ? GameId::CatacombAbyssv113 : GameId::CatacombAbyssv124;
        SavedGameConverterAbyss converter(gameId);
        EXPECT_EQ(actorIdMonsterSkeleton, converter.GetActorId(dosObject));
        EXPECT_EQ(StateIdDying, converter.GetDecorateStateId(dosObject));
        EXPECT_EQ(1u, converter.GetAnimationFrame(dosObject));

        CheckAnimationFrameExistsAbyss(actorIdMonsterSkeleton, StateIdDying, 1u);
    }
}
