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
#include "SavedGameInDosFormat_Data.h"
#include "../Engine/SavedGamesInDosFormat.h"
#include "../Catacomb3D/SavedInGameInDosFormatConfigCatacomb3D.h"
#include <cstring>

class SavedGamesInDosFormat_Test : public ::testing::Test
{
};

TEST(SavedGamesInDosFormat_Test, LoadSavedGames)
{
    // Initially no games are saved
    SavedGamesInDosFormat savedGames(savedGameInDosFormatConfigCatacomb3D);
    EXPECT_EQ(0u, savedGames.GetSavedGameInDosFormat().size());

    // Add nullptr. No saved game will be loaded.
    savedGames.AddSavedGame(nullptr);
    EXPECT_EQ(0u, savedGames.GetSavedGameInDosFormat().size());

    // Add first saved game data
    FileChunk* fileChunk = new FileChunk(3166);
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacomb3D, 3166);
    savedGames.AddSavedGame(fileChunk);
    EXPECT_EQ(1u, savedGames.GetSavedGameInDosFormat().size());
    EXPECT_EQ("level3", savedGames.GetSavedGameInDosFormat().at(0)->GetName());

    // Add second saved game data
    savedGames.AddSavedGame(fileChunk);
    EXPECT_EQ(2u, savedGames.GetSavedGameInDosFormat().size());

    // Check that a saved game can be retrieved by specifying its name
    const SavedGameInDosFormat* const firstSavedGame = savedGames.GetSavedGameInDosFormat().at(0);
    ASSERT_TRUE(firstSavedGame != nullptr);
    const std::string nameOfFirstSavedGame = firstSavedGame->GetName();
    EXPECT_EQ(firstSavedGame, savedGames.GetSavedGameInDosFormat(nameOfFirstSavedGame));

    // Requesting a saved game with the wrong name results in a nullptr
    EXPECT_EQ(nullptr, savedGames.GetSavedGameInDosFormat("WrongName"));

    delete fileChunk;
}
