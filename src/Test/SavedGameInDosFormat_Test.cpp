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

#include <gtest/gtest.h>
#include "../Engine/SavedGameInDosFormat.h"
#include "../Catacomb3D/SavedInGameInDosFormatConfigCatacomb3D.h"
#include "../Abyss/SavedGameInDosFormatConfigAbyss.h"
#include "../Armageddon/SavesGameInDosFormatConfigArmageddon.h"
#include "../Apocalypse/SavedGameInDosFormatConfigApocalypse.h"
#include "SavedGameInDosFormat_Data.h"
#include <cstring>

class SavedGameInDosFormat_Test : public ::testing::Test
{
protected:
    static void CheckObjectIsPlayer(
        const SavedGameInDosFormat::ObjectInDosFormat& object)
    {
        EXPECT_EQ(object.obclass, 1);  // playerobj
        EXPECT_EQ(object.size, 26214);
    }

    static void CheckObjectRanges(
        const SavedGameInDosFormat::ObjectInDosFormat& object)
    {
        EXPECT_LE(object.active, 3);
        EXPECT_LE(object.obclass, 39); // Apocalypse has 39 obclass enums, which is the most
        EXPECT_LE(object.dir, 8); // nodir
        EXPECT_LE(object.speed, 10000); // Fastest object has a speed of 10000
        EXPECT_LE(object.hitpoints, 100); // Nemesis has 100 hitpoints, which is the most
        EXPECT_LE(object.angle, 359);
    }

    static void CheckObjectCoordinates(
        const SavedGameInDosFormat::ObjectInDosFormat& object,
        const uint16_t mapWidth,
        const uint16_t mapHeight)
    {
        // tilex and tiley should fit within the dimensions of the map
        EXPECT_GE(object.tilex, 1);
        EXPECT_GE(object.tiley, 1);
        EXPECT_LE(object.tilex, mapWidth - 1);
        EXPECT_LE(object.tiley, mapHeight - 1);

        // Coordinates x and y should be located near tilex and tiley
        constexpr int32_t tileScaleFactor = 65536;
        EXPECT_GE(object.x, (object.tilex - 0.5) * tileScaleFactor);
        EXPECT_LE(object.x, (object.tilex + 1.5) * tileScaleFactor);
        EXPECT_GE(object.y, (object.tiley - 0.5) * tileScaleFactor);
        EXPECT_LE(object.y, (object.tiley + 1.5) * tileScaleFactor);
    }
};

TEST_F(SavedGameInDosFormat_Test, LoadSavedGameCatacomb3D)
{
    constexpr uint16_t mapWidth = 40u;
    constexpr uint16_t mapHeight = 28u;
    FileChunk* fileChunk = new FileChunk(3166);
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacomb3D, 3166);
    SavedGameInDosFormat savedGame(fileChunk, savedGameInDosFormatConfigCatacomb3D);
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
    constexpr uint16_t planeSize = mapWidth * mapHeight * sizeof(uint16_t);
    EXPECT_EQ(savedGame.GetPlane0()->GetSize(), planeSize);
    EXPECT_EQ(savedGame.GetPlane2()->GetSize(), planeSize);
    EXPECT_EQ(savedGame.GetNumberOfObjects(), 24);

    const SavedGameInDosFormat::ObjectInDosFormat& firstObject = savedGame.GetObject(0);
    EXPECT_EQ(firstObject.state16, 6056);
    EXPECT_EQ(firstObject.shootable , 0);
    EXPECT_EQ(firstObject.tileObject, 0);
    EXPECT_EQ(firstObject.distance, 0);
    EXPECT_EQ(firstObject.viewx, 0);
    EXPECT_EQ(firstObject.viewheight, 0);
    EXPECT_EQ(firstObject.xl, 2264606);
    EXPECT_EQ(firstObject.xh, 2317034);
    EXPECT_EQ(firstObject.yl, 1528112);
    EXPECT_EQ(firstObject.yh, 1580540);
    EXPECT_EQ(firstObject.temp1, 0);
    EXPECT_EQ(firstObject.temp2, 0);
    EXPECT_EQ(firstObject.next, -21001);
    EXPECT_EQ(firstObject.prev, 0);
    const SavedGameInDosFormat::ObjectInDosFormat& secondObject = savedGame.GetObject(1);
    EXPECT_EQ(secondObject.state16, 6574);
    EXPECT_EQ(secondObject.shootable, 0);
    EXPECT_EQ(secondObject.tileObject, 0);
    EXPECT_EQ(secondObject.distance, 14336);
    EXPECT_EQ(secondObject.viewx, -28);
    EXPECT_EQ(secondObject.viewheight, 32000);
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
    EXPECT_EQ(lastObject.state16, 6264);
    EXPECT_EQ(lastObject.shootable, 0);
    EXPECT_EQ(lastObject.tileObject, 0);
    EXPECT_EQ(lastObject.distance, 0);
    EXPECT_EQ(lastObject.viewx, 0);
    EXPECT_EQ(lastObject.viewheight, 0);
    EXPECT_EQ(lastObject.size, 21845);
    EXPECT_EQ(lastObject.xl, 2435755);
    EXPECT_EQ(lastObject.xh, 2479445);
    EXPECT_EQ(lastObject.yl, 1714859);
    EXPECT_EQ(lastObject.yh, 1758549);
    EXPECT_EQ(lastObject.temp1, 0);
    EXPECT_EQ(lastObject.temp2, 0);
    EXPECT_EQ(lastObject.next, 0);
    EXPECT_EQ(lastObject.prev, -16785);

    CheckObjectIsPlayer(savedGame.GetObject(0));

    for (uint16_t i = 0; i < savedGame.GetNumberOfObjects(); i++)
    {
        const SavedGameInDosFormat::ObjectInDosFormat& object = savedGame.GetObject(i);
        CheckObjectRanges(object);
        CheckObjectCoordinates(object, mapWidth, mapHeight);
    }

    delete fileChunk;
}

