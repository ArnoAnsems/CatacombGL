// Copyright (C) 2018 Arno Ansems
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

#include "GameArmageddon.h"
#include "..\Engine\IRenderer.h"

static const std::string ArmageddonName = "Catacomb Armageddon";

GameArmageddon::GameArmageddon(const std::string gamePath, IRenderer& renderer) :
    m_gameId (0),
    m_gamePath (gamePath),
    m_renderer (renderer),
    m_introView (NULL)
{
    m_gameMaps = NULL;
    m_egaGraph = NULL;
    m_audioRepository = NULL;
    m_audioPlayer = NULL;
}

GameArmageddon::~GameArmageddon()
{
    if (m_gameMaps != NULL)
    {
        delete m_gameMaps;
    }
    if (m_egaGraph != NULL)
    {
        delete m_egaGraph;
    }
    if (m_audioRepository != NULL)
    {
        delete m_audioRepository;
    }
    if (m_audioPlayer != NULL)
    {
        delete m_audioPlayer;
    }
    if (m_introView != NULL)
    {
        delete m_introView;
    }
}

void GameArmageddon::SpawnActors(Level* level, const DifficultyLevel difficultyLevel)
{

}

void GameArmageddon::DrawStatusBar(const int16_t health, const std::string& locationMessage, const PlayerInventory& playerInventory)
{
}

GameMaps* GameArmageddon::GetGameMaps()
{
    return NULL;
}

EgaGraph* GameArmageddon::GetEgaGraph()
{
    return NULL;
}

AudioRepository* GameArmageddon::GetAudioRepository()
{
    return NULL;
}

AudioPlayer* GameArmageddon::GetAudioPlayer()
{
    if (m_audioPlayer == NULL)
    {
        m_audioPlayer = new AudioPlayer(GetAudioRepository());
    }

    return m_audioPlayer;
}

IIntroView* GameArmageddon::GetIntroView()
{
    return m_introView;
}

const std::map<uint16_t, const DecorateActor>& GameArmageddon::GetDecorateActors() const
{
    return {};
}

const std::string& GameArmageddon::GetName() const
{
    return ArmageddonName;
}

const uint8_t GameArmageddon::GetId() const
{
    return m_gameId;
}