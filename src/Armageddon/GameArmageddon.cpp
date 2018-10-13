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
#include "EgaGraphArmageddon.h"
#include "GameMapsArmageddon.h"
#include "IntroViewArmageddon.h"
#include "AudioRepositoryArmageddon.h"
#include "DecorateAll.h"
#include "..\Engine\IRenderer.h"

static const std::string ArmageddonName = "Catacomb Armageddon v1.02";

GameArmageddon::GameArmageddon(const std::string gamePath, IRenderer& renderer) :
    m_gameId (3),
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
    Actor* const playerState = level->GetPlayerActor();

    for (uint16_t y = 0; y < level->GetLevelHeight(); y++)
    {
        for (uint16_t x = 0; x < level->GetLevelWidth(); x++)
        {
            const uint16_t tile = level->GetFloorTile(x, y);
            switch (tile)
            {
            case 1:
            case 2:
            case 3:
            case 4:
            {
                playerState->SetX(x + 0.5f);
                playerState->SetY(y + 0.5f);
                playerState->SetAngle((tile - 1) * 90.0f);
                break;
            }
            default:
                break;
            }
        }
    }
}

GameMaps* GameArmageddon::GetGameMaps()
{
    if (m_gameMaps == NULL)
    {
        m_gameMaps = new GameMaps(gameMapsArmageddon, m_gamePath);
    }

    return m_gameMaps;
}

EgaGraph* GameArmageddon::GetEgaGraph()
{
    if (m_egaGraph == NULL)
    {
        m_egaGraph = new EgaGraph(egaGraphArmageddon, m_gamePath, m_renderer);
    }

    return m_egaGraph;
}

AudioRepository* GameArmageddon::GetAudioRepository()
{
    if (m_audioRepository == NULL)
    {
        m_audioRepository = new AudioRepository(audioRepositoryArmageddon, m_gamePath);
    }

    return m_audioRepository;
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
    if (m_introView == NULL)
    {
        m_introView = new IntroViewArmageddon(m_renderer, m_gamePath);
    }

    return m_introView;
}

const std::map<uint16_t, const DecorateActor>& GameArmageddon::GetDecorateActors() const
{
    return decorateArmageddonAll;
}

const std::string& GameArmageddon::GetName() const
{
    return ArmageddonName;
}

const uint8_t GameArmageddon::GetId() const
{
    return m_gameId;
}

void GameArmageddon::DrawStatusBar(const int16_t health, const std::string& locationMessage, const PlayerInventory& playerInventory)
{
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(egaGraphicsArmageddon::STATUSPIC), 0, 120);

    DrawHealth(health);
    DrawKeys(playerInventory);
    DrawBonus(playerInventory);
    DrawGems(playerInventory);

    m_renderer.RenderTextCentered(locationMessage.c_str(), GetEgaGraph()->GetFont(3), EgaBrightYellow, 160, 121);
}

void GameArmageddon::DrawHealth(const int16_t health)
{
    const uint16_t percentage = (uint16_t)health;

    m_renderer.RenderNumber(percentage, GetEgaGraph()->GetFont(3), 3, EgaBrightYellow, 90, 176);

    uint16_t picnum;
    if (percentage > 75)
    {
        picnum = FACE1PIC;
    }
    else if (percentage > 50)
    {
        picnum = FACE2PIC;
    }
    else if (percentage > 25)
    {
        picnum = FACE3PIC;
    }
    else if (percentage)
    {
        picnum = FACE4PIC;
    }
    else
    {
        picnum = FACE5PIC;
    }

    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(picnum), 80, 134);
}

void GameArmageddon::DrawKeys(const PlayerInventory& playerInventory)
{
    m_renderer.RenderNumber(playerInventory.GetKeys(RedKey), GetEgaGraph()->GetFont(3), 2, EgaBrightYellow, 192, 149);
    m_renderer.RenderNumber(playerInventory.GetKeys(YellowKey), GetEgaGraph()->GetFont(3), 2, EgaBrightYellow, 216, 176);
    m_renderer.RenderNumber(playerInventory.GetKeys(GreenKey), GetEgaGraph()->GetFont(3), 2, EgaBrightYellow, 216, 149);
    m_renderer.RenderNumber(playerInventory.GetKeys(BlueKey), GetEgaGraph()->GetFont(3), 2, EgaBrightYellow, 192, 176);
}

void GameArmageddon::DrawBonus(const PlayerInventory& playerInventory)
{
    m_renderer.RenderNumber(playerInventory.GetBolts(), GetEgaGraph()->GetFont(3), 2, EgaBrightYellow, 158, 137);
    m_renderer.RenderNumber(playerInventory.GetNukes(), GetEgaGraph()->GetFont(3), 2, EgaBrightYellow, 158, 155);
    m_renderer.RenderNumber(playerInventory.GetPotions(), GetEgaGraph()->GetFont(3), 2, EgaBrightYellow, 158, 173);
}

void GameArmageddon::DrawGems(const PlayerInventory& playerInventory)
{
    for (uint8_t i = 0; i < 5; i++)
    {
        if (playerInventory.GetGem(i))
        {
            const uint16_t picNum = RADAR_RGEMPIC + i;
            m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(picNum), 256 + (i * 8), 173);
        }
    }
}