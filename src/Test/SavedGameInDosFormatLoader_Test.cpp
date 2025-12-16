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

#include "SavedGameInDosFormatLoader_Test.h"
#include "SavedGameInDosFormat_Data.h"

#include "../Engine/SavedGameInDosFormatLoader.h"
#include "../Engine/SavedGameInDosFormat.h"
#include "../Engine/Actor.h"
#include "../Catacomb3D/SavedGameConverterCatacomb3D.h"
#include "../Catacomb3D/SavedInGameInDosFormatConfigCatacomb3D.h"
#include "../Catacomb3D/DecorateAll.h"

#include <cstring>

SavedGameInDosFormatLoader_Test::SavedGameInDosFormatLoader_Test()
{
}

SavedGameInDosFormatLoader_Test::~SavedGameInDosFormatLoader_Test()
{
}

TEST(SavedGameInDosFormatLoader_Test, LoadPlayerActor)
{
    FileChunk* fileChunk = new FileChunk(3166);
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacomb3D, 3166);
    SavedGameInDosFormat savedGame(fileChunk, savedGameInDosFormatConfigCatacomb3D);
    EXPECT_TRUE(savedGame.Load());

    SavedGameConverterCatacomb3D converter(GameId::Catacomb3Dv122);
    SavedGameInDosFormatLoader loader(savedGame, converter, decorateCatacomb3DAll);

    Actor* playerActor = loader.LoadPlayerActor();
    ASSERT_TRUE(playerActor != nullptr);

    const SavedGameInDosFormat::ObjectInDosFormat& firstObject = savedGame.GetObject(0);
    EXPECT_EQ(firstObject.angle, playerActor->GetAngle());
    EXPECT_EQ(savedGame.GetBody(), playerActor->GetHealth());
    EXPECT_EQ(actorIdPlayer, playerActor->GetDecorateActor().id);
    EXPECT_EQ((float)firstObject.x / 65536.0f, playerActor->GetX());
    EXPECT_EQ((float)firstObject.y / 65536.0f, playerActor->GetY());
    EXPECT_EQ(firstObject.tilex, playerActor->GetTileX());
    EXPECT_EQ(firstObject.tiley, playerActor->GetTileY());

    delete fileChunk;
    delete playerActor;
}
