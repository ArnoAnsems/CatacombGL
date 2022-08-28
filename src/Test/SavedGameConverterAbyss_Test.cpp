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

        const uint8_t version = (versionIndex == 0) ? 1 : 2;
        SavedGameConverterAbyss converter(version);
        EXPECT_EQ(actorIdMonsterZombie, converter.GetActorId(dosObject));

        CheckAnimationFrameExistsAbyss(actorIdMonsterZombie, StateIdHidden, 0u);
    }
}
