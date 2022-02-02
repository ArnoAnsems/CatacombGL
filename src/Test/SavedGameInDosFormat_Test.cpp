// Copyright (C) 2021 Arno Ansems
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

#include "SavedGameInDosFormat_Test.h"
#include "../Engine/SavedGameInDosFormat.h"
#include "../Catacomb3D/SavedGameConverterCatacomb3D.h"
#include "../Abyss/SavedGameConverterAbyss.h"
#include "SavedGameInDosFormat_Data.h"

SavedGameInDosFormat_Test::SavedGameInDosFormat_Test()
{

}

SavedGameInDosFormat_Test::~SavedGameInDosFormat_Test()
{

}

TEST(SavedGameInDosFormat_Test, LoadSavedGameCatacomb3D)
{
    FileChunk* fileChunk = new FileChunk(3166);
    SavedGameConverterCatacomb3D converter;
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacomb3D, 3166);
    SavedGameInDosFormat savedGame(fileChunk, converter.GetDosFormatConfig());
    EXPECT_TRUE(savedGame.Load());

    EXPECT_EQ(savedGame.GetSignature(), "C3D");
    EXPECT_TRUE(savedGame.GetPresent());
    EXPECT_EQ(savedGame.GetName(), "level3");
    EXPECT_EQ(savedGame.GetDifficulty(), 2);
    EXPECT_EQ(savedGame.GetMapOn(), 2);
    EXPECT_EQ(savedGame.GetBolts(), 5);
    EXPECT_EQ(savedGame.GetNukes(), 2);
    EXPECT_EQ(savedGame.GetPotions(), 21);
    EXPECT_EQ(savedGame.GetKeys(0), 0);
    EXPECT_EQ(savedGame.GetKeys(1), 0);
    EXPECT_EQ(savedGame.GetKeys(2), 0);
    EXPECT_EQ(savedGame.GetKeys(3), 1);
    EXPECT_EQ(savedGame.GetScrolls(0), 0);
    EXPECT_EQ(savedGame.GetScrolls(7), 0);
    EXPECT_EQ(savedGame.GetScore(), 12700);
    EXPECT_EQ(savedGame.GetBody(), 32);
    EXPECT_EQ(savedGame.GetShotpower(), 0);
    constexpr uint16_t planeSize = 40u * 28u * sizeof(uint16_t);
    EXPECT_EQ(savedGame.GetPlane0()->GetSize(), planeSize);
    EXPECT_EQ(savedGame.GetPlane2()->GetSize(), planeSize);
    EXPECT_EQ(savedGame.GetNumberOfObjects(), 24);

    const SavedGameInDosFormat::ObjectInDosFormat& firstObject = savedGame.GetObject(0);
    EXPECT_EQ(firstObject.active, 1);
    EXPECT_EQ(firstObject.obclass, 1);  // playerobj
    EXPECT_EQ(firstObject.stateOffset, 6056);
    EXPECT_EQ(firstObject.shootable , 0);
    EXPECT_EQ(firstObject.tileObject, 0);
    EXPECT_EQ(firstObject.distance, 0);
    EXPECT_EQ(firstObject.dir, 0);
    EXPECT_EQ(firstObject.x, 2290820);
    EXPECT_EQ(firstObject.y, 1554326);
    EXPECT_EQ(firstObject.tilex, 34);
    EXPECT_EQ(firstObject.tiley, 23);
    EXPECT_EQ(firstObject.viewx, 0);
    EXPECT_EQ(firstObject.viewheight, 0);
    EXPECT_EQ(firstObject.angle, 208);
    EXPECT_EQ(firstObject.hitpoints, 0);
    EXPECT_EQ(firstObject.speed, 0);
    EXPECT_EQ(firstObject.size, 26214);
    EXPECT_EQ(firstObject.xl, 2264606);
    EXPECT_EQ(firstObject.xh, 2317034);
    EXPECT_EQ(firstObject.yl, 1528112);
    EXPECT_EQ(firstObject.yh, 1580540);
    EXPECT_EQ(firstObject.temp1, 0);
    EXPECT_EQ(firstObject.temp2, 0);
    EXPECT_EQ(firstObject.next, -21001);
    EXPECT_EQ(firstObject.prev, 0);
    const SavedGameInDosFormat::ObjectInDosFormat& secondObject = savedGame.GetObject(1);
    EXPECT_EQ(secondObject.active, 1);
    EXPECT_EQ(secondObject.obclass, 12);  // inertobj
    EXPECT_EQ(secondObject.stateOffset, 6574);
    EXPECT_EQ(secondObject.shootable, 0);
    EXPECT_EQ(secondObject.tileObject, 0);
    EXPECT_EQ(secondObject.distance, 14336);
    EXPECT_EQ(secondObject.dir, 3);  // west
    EXPECT_EQ(secondObject.x, 964608);
    EXPECT_EQ(secondObject.y, 163840);
    EXPECT_EQ(secondObject.tilex, 14);
    EXPECT_EQ(secondObject.tiley, 2);
    EXPECT_EQ(secondObject.viewx, -28);
    EXPECT_EQ(secondObject.viewheight, 32000);
    EXPECT_EQ(secondObject.angle, 0);
    EXPECT_EQ(secondObject.hitpoints, 0);
    EXPECT_EQ(secondObject.speed, 1536);
    EXPECT_EQ(secondObject.size, 16384);
    EXPECT_EQ(secondObject.xl, 948224);
    EXPECT_EQ(secondObject.xh, 980992);
    EXPECT_EQ(secondObject.yl, 147456);
    EXPECT_EQ(secondObject.yh, 180224);
    EXPECT_EQ(secondObject.temp1, 0);
    EXPECT_EQ(secondObject.temp2, 0);
    EXPECT_EQ(secondObject.next, -20933);
    EXPECT_EQ(secondObject.prev, -21069);
    const SavedGameInDosFormat::ObjectInDosFormat& lastObject = savedGame.GetObject(23);
    EXPECT_EQ(firstObject.active, 1);
    EXPECT_EQ(lastObject.obclass, 15);  // gateobj
    EXPECT_EQ(lastObject.stateOffset, 6264);
    EXPECT_EQ(lastObject.shootable, 0);
    EXPECT_EQ(lastObject.tileObject, 0);
    EXPECT_EQ(lastObject.distance, 0);
    EXPECT_EQ(lastObject.dir, 8); // nodir
    EXPECT_EQ(lastObject.x, 2457600);
    EXPECT_EQ(lastObject.y, 1736704);
    EXPECT_EQ(lastObject.tilex, 37);
    EXPECT_EQ(lastObject.tiley, 26);
    EXPECT_EQ(lastObject.viewx, 0);
    EXPECT_EQ(lastObject.viewheight, 0);
    EXPECT_EQ(lastObject.angle, 0);
    EXPECT_EQ(lastObject.hitpoints, 0);
    EXPECT_EQ(lastObject.speed, 0);
    EXPECT_EQ(lastObject.size, 21845);
    EXPECT_EQ(lastObject.xl, 2435755);
    EXPECT_EQ(lastObject.xh, 2479445);
    EXPECT_EQ(lastObject.yl, 1714859);
    EXPECT_EQ(lastObject.yh, 1758549);
    EXPECT_EQ(lastObject.temp1, 0);
    EXPECT_EQ(lastObject.temp2, 0);
    EXPECT_EQ(lastObject.next, 0);
    EXPECT_EQ(lastObject.prev, -16785);
}