TEST_F(SavedGameInDosFormat_Test, LoadInvalidSavedGameNullptr)
{
    SavedGameInDosFormat savedGame(nullptr, savedGameInDosFormatConfigCatacomb3D);
    EXPECT_FALSE(savedGame.Load());
    EXPECT_EQ(savedGame.GetErrorMessage(), "data is null");
}

TEST_F(SavedGameInDosFormat_Test, LoadInvalidSavedGameTooSmallForHeader)
{
    // Load saved game data that is too small to contain the header.
    FileChunk* fileChunk = new FileChunk(87);
    std::memset(fileChunk->GetChunk(), 0, 87);
    SavedGameInDosFormat savedGame(fileChunk, savedGameInDosFormatConfigCatacomb3D);
    EXPECT_FALSE(savedGame.Load());
    EXPECT_EQ(savedGame.GetErrorMessage(), "too small to contain header");
    delete fileChunk;
}

TEST_F(SavedGameInDosFormat_Test, LoadInvalidSavedGameCatacomb3DUnableToDecompressPlane0)
{
    FileChunk* fileChunk = new FileChunk(3166);
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacomb3D, 3166);
    // Set the compressed size of plane 0 too large
    *(uint16_t*)&(fileChunk->GetChunk()[84]) = 4000u;
    SavedGameInDosFormat savedGame(fileChunk, savedGameInDosFormatConfigCatacomb3D);
    EXPECT_FALSE(savedGame.Load());
    EXPECT_EQ(savedGame.GetErrorMessage(), "unable to decompress plane 0");
    delete fileChunk;
}

TEST_F(SavedGameInDosFormat_Test, LoadInvalidSavedGameCatacomb3DUnableToDecompressPlane2)
{
    FileChunk* fileChunk = new FileChunk(3166);
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacomb3D, 3166);
    // Set the compressed size of plane 2 too large
    *(uint16_t*)& (fileChunk->GetChunk()[1172]) = 3000u;
    SavedGameInDosFormat savedGame(fileChunk, savedGameInDosFormatConfigCatacomb3D);
    EXPECT_FALSE(savedGame.Load());
    EXPECT_EQ(savedGame.GetErrorMessage(), "unable to decompress plane 2");
    delete fileChunk;
}

