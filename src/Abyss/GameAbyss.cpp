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

#include "GameAbyss.h"
#include "GameMapsAbyss.h"
#include "IntroViewAbyss.h"
#include "../Engine/IRenderer.h"
#include "../Engine/ExtraMenu.h"

// Decorate
#include "DecorateAll.h"

static const std::string AbyssV113Name = "Catacomb Abyss v1.13 Shareware";
static const std::string AbyssV124Name = "Catacomb Abyss v1.24 Registered";

GameAbyss::GameAbyss(const uint8_t gameId, const std::string gamePath, IRenderer& renderer) :
    m_gameId (gameId),
    m_gamePath (gamePath),
    m_renderer (renderer),
    m_zombie_base_delay (0),
    m_introView (nullptr),
    m_helpPageIndex(0)
{
    m_gameMaps = nullptr;
    m_egaGraph = nullptr;
    m_audioRepository = nullptr;
    m_audioPlayer = nullptr;
    m_helpPages = nullptr;
}

GameAbyss::~GameAbyss()
{
    delete m_gameMaps;
    delete m_egaGraph;
    delete m_audioRepository;
    delete m_audioPlayer;
    delete m_introView;
    delete m_helpPages;
}

void GameAbyss::SpawnActors(Level* level, const DifficultyLevel difficultyLevel)
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
            case 12:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateScroll1);
                break;
            case 13:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateScroll2);
                break;
            case 14:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateScroll3);
                break;
            case 15:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateScroll4);
                break;
            case 16:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateScroll5);
                break;
            case 17:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateScroll6);
                break;
            case 18:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateScroll7);
                break;
            case 19:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateScroll8);
                break;
            case 29:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateKeyRed2);;
                break;
            case 21:
                {
                    Actor* bonusActorChest = level->IsWaterLevel() ? new Actor(x + 0.5f, y + 0.5f, 0, decorateWaterChest) : new Actor(x + 0.5f, y + 0.5f, 0, decorateChest);
                    actors[(y * level->GetLevelWidth()) + x] = bonusActorChest;
                    break;
                }
            case 20:
            case 24:
            case 30:
                {
                    Actor* redDemonActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateRedDemon);
                    actors[(y * level->GetLevelWidth()) + x] = redDemonActor;
                    break;
                }
            case 43:
                if (difficultyLevel < Hard)
                {
                    break;
                }
            case 38:
                if (difficultyLevel < Normal)
                {
                    break;
                }
            case 25:
                {
                    Actor* batActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateBat);
                    actors[(y * level->GetLevelWidth()) + x] = batActor;
                    break;
                }
            case 44:
                if (difficultyLevel < Hard)
                {
                    break;
                }
            case 39:
                if (difficultyLevel < Normal)
                {
                    break;
                }
            case 26:
                {
                    Actor* demonActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateDemon);
                    actors[(y * level->GetLevelWidth()) + x] = demonActor;
                    break;
                }
            case 41:
                if (difficultyLevel < Hard)
                {
                    break;
                }
            case 36:
                if (difficultyLevel < Normal)
                {
                    break;
                }
            case 22:
                {
                    Actor* trollActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateTroll);
                    actors[(y * level->GetLevelWidth()) + x] = trollActor;
                    break;
                }
            case 42:
                if (difficultyLevel < Hard)
                {
                    break;
                }
            case 37:
                if (difficultyLevel < Normal)
                {
                    break;
                }
            case 23:
                {
                    Actor* orcActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateOrc);
                    actors[(y * level->GetLevelWidth()) + x] = orcActor;
                    break;
                }
            case 65:
                if (difficultyLevel < Hard)
                {
                    break;
                }
            case 64:
                if (difficultyLevel < Normal)
                {
                    break;
                }
            case 63:
                {
                    const int16_t wetManDelay = (5*60)+ rand() % (5*60);
                    Actor* wetManActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateWetMan);
                    wetManActor->SetTemp2(wetManDelay);
                    actors[(y * level->GetLevelWidth()) + x] = wetManActor;
                    break;
                }
            case 68:
                if (difficultyLevel < Hard)
                {
                    break;
                }
            case 67:
                if (difficultyLevel < Normal)
                {
                    break;
                }
            case 66:
                {
                    Actor*eyeActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateEye);
                    actors[(y * level->GetLevelWidth()) + x] = eyeActor;
                    break;
                }
            case 45:
                if (difficultyLevel < Hard)
                {
                    break;
                }
            case 40:
                if (difficultyLevel < Normal)
                {
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
                    Actor* nemesisActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateNemesis);
                    nemesisActor->SetTemp1(actorIdBonusKeyRed); // Always drop key;
                    actors[(y * level->GetLevelWidth()) + x] = nemesisActor;
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
            case 54:
                {
                    actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateWarpPit);
                    break;
                }
            case 46:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateTomb1);
                break;
            case 47:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateTomb2);
                break;
            case 48:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateTomb3);
                break;
            case 51:
                if (difficultyLevel < Hard)
                {
                    break;
                }
            case 50:
                if (difficultyLevel < Normal)
                {
                    break;
                }
            case 49:
                {
                    int16_t spook_delay;
                    unsigned int tile = level->GetFloorTile(x, y + 1);
                    if (tile != 0)
                        spook_delay = (tile>>8)*30;
                    else
                    {
                        spook_delay = (2*60)+ rand() % (5*60);
                    }
                    Actor* spookActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateSpook);
                    spookActor->SetTemp2(spook_delay);
                    actors[(y * level->GetLevelWidth()) + x] = spookActor;
                    break;
                }
            case 53:
                if (difficultyLevel < Normal)
                {
                    break;
                }
            case 52:
                {
                    int16_t zombie_delay;
                    unsigned int tile = level->GetFloorTile(x, y + 1);
                    if (tile != 0)
                        zombie_delay = (tile>>8)*30;
                    else
                    {
                        const int16_t current_zombie_delay = (2*60)+ rand() % (4*60);
                        zombie_delay = m_zombie_base_delay+current_zombie_delay;
                        m_zombie_base_delay += current_zombie_delay;
                        if (m_zombie_base_delay > 8*60)
                            m_zombie_base_delay = 0;
                    }
                    Actor* zombieActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateZombie);
                    zombieActor->SetTemp2(zombie_delay);
                    actors[(y * level->GetLevelWidth()) + x] = zombieActor;
                    break;
                }
            case 56:
                if (difficultyLevel < Normal)
                {
                    break;
                }
            case 55:
                {
                    Actor* skeletonActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateSkeleton);
                    actors[(y * level->GetLevelWidth()) + x] = skeletonActor;
                    break;
                }
            case 57:
                {
                    Actor* freezeTimeActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateFreezeTime);
                    actors[(y * level->GetLevelWidth()) + x] = freezeTimeActor;
                    break;
                }
            case 58:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateGemRed);
                break;
            case 59:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateGemGreen);
                break;
            case 60:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateGemBlue);
                break;
            case 61:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateGemYellow);
                break;
            case 62:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateGemPurple);
                break;
            case 71:
                if (difficultyLevel < Hard)
                {
                    break;
                }
            case 70:
                if (difficultyLevel < Normal)
                {
                    break;
                }
            case 69:
                {
                    int16_t xofs[] = { 0,0,-1,+1 };
                    int16_t yofs[] = { -1,+1,0,0 };
                    uint16_t floorTile = 0;
                    int16_t loop = 0;
                    bool tileFound = false;
                    while (loop < 4 && !tileFound)
                    {
                        const uint16_t wallTile = level->GetWallTile(x + xofs[loop], y + yofs[loop]);
                        if (wallTile == 6 || wallTile == 7 || wallTile == 8 || wallTile == 41 || wallTile == 42 || wallTile == 43 || wallTile == 44)
                        {
                            floorTile = level->GetFloorTile(x + xofs[loop], y + yofs[loop]);
                            tileFound = true;
                        }
                        loop++;
                    }

                    // Only spawn a wall skeleton when an appropriate nearby wall tile was found!
                    if (tileFound)
                    {
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
                    }
                    break;
                }
            default:
                break;
            }
        }
    }
}

