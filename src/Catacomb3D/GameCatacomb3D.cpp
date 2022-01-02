// Copyright (C) 2019 Arno Ansems
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

#include "GameCatacomb3D.h"
#include "EgaGraphCatacomb3D.h"
#include "GameMapsCatacomb3D.h"
#include "IntroViewCatacomb3D.h"
#include "AudioRepositoryCatacomb3D.h"
#include "DecorateAll.h"
#include "../Engine/IRenderer.h"
#include "Catacomb3DMenu.h"

static const std::string Catacomb3DName = "Catacomb 3-D v1.22";

GameCatacomb3D::GameCatacomb3D(const std::string gamePath, const std::string configPath, IRenderer& renderer) :
    m_gameId(5),
    m_gamePath(gamePath),
    m_configPath(configPath),
    m_renderer(renderer),
    m_introView(nullptr),
    m_highScores(std::make_unique<HighScores>())
{
    m_gameMaps = nullptr;
    m_egaGraph = nullptr;
    m_audioRepository = nullptr;
    m_audioPlayer = nullptr;

    if (!m_highScores->LoadFromFile(m_configPath))
    {
        // The high scores could not be loaded from the config folder.
        // Try to obtain the high scores from the orginal game data.
        m_highScores->LoadFromFile(m_gamePath);
    }
}

GameCatacomb3D::~GameCatacomb3D()
{
    m_highScores->StoreToFile(m_configPath);
    delete m_gameMaps;
    delete m_egaGraph;
    delete m_audioRepository;
    delete m_audioPlayer;
    delete m_introView;
}

void GameCatacomb3D::SpawnActors(Level* level, const DifficultyLevel difficultyLevel)
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
            case 20:
            {
                Actor* bonusActorGrelminar = new Actor(x + 0.5f, y + 0.5f, 0, decorateGrelminar);
                actors[(y * level->GetLevelWidth()) + x] = bonusActorGrelminar;
                break;
            }
            case 21:
            {
                Actor* bonusActorChest = new Actor(x + 0.5f, y + 0.5f, 0, decorateChest);
                actors[(y * level->GetLevelWidth()) + x] = bonusActorChest;
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
            case 24:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateWarpToLevel);
                break;
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
                actors[(y * level->GetLevelWidth()) + x] = nemesisActor;
                break;
            }
            case 29:
            case 30:
            {
                Actor* bounceActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateBounce);
                const actorDirection dir = (tile == 29) ? north : west;
                bounceActor->SetDirection(dir);
                actors[(y * level->GetLevelWidth()) + x] = bounceActor;
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
            default:
                break;
            }
        }
    }
}

const DecorateActor& GameCatacomb3D::GetExplosionActor()
{
    return decorateExplosion;
}

const DecorateActor& GameCatacomb3D::GetExplodingWallActor()
{
    return decorateExplodingWall;
}

const DecorateActor& GameCatacomb3D::GetPlayerActor()
{
    return decoratePlayer;
}

GameMaps* GameCatacomb3D::GetGameMaps()
{
    if (m_gameMaps == nullptr)
    {
        m_gameMaps = new GameMaps(gameMapsCatacomb3D, m_gamePath);
    }

    return m_gameMaps;
}

EgaGraph* GameCatacomb3D::GetEgaGraph()
{
    if (m_egaGraph == nullptr)
    {
        m_egaGraph = new EgaGraph(egaGraphCatacomb3D, m_gamePath, m_renderer);
        m_highScores->LoadGraphics(*m_egaGraph, HIGHSCORESPIC);
    }

    return m_egaGraph;
}

AudioRepository* GameCatacomb3D::GetAudioRepository()
{
    if (m_audioRepository == nullptr)
    {
        m_audioRepository = new AudioRepository(audioRepositoryCatacomb3D, m_gamePath);
    }

    return m_audioRepository;
}

AudioPlayer* GameCatacomb3D::GetAudioPlayer()
{
    if (m_audioPlayer == nullptr)
    {
        m_audioPlayer = new AudioPlayer(GetAudioRepository());
    }

    return m_audioPlayer;
}