TEST_F(SavedGameInDosFormat_Test, LoadInvalidSavedGameCatacomb3DNoObjectFound)
{
    // Copy the raw saved game data to a fileChunk, but without the object data.
    const uint32_t sizeOfRawSavedGameDataWithoutObjects = 3166 - (24 * 68);
    FileChunk* fileChunk = new FileChunk(sizeOfRawSavedGameDataWithoutObjects);
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacomb3D, sizeOfRawSavedGameDataWithoutObjects);
    SavedGameInDosFormat savedGame(fileChunk, savedGameInDosFormatConfigCatacomb3D);
    EXPECT_FALSE(savedGame.Load());
    EXPECT_EQ(savedGame.GetErrorMessage(), "no objects found");
    delete fileChunk;
}

TEST_F(SavedGameInDosFormat_Test, LoadSavedGameAbyss)
{
    constexpr uint16_t mapWidth = 40u;
    constexpr uint16_t mapHeight = 28u;
    FileChunk* fileChunk = new FileChunk(5504);
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacombAbyss, 5504);
    SavedGameInDosFormat savedGame(fileChunk, savedGameInDosFormatConfigAbyss);
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

    constexpr uint16_t planeSize = mapWidth * mapHeight * sizeof(uint16_t);
    EXPECT_EQ(savedGame.GetPlane0()->GetSize(), planeSize);
    EXPECT_EQ(savedGame.GetPlane2()->GetSize(), planeSize);

    EXPECT_EQ(savedGame.GetNumberOfObjects(), 54);

    const SavedGameInDosFormat::ObjectInDosFormat& firstObject = savedGame.GetObject(0);
    EXPECT_EQ(firstObject.flags, 0);
    EXPECT_EQ(firstObject.distance, 0);
    EXPECT_EQ(firstObject.viewx, 0);
    EXPECT_EQ(firstObject.viewheight, 0);
    EXPECT_EQ(firstObject.size, 26214);

    const SavedGameInDosFormat::ObjectInDosFormat& lastObject = savedGame.GetObject(53);
    EXPECT_EQ(lastObject.flags, 1);  // of_shootable
    EXPECT_EQ(lastObject.distance, 0);
    EXPECT_EQ(lastObject.viewx, 0);
    EXPECT_EQ(lastObject.viewheight, 0);
    EXPECT_EQ(lastObject.size, 17920);

    CheckObjectIsPlayer(savedGame.GetObject(0));

    for (uint16_t i = 0; i < savedGame.GetNumberOfObjects(); i++)
    {
        const SavedGameInDosFormat::ObjectInDosFormat& object = savedGame.GetObject(i);
        CheckObjectRanges(object);
        CheckObjectCoordinates(object, mapWidth, mapHeight);
    }

    delete fileChunk;
}

TEST_F(SavedGameInDosFormat_Test, LoadSavedGameArmageddon)
{
    constexpr uint16_t mapWidth = 40u;
    constexpr uint16_t mapHeight = 28u;
    FileChunk* fileChunk = new FileChunk(6769);
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacombArmageddon, 6769);
    SavedGameInDosFormat savedGame(fileChunk, savedGameInDosFormatConfigArmageddon);
    EXPECT_TRUE(savedGame.Load());

    EXPECT_EQ(savedGame.GetFreezeTime(), 0);
    EXPECT_EQ(savedGame.GetMapOn(), 2);
    EXPECT_EQ(savedGame.GetBolts(), 0);
    EXPECT_EQ(savedGame.GetNukes(), 0);
    EXPECT_EQ(savedGame.GetPotions(), 0);
    EXPECT_EQ(savedGame.GetKeys(0), 0); // No red key
    EXPECT_EQ(savedGame.GetKeys(1), 1); // One yellow key
    EXPECT_EQ(savedGame.GetKeys(2), 0); // No green key
    EXPECT_EQ(savedGame.GetKeys(3), 0); // No blue key
    EXPECT_EQ(savedGame.GetScrolls(0), 0);
    EXPECT_EQ(savedGame.GetScrolls(1), 0);
    EXPECT_EQ(savedGame.GetBody(), 96); // health is at 96%
    EXPECT_EQ(savedGame.GetEasyModeOn(), true);
    EXPECT_EQ(savedGame.GetSkyColor(), 45081u); // 16-bit memory location of the sky color
    EXPECT_EQ(savedGame.GetGroundColor(), 45083u); // 16-bit memory location of the ground color
    EXPECT_EQ(savedGame.GetMapWidth(), 0);
    EXPECT_EQ(savedGame.GetMapHeight(), 0);

    constexpr uint16_t planeSize = mapWidth * mapHeight * sizeof(uint16_t);
    EXPECT_EQ(savedGame.GetPlane0()->GetSize(), planeSize);
    EXPECT_EQ(savedGame.GetPlane2()->GetSize(), planeSize);

    EXPECT_EQ(savedGame.GetNumberOfObjects(), 70);

    const SavedGameInDosFormat::ObjectInDosFormat& firstObject = savedGame.GetObject(0);
    EXPECT_EQ(firstObject.flags, 0);
    EXPECT_EQ(firstObject.distance, 0);
    EXPECT_EQ(firstObject.dir, 0); // north
    EXPECT_EQ(firstObject.viewx, 0);
    EXPECT_EQ(firstObject.viewheight, 0);
    EXPECT_EQ(firstObject.size, 26214);
    EXPECT_EQ(firstObject.state32, 0x1BFA0000);  // 0x1A580000 + 1A20000

    CheckObjectIsPlayer(savedGame.GetObject(0));

    for (uint16_t i = 0; i < savedGame.GetNumberOfObjects(); i++)
    {
        const SavedGameInDosFormat::ObjectInDosFormat& object = savedGame.GetObject(i);
        CheckObjectRanges(object);
        CheckObjectCoordinates(object, mapWidth, mapHeight);
    }

    delete fileChunk;
}