const DecorateActor& GameAbyss::GetExplosionActor()
{
    return decorateExplosion;
}

const DecorateActor& GameAbyss::GetExplodingWallActor()
{
    return decorateExplodingWall;
}

const DecorateActor& GameAbyss::GetPlayerActor()
{
    return decoratePlayer;
}

void GameAbyss::DrawStatusBar(const int16_t health, const std::string& locationMessage, const PlayerInventory& playerInventory, const uint16_t wideScreenMargin, const float /*playerAngle*/, const uint8_t /*levelIndex*/, const uint16_t /*shotPower*/, const uint32_t /*points*/)
{
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(egaGraphicsAbyss::STATUSPIC), 0, 120);

    const Font& font = *GetEgaGraph()->GetFont(3);
    RenderableText renderableText(font);
    RenderableTiles tiles(*GetEgaGraph()->GetTilesSize8());
    DrawHealth(tiles, health);
    DrawScrolls(renderableText, playerInventory);
    DrawKeys(tiles, playerInventory);
    DrawBonus(tiles, playerInventory);
    DrawGems(playerInventory);

    renderableText.Centered(locationMessage, EgaBrightYellow, 160, 121);
    m_renderer.RenderText(renderableText);
    m_renderer.RenderTiles(tiles);

    if (wideScreenMargin > 0)
    {
        // TODO: fill wide screen border
    }
}

