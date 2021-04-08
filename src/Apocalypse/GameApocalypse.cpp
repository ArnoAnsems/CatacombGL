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

#include "GameApocalypse.h"
#include "EgaGraphApocalypse.h"
#include "GameMapsApocalypse.h"
#include "IntroViewApocalypse.h"
#include "AudioRepositoryApocalypse.h"
#include "DecorateAll.h"
#include "..\Engine\IRenderer.h"
#include "..\Engine\ExtraMenu.h"

static const std::string ApocalypseName = "Catacomb Apocalypse v1.01";

GameApocalypse::GameApocalypse(const std::string gamePath, IRenderer& renderer) :
    m_gameId(4),
    m_gamePath(gamePath),
    m_renderer(renderer),
    m_introView(nullptr),
    m_zombie_base_delay(0)
{
    m_gameMaps = nullptr;
    m_egaGraph = nullptr;
    m_audioRepository = nullptr;
    m_audioPlayer = nullptr;
}

GameApocalypse::~GameApocalypse()
{
    delete m_gameMaps;
    delete m_egaGraph;
    delete m_audioRepository;
    delete m_audioPlayer;
    delete m_introView;
}

void GameApocalypse::SpawnActors(Level* level, const DifficultyLevel difficultyLevel)
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
            {
                const int16_t aquaManDelay = (4 * 60) + rand() % (3 * 60);
                Actor* aquaManActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateAquaMan);
                aquaManActor->SetTemp2(aquaManDelay);
                actors[(y * level->GetLevelWidth()) + x] = aquaManActor;
                break;
            }
            case 13:
            {
                const int16_t blobDelay = (rand() % 60) + (rand() % 100);
                Actor* blobActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateBlob);
                blobActor->SetTemp2(blobDelay);
                actors[(y * level->GetLevelWidth()) + x] = blobActor;
                break;
            }
            case 14:
            {
                Actor* bugActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateBug);
                actors[(y * level->GetLevelWidth()) + x] = bugActor;
                break;
            }
            case 15:
            {
                Actor* demonCyborgActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateCyborgDemon);
                actors[(y * level->GetLevelWidth()) + x] = demonCyborgActor;
                break;
            }
            case 16:
            {
                Actor* shooterEyeActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateShooterEye);
                actors[(y * level->GetLevelWidth()) + x] = shooterEyeActor;
                break;
            }
            case 17:
            {
                Actor* androidMageActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateAndroidMage);
                actors[(y * level->GetLevelWidth()) + x] = androidMageActor;
                break;
            }
            case 18:
            {
                Actor* invisDudeActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateInvisDude);
                actors[(y * level->GetLevelWidth()) + x] = invisDudeActor;
                break;
            }
            case 19:
            {
                Actor* roboTankActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateRoboTank);
                actors[(y * level->GetLevelWidth()) + x] = roboTankActor;
                break;
            }
            case 20:
            {
                Actor* skeletonActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateSkeleton);
                actors[(y * level->GetLevelWidth()) + x] = skeletonActor;
                break;
            }
            case 21:
            {
                Actor* stompyActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateStompy);
                actors[(y * level->GetLevelWidth()) + x] = stompyActor;
                break;
            }
            case 22:
            {
                Actor* trollActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateTroll);
                actors[(y * level->GetLevelWidth()) + x] = trollActor;
                break;
            }
            case 23:
            {
                Actor* wizardActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateWizard);
                actors[(y * level->GetLevelWidth()) + x] = wizardActor;
                break;
            }
            case 24:
            case 25:
            {
                Actor* bounceActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateBounce);
                const actorDirection dir = (tile == 24) ? north : west;
                bounceActor->SetDirection(dir);
                actors[(y * level->GetLevelWidth()) + x] = bounceActor;
                break;
            }
            case 26:
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
            case 27:
            {
                Actor* runningEyeActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateRunningEye);
                int16_t initialDirection = level->GetFloorTile(x, y + 1) >> 8;
                runningEyeActor->SetTemp2(initialDirection);
                actors[(y * level->GetLevelWidth()) + x] = runningEyeActor;
                break;
            }
            case 28:
            {
                const int16_t rayDelay = (rand() % 60) + (rand() % 100);
                Actor* rayActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateRay);
                rayActor->SetTemp2(rayDelay);
                actors[(y * level->GetLevelWidth()) + x] = rayActor;
                break;
            }
            case 29:
            {
                const int16_t timeLordDelay = (rand() % 60) + (rand() % 100);
                Actor* timeLordActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateTimeLord);
                timeLordActor->SetTemp2(timeLordDelay);
                actors[(y * level->GetLevelWidth()) + x] = timeLordActor;
                break;
            }
            case 30:
            {
                Actor* demonActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateDemon);
                actors[(y * level->GetLevelWidth()) + x] = demonActor;
                break;
            }
            case 31:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateColumn5);
                break;
            case 32:
            {
                Actor* fakeWallActor = new Actor(x + 0.5f, y + 0.5f, 0, decorateFakeWall);
                fakeWallActor->SetTemp1(level->GetWallTile(x,y));
                fakeWallActor->SetActive(true);
                actors[(y * level->GetLevelWidth()) + x] = fakeWallActor;
                break;
            }
            case 36:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateColumn1);
                break;
            case 37:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateFutureFirePot);
                break;
            case 38:
            {
                const uint16_t spot = level->GetFloorTile(x + 1, y) >> 8;
                const int16_t portalDelay = spot * 70;
                Actor* portalActor = new Actor(x + 0.5f, y + 0.5f, 0, decoratePortal);
                portalActor->SetTemp2(portalDelay);
                actors[(y * level->GetLevelWidth()) + x] = portalActor;
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
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateColumn2);
                break;
            case 46:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateColumn3);
                break;
            case 47:
            {
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateForceField);
                break;
            }
            case 48:
            {
                Actor* bonusActorOldChest = new Actor(x + 0.5f, y + 0.5f, 0, decorateOldChest);
                actors[(y * level->GetLevelWidth()) + x] = bonusActorOldChest;
                break;
            }
            case 49:
            {
                Actor* bonusActorChest = level->IsWaterLevel() ? new Actor(x + 0.5f, y + 0.5f, 0, decorateWaterChest) : new Actor(x + 0.5f, y + 0.5f, 0, decorateChest);
                actors[(y * level->GetLevelWidth()) + x] = bonusActorChest;
                break;
            }
            case 50:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateColumn4);
                break;
            case 51:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateOldFirePot);
                break;
            case 52:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateTomb1);
                break;
            case 53:
                actors[(y * level->GetLevelWidth()) + x] = new Actor(x + 0.5f, y + 0.5f, 0, decorateTomb2);
                break;

            case 64000:
            {
                // This is a special tile, which is used in Armageddon to indicate that the neigbouring tile contains
                // the floor and ceiling colors. In CatacombGL, these colors are obtained from GameMapsArmageddon.h instead.
                // So we just remove the color encoding and continue.
                level->SetFloorTile(x + 1, y, 0);
                break;
            }
            default:
                break;
            }
        }
    }
}

