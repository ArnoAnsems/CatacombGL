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

#include "SavedGamesInDosFormat.h"

SavedGamesInDosFormat::SavedGamesInDosFormat(const SavedGameInDosFormatConfig& config) :
    m_config(config),
    m_savedGames()
{

}

SavedGamesInDosFormat::~SavedGamesInDosFormat()
{
    for each (SavedGameInDosFormat* savedGame in m_savedGames)
    {
        delete savedGame;
    }
}

void SavedGamesInDosFormat::AddSavedGame(const FileChunk* fileChunk)
{
    if (fileChunk != nullptr)
    {
        SavedGameInDosFormat* savedGame = new SavedGameInDosFormat(fileChunk, m_config);
        const bool savedGameIsLoaded = savedGame->Load();

        if (savedGameIsLoaded)
        {
            m_savedGames.push_back(savedGame);
        }
        else
        {
            delete savedGame;
        }
    }
}

void SavedGamesInDosFormat::AddSavedGame(const FileChunk* fileChunk, const std::string& name)
{
    if (fileChunk != nullptr)
    {
        SavedGameInDosFormat* savedGame = new SavedGameInDosFormat(fileChunk, m_config, name);
        const bool savedGameIsLoaded = savedGame->Load();

        if (savedGameIsLoaded)
        {
            m_savedGames.push_back(savedGame);
        }
        else
        {
            delete savedGame;
        }
    }
}

const std::vector<SavedGameInDosFormat*>& SavedGamesInDosFormat::GetSavedGameInDosFormat() const
{
    return m_savedGames;
}

const SavedGameInDosFormat* const SavedGamesInDosFormat::GetSavedGameInDosFormat(const std::string name) const
{
    SavedGameInDosFormat* matchingSavedGame = nullptr;
    for each (SavedGameInDosFormat * savedGame in m_savedGames)
    {
        if (savedGame->GetName() == name)
        {
            matchingSavedGame = savedGame;
            break;
        }
    }

    return matchingSavedGame;
}