void GameAbyss::DrawHealth(RenderableTiles& renderableTiles, const int16_t health)
{
    const uint16_t percentage = (uint16_t) health;
    renderableTiles.DrawNumberRightAligned(96, 177, health);

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

    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(picnum), 64, 134);
}

void GameAbyss::DrawScrolls(RenderableText& renderableText, const PlayerInventory& playerInventory)
{
    for (uint8_t loop=0;loop<8;loop++)
    {
        if (playerInventory.GetScroll(loop))
        {
            uint8_t y = 150 + ((loop > 3) * 10);
            uint8_t x = 209 + (loop % 4) * 8;
            renderableText.Number(loop + 1, 1, EgaBlack, x, y);
        }
    }
}

void GameAbyss::DrawKeys(RenderableTiles& renderableTiles, const PlayerInventory& playerInventory)
{
    renderableTiles.DrawNumberRightAligned(176, 150, playerInventory.GetKeys(RedKey));
    renderableTiles.DrawNumberRightAligned(200, 177, playerInventory.GetKeys(YellowKey));
    renderableTiles.DrawNumberRightAligned(200, 150, playerInventory.GetKeys(GreenKey));
    renderableTiles.DrawNumberRightAligned(176, 177, playerInventory.GetKeys(BlueKey));
}

void GameAbyss::DrawBonus(RenderableTiles& renderableTiles, const PlayerInventory& playerInventory)
{
    renderableTiles.DrawNumberRightAligned(152, 138, playerInventory.GetBolts());
    renderableTiles.DrawNumberRightAligned(152, 156, playerInventory.GetNukes());
    renderableTiles.DrawNumberRightAligned(152, 174, playerInventory.GetPotions());
}

void GameAbyss::DrawGems(const PlayerInventory& playerInventory)
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

void GameAbyss::DrawScroll(const uint8_t scrollIndex)
{
    m_renderer.Render2DBar(0, 0, 320, 120, EgaBlack);
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(SCROLLTOPPIC), 80, 0);
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(SCROLL1PIC + scrollIndex), 80, 32);
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(SCROLLBOTTOMPIC), 80, 88);
}

void GameAbyss::DrawFinal()
{
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(FINALEPIC), 0, 0);
}

GameMaps* GameAbyss::GetGameMaps()
{
    if (m_gameMaps == nullptr)
    {
        m_gameMaps = new GameMaps(gameMapsAbyss, m_gamePath);
    }

    return m_gameMaps;
}

EgaGraph* GameAbyss::GetEgaGraph()
{
    if (m_egaGraph == nullptr)
    {
        const egaGraphStaticData& staticData = (m_gameId == 1) ? egaGraphAbyss : egaGraphAbyssV124;
        m_egaGraph = new EgaGraph(staticData, m_gamePath, m_renderer);
    }

    return m_egaGraph;
}

AudioRepository* GameAbyss::GetAudioRepository()
{
    if (m_audioRepository == nullptr)
    {
        m_audioRepository = new AudioRepository(audioRepositoryAbyss, m_gamePath);
    }

    return m_audioRepository;
}

AudioPlayer* GameAbyss::GetAudioPlayer()
{
    if (m_audioPlayer == nullptr)
    {
        m_audioPlayer = new AudioPlayer(GetAudioRepository());
    }

    return m_audioPlayer;
}

IIntroView* GameAbyss::GetIntroView()
{
    if (m_introView == nullptr)
    {
        m_introView = new IntroViewAbyss(m_renderer, m_gamePath);
    }

    return m_introView;
}

IMenu* GameAbyss::CreateMenu(ConfigurationSettings& configurationSettings, PlayerInput& playerInput, std::vector<std::string>& savedGames)
{
    return new ExtraMenu(configurationSettings, *GetAudioPlayer(), playerInput, GetEgaGraph(), GetMenuCursorPic(), savedGames, m_renderer, GetCatalogInfo(), m_gamePath);
}

