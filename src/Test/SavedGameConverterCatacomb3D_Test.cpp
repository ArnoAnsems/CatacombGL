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

void CheckAnimationFrameExists(const uint16_t actorId, const DecorateStateId stateId, const uint16_t frameIndex)
{
    const auto actorIt = decorateCatacomb3DAll.find(actorId);
    ASSERT_TRUE(actorIt != decorateCatacomb3DAll.end());
    const auto stateIt = actorIt->second.states.find(stateId);
    ASSERT_TRUE(stateIt != actorIt->second.states.end());
    EXPECT_LE(frameIndex, stateIt->second.animation.size() - 1);
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertAttackingOrc)
{
    const uint16_t obclassOrc = 3;
    const uint16_t s_trollattack3 = 0x190E;

    SavedGameInDosFormat::ObjectInDosFormat dosObject;
    dosObject.obclass = obclassOrc;
    dosObject.state16 = s_trollattack3;

    SavedGameConverterCatacomb3D converter;
    EXPECT_EQ(actorIdMonsterOrc, converter.GetActorId(dosObject));
    EXPECT_EQ(StateIdAttack, converter.GetDecorateStateId(dosObject));
    EXPECT_EQ(2u, converter.GetAnimationFrame(dosObject));

    CheckAnimationFrameExists(actorIdMonsterOrc, StateIdAttack, 2u);
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertDeadTroll)
{
    const uint16_t obclassInert = 12;
    const uint16_t s_trolldie3 = 0x1936;

    SavedGameInDosFormat::ObjectInDosFormat dosObject;
    dosObject.obclass = obclassInert;
    dosObject.state16 = s_trolldie3;

    SavedGameConverterCatacomb3D converter;
    EXPECT_EQ(actorIdMonsterTroll, converter.GetActorId(dosObject));
    EXPECT_EQ(StateIdDead, converter.GetDecorateStateId(dosObject));
    EXPECT_EQ(0, converter.GetAnimationFrame(dosObject));

    CheckAnimationFrameExists(actorIdMonsterTroll, StateIdDead, 0);
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertBoltBonus)
{
    const uint16_t obclassBonus = 2;
    const uint16_t s_boltbonus2 = 0x17CE;

    SavedGameInDosFormat::ObjectInDosFormat dosObject;
    dosObject.obclass = obclassBonus;
    dosObject.state16 = s_boltbonus2;

    SavedGameConverterCatacomb3D converter;
    EXPECT_EQ(actorIdBonusBolt, converter.GetActorId(dosObject));
    EXPECT_EQ(StateIdWaitForPickup, converter.GetDecorateStateId(dosObject));
    EXPECT_EQ(1u, converter.GetAnimationFrame(dosObject));

    CheckAnimationFrameExists(actorIdBonusBolt, StateIdWaitForPickup, 1u);
}

TEST(SavedGameConverterCatacomb3D_Test, ConvertPlayerShot)
{
    const uint16_t obclassPlayerShot = 9;
    const uint16_t s_pshot1 = 0x1776;

    SavedGameInDosFormat::ObjectInDosFormat dosObject;
    dosObject.obclass = obclassPlayerShot;
    dosObject.state16 = s_pshot1;

    SavedGameConverterCatacomb3D converter;
    EXPECT_EQ(actorIdProjectilePlayerShot, converter.GetActorId(dosObject));
    EXPECT_EQ(StateIdProjectileFly, converter.GetDecorateStateId(dosObject));
    EXPECT_EQ(0, converter.GetAnimationFrame(dosObject));

    CheckAnimationFrameExists(actorIdProjectilePlayerShot, StateIdProjectileFly, 0);
}