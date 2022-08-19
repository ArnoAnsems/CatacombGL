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

TEST(SavedGameConverterCatacomb3D_Test, ConvertOrc)
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
}