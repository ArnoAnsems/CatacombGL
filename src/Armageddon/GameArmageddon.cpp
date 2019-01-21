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
    m_introView (NULL),
    m_zombie_base_delay(0)
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
    Actor** actors = level->GetBlockingActors();
    Actor* const playerState = new Actor(0, 0, 0, decoratePlayer);
    level->SetPlayerActor(playerState);

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
            case 5:
            {
                Actor* bonusActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateBolt);
                actors[(y * level->GetLevelWidth()) + x] = bonusActor;
                break;
            }
            case 6:
            {
                Actor* bonusActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateNuke);
                actors[(y * level->GetLevelWidth()) + x] = bonusActor;
                break;
            }
            case 7:

            {
                Actor* bonusActor = new Actor(x + 0.5f, y + 0.5f, 0, decoratePotion);
                actors[(y * level->GetLevelWidth()) + x] = bonusActor;
                break;
            }
            case 8:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateKeyRed);
                break;
            case 9:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateKeyYellow);
                break;
            case 10:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateKeyGreen);
                break;
            case 11:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateKeyBlue);
                break;
            case 20:
            {
                Actor* redDemonActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateRedDemon);
                actors[(y * level->GetLevelWidth()) + x] = redDemonActor;
                break;
            }
            case 21:
            {
                int16_t viper_delay;
                unsigned int tile = level->GetFloorTile(x, y + 1);
                if (tile != 0)
                    viper_delay = (tile >> 8) * 30;
                else
                {
                    const int16_t current_viper_delay = (2 * 60) + rand() % (4 * 60);
                    viper_delay = m_zombie_base_delay + current_viper_delay;
                    m_zombie_base_delay += current_viper_delay;
                    if (m_zombie_base_delay > 8 * 60)
                        m_zombie_base_delay = 0;
                }
                Actor* viperActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateViper);
                viperActor->SetTemp2(viper_delay);
                actors[(y * level->GetLevelWidth()) + x] = viperActor;
                break;
            }
            case 22:
            {
                Actor* wretchedPoxActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateWretchedPox);
                actors[(y * level->GetLevelWidth()) + x] = wretchedPoxActor;
                break;
            }
            case 23:
            {
                Actor* succubusActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateSuccubus);
                actors[(y * level->GetLevelWidth()) + x] = succubusActor;
                break;
            }
            case 24:
            {
                const int16_t dragonDelay = (5 * 60) + rand() % (5 * 60);
                Actor* dragonActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateDragon);
                dragonActor->SetTemp2(dragonDelay);
                actors[(y * level->GetLevelWidth()) + x] = dragonActor;
                break;
            }
            case 25:
            {
                Actor* batActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateBat);
                actors[(y * level->GetLevelWidth()) + x] = batActor;
                break;
            }
            case 26:
            {
                Actor* eyeActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateEye);
                actors[(y * level->GetLevelWidth()) + x] = eyeActor;
                break;
            }
            case 27:
            {
                Actor* mageActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateMage);
                actors[(y * level->GetLevelWidth()) + x] = mageActor;
                break;
            }
            case 28:
            {
                const bool dropKey = (level->GetFloorTile(x + 1, y) >> 8 == 1);
                const uint16_t nemesisHealth = (level->GetFloorTile(x, y + 1) >> 8);
                Actor* nemesisActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateNemesis);
                nemesisActor->SetTemp1(dropKey ? actorIdBonusKeyRed : actorIdNone);
                if (nemesisHealth > 0)
                {
                    nemesisActor->SetTemp2(nemesisHealth * 3); // Shot power
                    nemesisActor->SetHealth(nemesisHealth * 10);
                }
                actors[(y * level->GetLevelWidth()) + x] = nemesisActor;
                break;
            }
            case 30:
            {
                int16_t ant_delay;
                unsigned int tile = level->GetFloorTile(x, y + 1);
                if (tile != 0)
                    ant_delay = (tile >> 8) * 30;
                else
                {
                    ant_delay = (2 * 60) + rand() % (5 * 60);
                }
                Actor* antActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateAnt);
                antActor->SetTemp2(ant_delay);
                actors[(y * level->GetLevelWidth()) + x] = antActor;
                break;
            }
            case 31:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateWarpPortal1);
                break;
            }
            case 32:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateWarpPortal2);
                break;
            }
            case 33:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateWarpPortal3);
                break;
            }
            case 34:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateWarpPortal4);
                break;
            }
            case 35:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateWarpPortal5);
                break;
            }
            case 36:
            {
                int16_t zombie_delay;
                unsigned int tile = level->GetFloorTile(x, y + 1);
                if (tile != 0)
                    zombie_delay = (tile >> 8) * 30;
                else
                {
                    const int16_t current_zombie_delay = (2 * 60) + rand() % (4 * 60);
                    zombie_delay = m_zombie_base_delay + current_zombie_delay;
                    m_zombie_base_delay += current_zombie_delay;
                    if (m_zombie_base_delay > 8 * 60)
                        m_zombie_base_delay = 0;
                }
                Actor* zombieActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateZombie);
                zombieActor->SetTemp2(zombie_delay);
                actors[(y * level->GetLevelWidth()) + x] = zombieActor;
                break;
            }
            case 37:
            {
                Actor* skeletonActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateSkeleton);
                actors[(y * level->GetLevelWidth()) + x] = skeletonActor;
                break;
            }
            case 38:
            {
                int16_t xofs[] = { 0,0,-1,+1 };
                int16_t yofs[] = { -1,+1,0,0 };
                uint16_t floorTile = 0;
                int16_t loop = 0;
                bool tileFound = false;
                while (loop<4 && !tileFound)
                {
                    const uint16_t wallTile = level->GetWallTile(x + xofs[loop], y + yofs[loop]);
                    if (wallTile == 66 || wallTile == 67 || wallTile == 68 || wallTile == 69)
                    {
                        floorTile = level->GetFloorTile(x + xofs[loop], y + yofs[loop]);
                        tileFound = true;
                    }
                    loop++;
                }
                int16_t zombie_delay;
                if (floorTile > 0)
                {
                    zombie_delay = (floorTile >> 8) * 30;
                }
                else
                {
                    const int16_t current_zombie_delay = (2 * 60) + rand() % (4 * 60);
                    zombie_delay = m_zombie_base_delay + current_zombie_delay;
                    m_zombie_base_delay += current_zombie_delay;
                    if (m_zombie_base_delay > 8 * 60)
                        m_zombie_base_delay = 0;
                }
                Actor* wallSkeletonActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateWallSkeleton);
                wallSkeletonActor->SetTemp2(zombie_delay);
                actors[(y * level->GetLevelWidth()) + x] = wallSkeletonActor;
                break;
            }
            case 39:
            {
                Actor* freezeTimeActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateFreezeTime);
                actors[(y * level->GetLevelWidth()) + x] = freezeTimeActor;
                break;
            }
            case 40:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateGemRed);
                break;
            case 41:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateGemGreen);
                break;
            case 42:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateGemBlue);
                break;
            case 43:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateGemYellow);
                break;
            case 44:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateGemPurple);
                break;
            case 45:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateTomb1);
                break;
            case 46:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateTomb2);
                break;
            case 47:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateTomb3);
                break;
            case 49:
            {
                Actor* bonusActorChest = level->IsWaterLevel() ? new Actor(x + 0.5f, y + 0.5f, 0, decorateWaterChest) : new Actor(x + 0.5f, y + 0.5f, 0, decorateChest);
                actors[(y * level->GetLevelWidth()) + x] = bonusActorChest;
                break;
            }
            case 50:
            {
                int16_t zombie_delay;
                unsigned int tile = level->GetFloorTile(x, y + 1);
                if (tile != 0)
                    zombie_delay = (tile >> 8) * 30;
                else
                {
                    const int16_t current_zombie_delay = (2 * 60) + rand() % (4 * 60);
                    zombie_delay = m_zombie_base_delay + current_zombie_delay;
                    m_zombie_base_delay += current_zombie_delay;
                    if (m_zombie_base_delay > 8 * 60)
                        m_zombie_base_delay = 0;
                }
                Actor* treeActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateTree);
                treeActor->SetTemp2(zombie_delay);
                actors[(y * level->GetLevelWidth()) + x] = treeActor;
                break;
            }
            case 51:
            {
                int16_t zombie_delay = rand() % 30;
                unsigned int tile = level->GetFloorTile(x, y + 1);
                if (tile != 0)
                    zombie_delay = (tile >> 8) * 30;
                else
                {
                    const int16_t current_zombie_delay = (2 * 60) + rand() % (4 * 60);
                    zombie_delay = m_zombie_base_delay + current_zombie_delay;
                    m_zombie_base_delay += current_zombie_delay;
                    if (m_zombie_base_delay > 8 * 60)
                        m_zombie_base_delay = 0;
                }
                Actor* bunnyActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateBunny);
                bunnyActor->SetTemp2(zombie_delay);
                actors[(y * level->GetLevelWidth()) + x] = bunnyActor;
                break;
            }
            case 52:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch1);
                break;
            }
            case 54:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateColumn);
                break;
            case 55:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateSulphurGas);
                break;
            case 56:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateFirePot);
                break;
            case 57:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch13);
                break;
            }
            case 58:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateFountain);
                break;
            }
            case 59:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateForceField);
                break;
            }
            case 60:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch2);
                break;
            }
            case 61:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch3);
                break;
            }
            case 62:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch4);
                break;
            }
            case 63:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch5);
                break;
            }
            case 64:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch6);
                break;
            }
            case 66:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch12);
                break;
            }
            case 67:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch7);
                break;
            }
            case 68:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch8);
                break;
            }
            case 69:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch9);
                break;
            }
            case 70:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch10);
                break;
            }
            case 71:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateArch11);
                break;
            }
            default:
                break;
            }
        }
    }
}

const DecorateActor& GameArmageddon::GetExplosionActor()
{
    return decorateExplosion;
}

const DecorateActor& GameArmageddon::GetExplodingWallActor()
{
    return decorateExplodingWall;
}

const DecorateActor& GameArmageddon::GetPlayerActor()
{
    return decoratePlayer;
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

void GameArmageddon::DrawScroll(const uint8_t scrollIndex)
{
    // No scrolls in Armageddon
}

void GameArmageddon::DrawFinal()
{
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(FINALEPIC), 0, 0);
}

const uint16_t GameArmageddon::GetMenuCursorPic() const
{
    return SKULL_SHOTPIC;
}

const uint16_t GameArmageddon::GetNorthIconSprite() const
{
    return NORTHICONSPR;
}

const std::string GameArmageddon::GetSavedGamesPath() const
{
    return "\\Armageddon";
}

const std::vector<std::vector<uint16_t>> wallSkeletonAnimations = { { 66, 68, 21 },{ 67, 69, 21 } };
const std::vector<std::vector<uint16_t>>& GameArmageddon::GetWallSkeletonAnimations() const
{
    return wallSkeletonAnimations;
}