const DecorateActor& GameApocalypse::GetExplosionActor()
{
    return decorateExplosion;
}

const DecorateActor& GameApocalypse::GetExplodingWallActor()
{
    return decorateExplodingWall;
}

const DecorateActor& GameApocalypse::GetPlayerActor()
{
    return decoratePlayer;
}

GameMaps* GameApocalypse::GetGameMaps()
{
    if (m_gameMaps == nullptr)
    {
        m_gameMaps = new GameMaps(gameMapsApocalypse, m_gamePath);
    }

    return m_gameMaps;
}

EgaGraph* GameApocalypse::GetEgaGraph()
{
    if (m_egaGraph == nullptr)
    {
        m_egaGraph = new EgaGraph(egaGraphApocalypse, m_gamePath, m_renderer);
    }

    return m_egaGraph;
}

AudioRepository* GameApocalypse::GetAudioRepository()
{
    if (m_audioRepository == nullptr)
    {
        m_audioRepository = new AudioRepository(audioRepositoryApocalypse, m_gamePath);
    }

    return m_audioRepository;
}

AudioPlayer* GameApocalypse::GetAudioPlayer()
{
    if (m_audioPlayer == nullptr)
    {
        m_audioPlayer = new AudioPlayer(GetAudioRepository());
    }

    return m_audioPlayer;
}