IIntroView* GameCatacomb3D::GetIntroView()
{
    if (m_introView == nullptr)
    {
        m_introView = new IntroViewCatacomb3D(m_renderer, GetEgaGraph(), m_gamePath, *m_highScores);
    }

    return m_introView;
}

IMenu* GameCatacomb3D::CreateMenu(
    ConfigurationSettings& configurationSettings,
    PlayerInput& playerInput,
    std::vector<std::string>& savedGames,
    SavedGamesInDosFormat& savedGamesInDosFormat)
{
    return new Catacomb3DMenu(configurationSettings, *GetAudioPlayer(), playerInput, GetEgaGraph(), savedGames, *m_highScores, savedGamesInDosFormat);
}

void GameCatacomb3D::DrawHelpPage()
{
    // No help page in the Catacomb 3-D
}

bool GameCatacomb3D::ProcessInputOnHelpPage(PlayerInput& playerInput)
{
    return (playerInput.GetFirstKeyPressed() != SDLK_UNKNOWN);
}

const std::map<uint16_t, const DecorateActor>& GameCatacomb3D::GetDecorateActors() const
{
    return decorateCatacomb3DAll;
}

const std::string& GameCatacomb3D::GetName() const
{
    return Catacomb3DName;
}

const uint8_t GameCatacomb3D::GetId() const
{
    return m_gameId;
}

void GameCatacomb3D::DrawStatusBarWideScreenMargin(const int16_t offsetX, const int16_t marginWidth)
{
    if (marginWidth > 0 && marginWidth < 8)
    {
        m_renderer.Render2DBar(offsetX, 144, marginWidth, 56, EgaLightGray);
    }

    if (marginWidth >= 8)
    {
        m_renderer.Render2DPictureSegment(GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::STATUSPIC), offsetX, 144, 0, 0, 4, 56);
        m_renderer.Render2DPictureSegment(GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::STATUSPIC), offsetX + marginWidth - 4, 144, 258, 0, 4, 56);

        if (marginWidth > 8)
        {
            int16_t remainingWidth = marginWidth - 8;

            while (remainingWidth > 0)
            {
                const int16_t segmentWidth = (remainingWidth >= 10) ? 10 : remainingWidth;
                m_renderer.Render2DPictureSegment(GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::STATUSPIC), offsetX + marginWidth - 4 - remainingWidth, 144, 50, 0, segmentWidth, 56);
                remainingWidth -= segmentWidth;
            }
        }
    }
}