TEST(SavedGamesInDosFormat_Test, LoadInvalidSavedGameNullptr)
{
    SavedGameConverterCatacomb3D converter;
    SavedGameInDosFormat savedGame(nullptr, converter.GetDosFormatConfig());
    EXPECT_FALSE(savedGame.Load());
    EXPECT_EQ(savedGame.GetErrorMessage(), "data is null");
}

TEST(SavedGamesInDosFormat_Test, LoadInvalidSavedGameTooSmallForHeader)
{
    // Load saved game data that is too small to contain the header.
    FileChunk* fileChunk = new FileChunk(87);
    SavedGameConverterCatacomb3D converter;
    std::memset(fileChunk->GetChunk(), 0, 87);
    SavedGameInDosFormat savedGame(fileChunk, converter.GetDosFormatConfig());
    EXPECT_FALSE(savedGame.Load());
    EXPECT_EQ(savedGame.GetErrorMessage(), "too small to contain header");
}

TEST(SavedGamesInDosFormat_Test, LoadInvalidSavedGameCatacomb3DUnableToDecompressPlane0)
{
    FileChunk* fileChunk = new FileChunk(3166);
    SavedGameConverterCatacomb3D converter;
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacomb3D, 3166);
    // Set the compressed size of plane 0 too large
    *(uint16_t*)&(fileChunk->GetChunk()[84]) = 4000u;
    SavedGameInDosFormat savedGame(fileChunk, converter.GetDosFormatConfig());
    EXPECT_FALSE(savedGame.Load());
    EXPECT_EQ(savedGame.GetErrorMessage(), "unable to decompress plane 0");
}

TEST(SavedGamesInDosFormat_Test, LoadInvalidSavedGameCatacomb3DUnableToDecompressPlane2)
{
    FileChunk* fileChunk = new FileChunk(3166);
    SavedGameConverterCatacomb3D converter;
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacomb3D, 3166);
    // Set the compressed size of plane 2 too large
    *(uint16_t*)& (fileChunk->GetChunk()[1172]) = 3000u;
    SavedGameInDosFormat savedGame(fileChunk, converter.GetDosFormatConfig());
    EXPECT_FALSE(savedGame.Load());
    EXPECT_EQ(savedGame.GetErrorMessage(), "unable to decompress plane 2");
}