IIntroView* GameApocalypse::GetIntroView()
{
    if (m_introView == nullptr)
    {
        m_introView = new IntroViewApocalypse(m_renderer, m_gamePath);
    }

    return m_introView;
}

IMenu* GameApocalypse::CreateMenu(ConfigurationSettings& configurationSettings, PlayerInput& playerInput, std::vector<std::string>& savedGames)
{
    return new ExtraMenu(configurationSettings, *GetAudioPlayer(), playerInput, GetEgaGraph(), GetMenuCursorPic(), savedGames, m_renderer, GetCatalogInfo(), m_gamePath);
}

void GameApocalypse::DrawHelpPage()
{
    m_renderer.Render2DBar(0, 0, 640, 200, EgaLightGray);
    const Font& font = *GetEgaGraph()->GetFont(3);
    RenderableText renderableText(font);
    renderableText.LeftAligned("                    SUMMARY OF GAME CONTROLS", EgaBlue, 128, 19);
    renderableText.LeftAligned("         ACTION", EgaRed, 128, 37);
    renderableText.LeftAligned("Arrow keys, joystick, or mouse", EgaRed, 128, 55);
    renderableText.LeftAligned("TAB or V while turning", EgaRed, 128, 64);
    renderableText.LeftAligned("ALT or Button 2 while turning", EgaRed, 128, 73);
    renderableText.LeftAligned("CTRL or Button 1", EgaRed, 128, 82);
    renderableText.LeftAligned("Z", EgaRed, 128, 91);
    renderableText.LeftAligned("X or Enter", EgaRed, 128, 100);
    renderableText.LeftAligned("F1", EgaRed, 128, 109);
    renderableText.LeftAligned("F2", EgaRed, 128, 118);
    renderableText.LeftAligned("F3", EgaRed, 128, 127);
    renderableText.LeftAligned("F4", EgaRed, 128, 136);
    renderableText.LeftAligned("F5", EgaRed, 128, 145);
    renderableText.LeftAligned("ESC", EgaRed, 128, 154);

    renderableText.LeftAligned("   REACTION", EgaDarkGray, 400, 37);
    renderableText.LeftAligned("Move and turn", EgaDarkGray, 400, 55);
    renderableText.LeftAligned("Turn quickly (Quick Turn)", EgaDarkGray, 400, 64);
    renderableText.LeftAligned("Move sideways", EgaDarkGray, 400, 73);
    renderableText.LeftAligned("Shoot a Missile", EgaDarkGray, 400, 82);
    renderableText.LeftAligned("Shoot a Zapper", EgaDarkGray, 400, 91);
    renderableText.LeftAligned("Shoot an Xterminator", EgaDarkGray, 400, 100);
    renderableText.LeftAligned("Help (this screen)", EgaDarkGray, 400, 109);
    renderableText.LeftAligned("Sound control", EgaDarkGray, 400, 118);
    renderableText.LeftAligned("Save game position", EgaDarkGray, 400, 127);
    renderableText.LeftAligned("Restore a saved game", EgaDarkGray, 400, 136);
    renderableText.LeftAligned("Joystick control", EgaDarkGray, 400, 145);
    renderableText.LeftAligned("System options", EgaDarkGray, 400, 154);

    m_renderer.RenderText(renderableText);
}

bool GameApocalypse::ProcessInputOnHelpPage(PlayerInput& playerInput)
{
    return (playerInput.GetFirstKeyPressed() != SDLK_UNKNOWN);
}