void GameCatacomb3D::DrawStatusBar(const int16_t health, const std::string& locationMessage, const PlayerInventory& playerInventory, const uint16_t wideScreenMargin, const float playerAngle, const uint8_t levelIndex, const uint16_t shotPower, const uint32_t points)
{
    const uint16_t sideBarWidth = GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::SIDEBARSPIC)->GetImageWidth();
    m_renderer.Render2DPictureSegment(GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::STATUSPIC), 0, 144, 0, 0, 320 - sideBarWidth, 56);
    m_renderer.Render2DPictureSegment(GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::STATUSPIC), 320 + wideScreenMargin - sideBarWidth - 2, 144, 320 - sideBarWidth - 2, 0, sideBarWidth + 2, 56);
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::SIDEBARSPIC), 320 + wideScreenMargin - sideBarWidth, 0);
    if (shotPower > 0)
    {
        m_renderer.Render2DPictureSegment(GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::SHOTPOWERPIC), 320 + wideScreenMargin - sideBarWidth + 8, 136 - shotPower, 0, 56 - shotPower, GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::SHOTPOWERPIC)->GetImageWidth(), shotPower);
    }

    const uint16_t compasPictureIndex = COMPAS1PIC + ((uint16_t)((playerAngle + 11.25f) / 22.5f) % 16);
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(compasPictureIndex), 320 + wideScreenMargin - sideBarWidth, 144);

    // Health status
    if (health < 100)
    {
        m_renderer.Render2DPictureSegment(GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::NOBODYPIC), 320 + wideScreenMargin - sideBarWidth + 8, 8, 0, 0, GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::NOBODYPIC)->GetImageWidth(), GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::BODYPIC)->GetImageHeight());
    }
    if (health > 0)
    {
        m_renderer.Render2DPictureSegment(GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::BODYPIC), 320 + wideScreenMargin - sideBarWidth + 8, 8, 0, 0, GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::BODYPIC)->GetImageWidth(), (uint16_t)(GetEgaGraph()->GetPicture(egaGraphicsCatacomb3D::BODYPIC)->GetImageHeight() * (health / 100.0f)));
    }
    if (health < 1)
    {
        // Dead
        if (wideScreenMargin > 0)
        {
            m_renderer.Render2DBar(0 - wideScreenMargin, 0, wideScreenMargin, 144, EgaDarkGray);
            m_renderer.Render2DBar(320 - sideBarWidth, 0, wideScreenMargin, 144, EgaDarkGray);
        }
        m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(DEADPIC), 0, 0);
    }

    RenderableText renderableText(*m_egaGraph->GetFont(3));
    renderableText.Centered(std::to_string(levelIndex + 1), EgaBrightYellow, 12, 148);

    const uint16_t maxItemsToShow = 9;

    RenderableTiles tiles(*GetEgaGraph()->GetTilesSize8());

    const int8_t numBoltsToShow = (playerInventory.GetBolts() < maxItemsToShow) ? playerInventory.GetBolts() : maxItemsToShow;
    for (int8_t i = 0; i < numBoltsToShow; i++)
    {
        tiles.Add(56 + (i * 8), 165, RenderableTiles::TileIdBolt);
    }
    const int8_t numNukesToShow = (playerInventory.GetNukes() < maxItemsToShow) ? playerInventory.GetNukes() : maxItemsToShow;
    for (int8_t i = 0; i < numNukesToShow; i++)
    {
        tiles.Add(56 + (i * 8), 175, RenderableTiles::TileIdNuke);
    }
    const int8_t numPotionsToShow = (playerInventory.GetPotions() < maxItemsToShow) ? playerInventory.GetPotions() : maxItemsToShow;
    for (int8_t i = 0; i < numPotionsToShow; i++)
    {
        tiles.Add(56 + (i * 8), 185, RenderableTiles::TileIdPotion);
    }

    int8_t x = 24;
    for (int8_t i = 0; i < 4; i++)
    {
        const int8_t numKeysToShow = playerInventory.GetKeys(i) > 2 ? 2 : playerInventory.GetKeys(i);
        for (int8_t j = 0; j < numKeysToShow; j++)
        {
            tiles.Add((x * 8), 165, (uint16_t)(RenderableTiles::TileIdFirstKey + i));
            x++;
        }
    }

    for (int8_t i = 0; i < 8; i++)
    {
        if (playerInventory.GetScroll(i))
        {
            tiles.Add(192 + (i * 8), 175, (uint16_t)(RenderableTiles::TileIdFirstScroll + i));
        }
    }

    // Score
    {
        const std::string scoreStr = std::to_string(points);
        const int16_t length = (int16_t)scoreStr.length();
        int16_t x = 256 - (8 * (int16_t)length);
        for (int16_t i = 0; i < length; i++)
        {
            tiles.Add(x, 185, (uint16_t)(RenderableTiles::TileIdFirstNumber + scoreStr.at(i) - '0' ));
            x += 8;
        }
    }

    m_renderer.RenderTiles(tiles);
    
    DrawStatusBarWideScreenMargin(0 - wideScreenMargin, wideScreenMargin);
    DrawStatusBarWideScreenMargin(320 - sideBarWidth - 2, wideScreenMargin);

    renderableText.Centered(locationMessage, EgaBrightYellow, 144, 148);
    m_renderer.RenderText(renderableText);
}

void GameCatacomb3D::DrawScroll(const uint8_t scrollIndex)
{
    m_renderer.Render2DBar(0, 0, 320, 120, EgaBlack);
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(SCROLLTOPPIC), 0, 0);
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(SCROLL1PIC + scrollIndex), 0, 32);
}

void GameCatacomb3D::DrawFinal()
{
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(FINALEPIC), 0, 0);
}

const uint16_t GameCatacomb3D::GetMenuCursorPic() const
{
    return PSHOT1PIC;
}

const uint16_t GameCatacomb3D::GetNorthIconSprite() const
{
    return 0;
}

const std::string GameCatacomb3D::GetSavedGamesPath() const
{
    return "\\Cat3D";
}