TEST_F(SavedGameInDosFormat_Test, LoadSavedGameApocalypse)
{
    constexpr uint16_t mapWidth = 40u;
    constexpr uint16_t mapHeight = 28u;
    FileChunk* fileChunk = new FileChunk(6962);
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacombApocalypse, 6962);
    SavedGameInDosFormat savedGame(fileChunk, savedGameInDosFormatConfigApocalypse);
    EXPECT_TRUE(savedGame.Load());

    EXPECT_EQ(savedGame.GetFreezeTime(), 0);
    EXPECT_EQ(savedGame.GetMapOn(), 16);
    EXPECT_EQ(savedGame.GetBolts(), 0);
    EXPECT_EQ(savedGame.GetNukes(), 0);
    EXPECT_EQ(savedGame.GetPotions(), 0);
    EXPECT_EQ(savedGame.GetKeys(0), 1);
    EXPECT_EQ(savedGame.GetKeys(1), 0);
    EXPECT_EQ(savedGame.GetKeys(2), 0);
    EXPECT_EQ(savedGame.GetKeys(3), 0);
    EXPECT_EQ(savedGame.GetScrolls(0), 0);
    EXPECT_EQ(savedGame.GetScrolls(1), 0);
    EXPECT_EQ(savedGame.GetBody(), 15);
    EXPECT_EQ(savedGame.GetEasyModeOn(), true);
    EXPECT_EQ(savedGame.GetSkyColor(), 47003u); // 16-bit memory location of the sky color
    EXPECT_EQ(savedGame.GetGroundColor(), 47005u); // 16-bit memory location of the ground color
    EXPECT_EQ(savedGame.GetMapWidth(), 0);
    EXPECT_EQ(savedGame.GetMapHeight(), 0);

    constexpr uint16_t planeSize = mapWidth * mapHeight * sizeof(uint16_t);
    EXPECT_EQ(savedGame.GetPlane0()->GetSize(), planeSize);
    EXPECT_EQ(savedGame.GetPlane2()->GetSize(), planeSize);

    EXPECT_EQ(savedGame.GetNumberOfObjects(), 77);

    const SavedGameInDosFormat::ObjectInDosFormat& firstObject = savedGame.GetObject(0);
    EXPECT_EQ(firstObject.flags, 0);
    EXPECT_EQ(firstObject.distance, 0);
    EXPECT_EQ(firstObject.viewx, 0);
    EXPECT_EQ(firstObject.viewheight, 0);
    EXPECT_EQ(firstObject.size, 26214);
    EXPECT_EQ(firstObject.state32, 0x1C110000); // 0x1A6F0000 + 1A20000

    CheckObjectIsPlayer(savedGame.GetObject(0));

    for (uint16_t i = 0; i < savedGame.GetNumberOfObjects(); i++)
    {
        const SavedGameInDosFormat::ObjectInDosFormat& object = savedGame.GetObject(i);
        CheckObjectRanges(object);
        CheckObjectCoordinates(object, mapWidth, mapHeight);
    }

    delete fileChunk;
}