TEST(SavedGamesInDosFormat_Test, LoadInvalidSavedGameCatacomb3DNoObjectFound)
{
    // Copy the raw saved game data to a fileChunk, but without the object data.
    const uint32_t sizeOfRawSavedGameDataWithoutObjects = 3166 - (24 * 68);
    FileChunk* fileChunk = new FileChunk(sizeOfRawSavedGameDataWithoutObjects);
    SavedGameConverterCatacomb3D converter;
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacomb3D, sizeOfRawSavedGameDataWithoutObjects);
    SavedGameInDosFormat savedGame(fileChunk, converter.GetDosFormatConfig());
    EXPECT_FALSE(savedGame.Load());
    EXPECT_EQ(savedGame.GetErrorMessage(), "no objects found");
}

TEST(SavedGameInDosFormat_Test, LoadSavedGameAbyss)
{
    FileChunk* fileChunk = new FileChunk(5504);
    SavedGameConverterAbyss converter;
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacombAbyss, 5504);
    SavedGameInDosFormat savedGame(fileChunk, converter.GetDosFormatConfig());
    EXPECT_TRUE(savedGame.Load());

    EXPECT_EQ(savedGame.GetFreezeTime(), 0);
    EXPECT_EQ(savedGame.GetMapOn(), 0);
    EXPECT_EQ(savedGame.GetBolts(), 4);
    EXPECT_EQ(savedGame.GetNukes(), 3);
    EXPECT_EQ(savedGame.GetPotions(), 2);
    EXPECT_EQ(savedGame.GetKeys(0), 1);
    EXPECT_EQ(savedGame.GetKeys(1), 0);
    EXPECT_EQ(savedGame.GetKeys(2), 1);
    EXPECT_EQ(savedGame.GetKeys(3), 0);
    EXPECT_EQ(savedGame.GetScrolls(0), 1);
    EXPECT_EQ(savedGame.GetScrolls(1), 0);
    EXPECT_EQ(savedGame.GetBody(), 36);
    EXPECT_EQ(savedGame.GetEasyModeOn(), true);

    constexpr uint16_t planeSize = 40u * 28u * sizeof(uint16_t);
    EXPECT_EQ(savedGame.GetPlane0()->GetSize(), planeSize);
    EXPECT_EQ(savedGame.GetPlane2()->GetSize(), planeSize);

    EXPECT_EQ(savedGame.GetNumberOfObjects(), 54);

    const SavedGameInDosFormat::ObjectInDosFormat& firstObject = savedGame.GetObject(0);
    EXPECT_EQ(firstObject.active, 3);  // always
    EXPECT_EQ(firstObject.obclass, 1);  // playerobj
    EXPECT_EQ(firstObject.flags, 0);
    EXPECT_EQ(firstObject.distance, 0);
    EXPECT_EQ(firstObject.dir, 0); // north
    EXPECT_EQ(firstObject.x, 2529484);
    EXPECT_EQ(firstObject.y, 117834);
    EXPECT_EQ(firstObject.tilex, 38);
    EXPECT_EQ(firstObject.tiley, 1);
    EXPECT_EQ(firstObject.viewx, 0);
    EXPECT_EQ(firstObject.viewheight, 0);
    EXPECT_EQ(firstObject.angle, 197);
    EXPECT_EQ(firstObject.hitpoints, 0);
    EXPECT_EQ(firstObject.speed, 0);
    EXPECT_EQ(firstObject.size, 26214);

    const SavedGameInDosFormat::ObjectInDosFormat& lastObject = savedGame.GetObject(53);
    EXPECT_EQ(lastObject.active, 1);  // noalways
    EXPECT_EQ(lastObject.obclass, 4);  // batobj
    EXPECT_EQ(lastObject.flags, 1);  // of_shootable
    EXPECT_EQ(lastObject.distance, 0);
    EXPECT_EQ(lastObject.dir, 8); // nodir
    EXPECT_EQ(lastObject.x, 2326528);
    EXPECT_EQ(lastObject.y, 1671168);
    EXPECT_EQ(lastObject.tilex, 35);
    EXPECT_EQ(lastObject.tiley, 25);
    EXPECT_EQ(lastObject.viewx, 0);
    EXPECT_EQ(lastObject.viewheight, 0);
    EXPECT_EQ(lastObject.angle, 0);
    EXPECT_EQ(lastObject.hitpoints, 1);
    EXPECT_EQ(lastObject.speed, 2000);
    EXPECT_EQ(lastObject.size, 17920);
}