void GameAbyss::DrawHelpPage()
{
    HelpPages* helpPages = GetHelpPages();

    const HelpPage& helpPage = helpPages->GetPage(m_helpPageIndex);
    const Font& font = *GetEgaGraph()->GetFont(3);
    RenderableText renderableText(font);
    uint16_t yOffset = 8;
    for (uint16_t lineIndex = 0; lineIndex < helpPage.size(); lineIndex++)
    {
        const HelpLine& helpLine = helpPage.at(lineIndex);
        if (helpLine.centered)
        {
            renderableText.Centered(helpLine.line, EgaDarkGray, 320, yOffset);
            yOffset += 9;
        }
        else
        {
            const uint8_t numberOfLines = renderableText.LeftAlignedMultiLine(helpLine.line, EgaDarkGray, 16, yOffset);
            yOffset += (numberOfLines * 9);
        }
    }
    m_renderer.RenderText(renderableText);
}

bool GameAbyss::ProcessInputOnHelpPage(PlayerInput& playerInput)
{
    if (playerInput.IsKeyJustPressed(SDLK_ESCAPE))
    {
        m_helpPageIndex = 0;
        return true;
    }
    else if (playerInput.IsKeyJustPressed(SDLK_LEFT) || playerInput.IsKeyJustPressed(SDLK_UP))
    {
        if (m_helpPageIndex > 0)
        {
            m_helpPageIndex--;
        }
    }
    else if (playerInput.IsKeyJustPressed(SDLK_RIGHT) || playerInput.IsKeyJustPressed(SDLK_DOWN))
    {
        if (m_helpPageIndex < GetHelpPages()->GetNumberOfPages() - 1)
        {
            m_helpPageIndex++;
        }
    }

    return false;
}

HelpPages* GameAbyss::GetHelpPages()
{
    if (m_helpPages == nullptr)
    {
        std::ifstream file;
        const std::string fullPath = m_gamePath + "HELP.TXT";
        file.open(fullPath, std::ifstream::binary);
        if (file.is_open())
        {
            std::string str;
            char c = 0;
            while (file.get(c))
            {
                str.append(1, c);
            }
            file.close();
            m_helpPages = new HelpPages(str);
        }
        else
        {
            m_helpPages = new HelpPages();
        }
    }

    return m_helpPages;
}

const std::map<uint16_t, const DecorateActor>& GameAbyss::GetDecorateActors() const
{
    return decorateAbyssAll;
}

const std::string& GameAbyss::GetName() const
{
    return (m_gameId == 1) ? AbyssV113Name : AbyssV124Name;
}

const uint8_t GameAbyss::GetId() const
{
    return m_gameId;
}

const uint16_t GameAbyss::GetMenuCursorPic() const
{
    return SKULL_SHOTPIC;
}

const uint16_t GameAbyss::GetNorthIconSprite() const
{
    return NORTHICONSPR;
}

const std::string GameAbyss::GetSavedGamesPath() const
{
    return "\\Abyss";
}

const std::vector<std::vector<uint16_t>> wallSkeletonAnimations = { { 6, 7, 8, 6 }, { 41, 43, 41}, {42, 44, 42 } };
const std::vector<std::vector<uint16_t>>& GameAbyss::GetWallSkeletonAnimations() const
{
    return wallSkeletonAnimations;
}

const SDL_Keycode GameAbyss::GetCheatsKeyCode() const
{
    return SDLK_F10;
}

void GameAbyss::PlaySoundBodyExplode()
{
    // Not applicable
}

void GameAbyss::PlaySoundHitGate()
{
    m_audioPlayer->Play(HIT_GATESND);
}

void GameAbyss::PlaySoundPlayerHurt(const int16_t playerHealth)
{
    if (playerHealth > 33)
    {
        m_audioPlayer->Play(TAKEDAMAGESND);
    }
    else if (playerHealth > 0)
    {
        m_audioPlayer->Play(TAKEDMGHURTSND);
    }
    else
    {
        m_audioPlayer->Play(GAMEOVERSND);
    }
}

void GameAbyss::PlaySoundGetPotion()
{
    m_audioPlayer->Play(GETPOTIONSND);
}

void GameAbyss::PlaySoundUsePotion()
{
    m_audioPlayer->Play(USEPOTIONSND);
}

void GameAbyss::PlaySoundNoItem()
{
    m_audioPlayer->Play(NOITEMSND);
}

void GameAbyss::PlaySoundGetBolt()
{
    m_audioPlayer->Play(GETBOLTSND);
}

void GameAbyss::PlaySoundGetNuke()
{
    m_audioPlayer->Play(GETNUKESND);
}

void GameAbyss::PlaySoundGetScroll()
{
    m_audioPlayer->Play(GETSCROLLSND);
}

void GameAbyss::PlaySoundGetGem()
{
    m_audioPlayer->Play(GETGEMSND);
}