const std::map<uint16_t, const DecorateActor>& GameApocalypse::GetDecorateActors() const
{
    return decorateApocalypseAll;
}

const std::string& GameApocalypse::GetName() const
{
    return ApocalypseName;
}

const uint8_t GameApocalypse::GetId() const
{
    return m_gameId;
}

void GameApocalypse::DrawStatusBar(const int16_t health, const std::string& locationMessage, const PlayerInventory& playerInventory, const uint16_t wideScreenMargin, const float /*playerAngle*/, const uint8_t /*levelIndex*/, const uint16_t /*shotPower*/, const uint32_t /*points*/)
{
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(egaGraphicsApocalypse::STATUSPIC), 0, 120);
    const Font& font = *GetEgaGraph()->GetFont(3);
    RenderableText renderableText(font);
    RenderableTiles tiles(*GetEgaGraph()->GetTilesSize8());
    DrawHealth(tiles, health);
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

void GameApocalypse::DrawHealth(RenderableTiles& renderableTiles, const int16_t health)
{
    const uint16_t percentage = (uint16_t)health;

    renderableTiles.DrawNumberRightAligned(112, 177, health);

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

void GameApocalypse::DrawKeys(RenderableTiles& renderableTiles, const PlayerInventory& playerInventory)
{
    renderableTiles.DrawNumberRightAligned(208, 150, playerInventory.GetKeys(RedKey));
    renderableTiles.DrawNumberRightAligned(232, 177, playerInventory.GetKeys(YellowKey));
    renderableTiles.DrawNumberRightAligned(232, 150, playerInventory.GetKeys(GreenKey));
    renderableTiles.DrawNumberRightAligned(208, 177, playerInventory.GetKeys(BlueKey));
}

void GameApocalypse::DrawBonus(RenderableTiles& renderableTiles, const PlayerInventory& playerInventory)
{
    renderableTiles.DrawNumberRightAligned(176, 138, playerInventory.GetBolts());
    renderableTiles.DrawNumberRightAligned(176, 156, playerInventory.GetNukes());
    renderableTiles.DrawNumberRightAligned(176, 174, playerInventory.GetPotions());
}

void GameApocalypse::DrawGems(const PlayerInventory& playerInventory)
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

void GameApocalypse::DrawScroll(const uint8_t scrollIndex)
{
    // No scrolls in Apocalypse
}

void GameApocalypse::DrawFinal()
{
    m_renderer.Render2DPicture(GetEgaGraph()->GetPicture(FINALEPIC), 0, 0);
}

const uint16_t GameApocalypse::GetMenuCursorPic() const
{
    return SKULL_SHOTPIC;
}

const uint16_t GameApocalypse::GetNorthIconSprite() const
{
    return NORTHICONSPR;
}

const std::string GameApocalypse::GetSavedGamesPath() const
{
    return "\\Apocalypse";
}

const std::vector<std::vector<uint16_t>> wallSkeletonAnimations = { { 0 } };
const std::vector<std::vector<uint16_t>>& GameApocalypse::GetWallSkeletonAnimations() const
{
    // TODO
    return wallSkeletonAnimations;
}

const SDL_Keycode GameApocalypse::GetCheatsKeyCode() const
{
    return SDLK_BACKSPACE;
}

void GameApocalypse::PlaySoundBodyExplode()
{
    m_audioPlayer->Play(BODY_EXPLODESND);
}

void GameApocalypse::PlaySoundHitGate()
{
    m_audioPlayer->Play(HIT_GATESND);
}

void GameApocalypse::PlaySoundPlayerHurt(const int16_t playerHealth)
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

void GameApocalypse::PlaySoundGetPotion()
{
    m_audioPlayer->Play(GETPOTIONSND);
}

void GameApocalypse::PlaySoundUsePotion()
{
    m_audioPlayer->Play(USEPOTIONSND);
}

void GameApocalypse::PlaySoundNoItem()
{
    m_audioPlayer->Play(NOITEMSND);
}

void GameApocalypse::PlaySoundGetBolt()
{
    m_audioPlayer->Play(GETBOLTSND);
}

void GameApocalypse::PlaySoundGetNuke()
{
    m_audioPlayer->Play(GETNUKESND);
}

void GameApocalypse::PlaySoundGetScroll()
{
    // No scrolls in Apocalypse
}

void GameApocalypse::PlaySoundGetGem()
{
    m_audioPlayer->Play(GETGEMSND);
}

void GameApocalypse::PlaySoundGetKey()
{
    m_audioPlayer->Play(GETKEYSND);
}

void GameApocalypse::PlaySoundGetPoints()
{
    m_audioPlayer->Play(GETPOINTSSND);
}

void GameApocalypse::PlaySoundUseKey()
{
    m_audioPlayer->Play(USEKEYSND);
}

void GameApocalypse::PlaySoundUseBolt()
{
    m_audioPlayer->Play(USEBOLTSND);
}

void GameApocalypse::PlaySoundUseNuke()
{
    m_audioPlayer->Play(USENUKESND);
}

void GameApocalypse::PlaySoundFreezeTime()
{
    m_audioPlayer->Play(FREEZETIMESND);
}

void GameApocalypse::PlaySoundFreezeTimeTick(const int32_t freezeTicks)
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

void GameApocalypse::PlaySoundWalk(const bool walk1)
{
    m_audioPlayer->Play(walk1 ? WALK1SND : WALK2SND);
}

void GameApocalypse::PlaySoundBossDeath()
{
    m_audioPlayer->Play(GRELM_DEADSND);
}

void GameApocalypse::PlaySoundShoot()
{
    m_audioPlayer->Play(SHOOTSND);
}

void GameApocalypse::PlaySoundShootWall()
{
    m_audioPlayer->Play(SHOOTWALLSND);
}

void GameApocalypse::PlaySoundBoom()
{
    m_audioPlayer->Play(BOOMSND);
}

void GameApocalypse::PlaySoundSmallMonster()
{
    m_audioPlayer->Play(SMALLMONSTERSND);
}

void GameApocalypse::PlaySoundLargeMonster()
{
    m_audioPlayer->Play(LARGEMONSTERSND);
}

void GameApocalypse::PlaySoundWarpUpOrDown(const bool up)
{
    m_audioPlayer->Play(up ? WARPUPSND : WARPDOWNSND);
}

void GameApocalypse::PlaySoundWarp()
{
    m_audioPlayer->Play(WARPSND);
}

void GameApocalypse::PlaySoundPortal()
{
    m_audioPlayer->Play(PORTALSND);
}

static ViewPorts::ViewPortRect3D original3DViewArea = { 0, 120, 320, 120 };

const ViewPorts::ViewPortRect3D& GameApocalypse::GetOriginal3DViewArea()
{
    return original3DViewArea;
}

static const ManaBar::ManaBarConfig manaBarConfig = { original3DViewArea.width / 2, 114, EgaBrightCyan, EgaCyan };

const ManaBar::ManaBarConfig& GameApocalypse::GetManaBarConfig()
{
    return manaBarConfig;
}

const CatalogInfo catalogInfo =
{
    "Hint Book"
,
{
    "HINT01.HNT",
    "HINT02.HNT",
    "HINT03.HNT",
    "HINT04.HNT",
    "HINT05.HNT",
    "HINT06.HNT",
    "HINT07.HNT",
    "HINT08.HNT",
    "HINT09.HNT",
    "HINT10.HNT",
    "HINT11.HNT",
    "HINT12.HNT",
    "HINT13.HNT",
    "HINT14.HNT",
    "HINT15.HNT",
    "HINT16.HNT",
    "HINT17.HNT",
    "HINT18.HNT",
    "HINT19.HNT"
} };

const CatalogInfo& GameApocalypse::GetCatalogInfo() const
{
    return catalogInfo;
}