const std::vector<std::vector<uint16_t>> wallSkeletonAnimations = { { 0 } };
const std::vector<std::vector<uint16_t>>& GameCatacomb3D::GetWallSkeletonAnimations() const
{
    return wallSkeletonAnimations;
}

const SDL_Keycode GameCatacomb3D::GetCheatsKeyCode() const
{
    return SDLK_F10;
}

void GameCatacomb3D::PlaySoundBodyExplode()
{
    
}

void GameCatacomb3D::PlaySoundHitGate()
{
    
}

void GameCatacomb3D::PlaySoundPlayerHurt(const int16_t playerHealth)
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

void GameCatacomb3D::PlaySoundGetPotion()
{
    m_audioPlayer->Play(GETPOTIONSND);
}

void GameCatacomb3D::PlaySoundUsePotion()
{
    m_audioPlayer->Play(USEPOTIONSND);
}

void GameCatacomb3D::PlaySoundNoItem()
{
    m_audioPlayer->Play(NOITEMSND);
}

void GameCatacomb3D::PlaySoundGetBolt()
{
    m_audioPlayer->Play(GETBOLTSND);
}

void GameCatacomb3D::PlaySoundGetNuke()
{
    m_audioPlayer->Play(GETNUKESND);
}

void GameCatacomb3D::PlaySoundGetScroll()
{
    m_audioPlayer->Play(GETSCROLLSND);
}

void GameCatacomb3D::PlaySoundGetGem()
{
    // No gems in Catacomb 3-D
}

void GameCatacomb3D::PlaySoundGetKey()
{
    m_audioPlayer->Play(GETKEYSND);
}

void GameCatacomb3D::PlaySoundGetPoints()
{
    m_audioPlayer->Play(GETPOINTSSND);
}

void GameCatacomb3D::PlaySoundUseKey()
{
    m_audioPlayer->Play(USEKEYSND);
}

void GameCatacomb3D::PlaySoundUseBolt()
{
    m_audioPlayer->Play(USEBOLTSND);
}

void GameCatacomb3D::PlaySoundUseNuke()
{
    m_audioPlayer->Play(USENUKESND);
}

void GameCatacomb3D::PlaySoundFreezeTime()
{
    // No freezetime in Catacomb 3-D
}

void GameCatacomb3D::PlaySoundFreezeTimeTick(const int32_t freezeTicks)
{
    // No freezetime in Catacomb 3-D
}

void GameCatacomb3D::PlaySoundWalk(const bool walk1)
{
    m_audioPlayer->Play(walk1 ? WALK1SND : WALK2SND);
}

void GameCatacomb3D::PlaySoundBossDeath()
{
    // TODO
}

void GameCatacomb3D::PlaySoundShoot()
{
    m_audioPlayer->Play(SHOOTSND);
}

void GameCatacomb3D::PlaySoundShootWall()
{
    m_audioPlayer->Play(SHOOTWALLSND);
}

void GameCatacomb3D::PlaySoundBoom()
{
    m_audioPlayer->Play(BIGSHOOTSND);
}

void GameCatacomb3D::PlaySoundSmallMonster()
{
}

void GameCatacomb3D::PlaySoundLargeMonster()
{
}

void GameCatacomb3D::PlaySoundWarpUpOrDown(const bool up)
{
    m_audioPlayer->Play(up ? WARPUPSND : WARPDOWNSND);
}

void GameCatacomb3D::PlaySoundWarp()
{
    // TODO
}

void GameCatacomb3D::PlaySoundPortal()
{

}

static ViewPorts::ViewPortRect3D original3DViewArea = { 0, 144, 264, 144 };

const ViewPorts::ViewPortRect3D& GameCatacomb3D::GetOriginal3DViewArea()
{
    return original3DViewArea;
}

static const ManaBar::ManaBarConfig manaBarConfig = { original3DViewArea.width / 2, 138, EgaBrightWhite, EgaLightGray };

const ManaBar::ManaBarConfig& GameCatacomb3D::GetManaBarConfig()
{
    return manaBarConfig;
}

const CatalogInfo catalogInfo = { "", {"NotAvailable" } };

const CatalogInfo& GameCatacomb3D::GetCatalogInfo() const
{
    return catalogInfo;
}