void GameAbyss::PlaySoundGetKey()
{
    m_audioPlayer->Play(GETKEYSND);
}

void GameAbyss::PlaySoundGetPoints()
{
    m_audioPlayer->Play(GETPOINTSSND);
}

void GameAbyss::PlaySoundUseKey()
{
    m_audioPlayer->Play(USEKEYSND);
}

void GameAbyss::PlaySoundUseBolt()
{
    m_audioPlayer->Play(USEBOLTSND);
}

void GameAbyss::PlaySoundUseNuke()
{
    m_audioPlayer->Play(USENUKESND);
}

void GameAbyss::PlaySoundFreezeTime()
{
    m_audioPlayer->Play(FREEZETIMESND);
}

void GameAbyss::PlaySoundFreezeTimeTick(const int32_t freezeTicks)
{
    if (freezeTicks == 0)
    {
        m_audioPlayer->Play(TIMERETURNSND);
    }
    else if (freezeTicks < 20 && freezeTicks % 2 == 0)
    {
        m_audioPlayer->Play(TICKSND);
    }
}

void GameAbyss::PlaySoundWalk(const bool walk1)
{
    m_audioPlayer->Play(walk1 ? WALK1SND : WALK2SND);
}

void GameAbyss::PlaySoundBossDeath()
{
    m_audioPlayer->Play(GRELM_DEADSND);
}

void GameAbyss::PlaySoundShoot()
{
    m_audioPlayer->Play(SHOOTSND);
}

void GameAbyss::PlaySoundShootWall()
{
    m_audioPlayer->Play(SHOOTWALLSND);
}

void GameAbyss::PlaySoundBoom()
{
    m_audioPlayer->Play(BOOMSND);
}

void GameAbyss::PlaySoundSmallMonster()
{

}

void GameAbyss::PlaySoundLargeMonster()
{

}

void GameAbyss::PlaySoundWarpUpOrDown(const bool up)
{
    m_audioPlayer->Play(up ? WARPUPSND : WARPDOWNSND);
}

void GameAbyss::PlaySoundWarp()
{
    m_audioPlayer->Play(WARPSND);
}

void GameAbyss::PlaySoundPortal()
{

}

static ViewPorts::ViewPortRect3D original3DViewArea = { 0, 120, 320, 120 };

const ViewPorts::ViewPortRect3D& GameAbyss::GetOriginal3DViewArea()
{
    return original3DViewArea;
}

static const ManaBar::ManaBarConfig manaBarConfig = { original3DViewArea.width / 2, 114, EgaBrightCyan, EgaCyan };

const ManaBar::ManaBarConfig& GameAbyss::GetManaBarConfig()
{
    return manaBarConfig;
}

const CatalogInfo catalogInfoV113 =
{
    "Demo"
,
{
    "OPEN.CAT",
    "TRILOGY.CAT",
    "QUEST.CAT",
    "INVITED.CAT",
    "GROUPIC.CAT",
    "LEGEND.CAT", // It's easy to explore the dark regions of THE CATACOMBS
    "FOLLOW.CAT",
    "CEMETRY.CAT",
    "ORCMINE.CAT",
    "GRATE.CAT",
    "INFERNO.CAT",
    "COOKIE.CAT",// Halls of the wretched pox
    "DRAGONS.CAT",
    "FLAME.CAT",
    "CRYSTAL.CAT",
    "ALTAR.CAT",
    "REDHALL.CAT", // Sanctuary of the damned
    "EGYPT.CAT",
    "APC.CAT", // Citadel of the automations
    "VAULT.CAT", // Are you up to it?
    "LOUNGE.CAT", // Play ALL 3 of the Catacomb 3D Adventures
    "HOMEBBS.CAT",
    "VICTORY.CAT"
}};

const CatalogInfo catalogInfoV124 =
{
    "Hints & Solutions"
,
{
    "HINTGO1.HNT",
    "HINTGO2.HNT",
    "CEMETRY.HNT",
    "GARDEN.HNT",
    "MAUSGND.HNT",
    "MAUSOL.HNT",
    "CRYPT.HNT",
    "DEN.HNT",
    "SUBVALT.HNT",
    "AQUDUCT.HNT",
    "ORCMINE.HNT",
    "TROLLS.HNT",
    "INFERNO.HNT",
    "TITANS.HNT",
    "COVEN.HNT",
    "SANCTUM.HNT",
    "HAUNT.HNT",
    "PASSAGE.HNT",
    "VICTORY.HNT",
    "HINTGO3.HNT"
} };

const CatalogInfo& GameAbyss::GetCatalogInfo() const
{
    return (m_gameId == 1) ? catalogInfoV113 : catalogInfoV124;
}
