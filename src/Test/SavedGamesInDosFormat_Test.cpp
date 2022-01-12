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

#include "SavedGamesInDosFormat_Test.h"
#include "SavedGameInDosFormat_Data.h"
#include "../Engine/SavedGamesInDosFormat.h"
#include "../Catacomb3D/SavedGameConverterCatacomb3D.h"

SavedGamesInDosFormat_Test::SavedGamesInDosFormat_Test()
{

}

SavedGamesInDosFormat_Test::~SavedGamesInDosFormat_Test()
{

}

TEST(SavedGamesInDosFormat_Test, LoadSavedGames)
{
    // Initially no games are saved
    SavedGameConverterCatacomb3D converter;
    SavedGamesInDosFormat savedGames(converter.GetDosFormatConfig());
    EXPECT_EQ(0, savedGames.GetSavedGameInDosFormat().size());

    // Add nullptr. No saved game will be loaded.
    savedGames.AddSavedGame(nullptr);
    EXPECT_EQ(0, savedGames.GetSavedGameInDosFormat().size());

    // Add first saved game data
    FileChunk* fileChunk = new FileChunk(3166);
    std::memcpy(fileChunk->GetChunk(), rawSavedGameDataCatacomb3D, 3166);
    savedGames.AddSavedGame(fileChunk);
    EXPECT_EQ(1, savedGames.GetSavedGameInDosFormat().size());
    EXPECT_EQ("level3", savedGames.GetSavedGameInDosFormat().at(0)->GetName());

    // Add second saved game data
    savedGames.AddSavedGame(fileChunk);
    EXPECT_EQ(2, savedGames.GetSavedGameInDosFormat().size());
}
