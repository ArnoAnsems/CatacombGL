// Copyright (C) 1993-2014 Flat Rock Software
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

#include "EngineCore.h"
#include "../../ThirdParty/ReflectionHLE/id_sd.h"
#include "DefaultFont.h"
#include "LevelLocationNames.h"
#include "Macros.h"
#include "RenderableTiles.h"
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

constexpr uint8_t versionMajor = 0;
constexpr uint8_t versionMinor = 5;
constexpr uint8_t versionLevel = 5;
const std::string versionPhase = "Beta";

constexpr uint8_t VictoryStatePlayGetBolt = 0;
constexpr uint8_t VictoryStatePlayingGetBolt = 1;
constexpr uint8_t VictoryStatePlayGetNuke = 2;
constexpr uint8_t VictoryStatePlayingGetNuke = 3;
constexpr uint8_t VictoryStatePlayGetPotion = 4;
constexpr uint8_t VictoryStatePlayingGetPotion = 5;
constexpr uint8_t VictoryStatePlayGetKey = 6;
constexpr uint8_t VictoryStatePlayingGetKey = 7;
constexpr uint8_t VictoryStatePlayGetScroll = 8;
constexpr uint8_t VictoryStatePlayingGetScroll = 9;
constexpr uint8_t VictoryStatePlayGetPoint = 10;
constexpr uint8_t VictoryStatePlayingGetPoint = 11;
constexpr uint8_t VictoryStateDone = 12;

constexpr uint8_t NotReadingAnyScroll = 255u;

const float aspectRatios[2] =
{
    4.0f / 3.0f,
    10.0f 
};

EngineCore::EngineCore(IGame& game, const ISystem& system, PlayerInput& keyboardInput, ConfigurationSettings& configurationSettings) :
    m_game(game),
    m_configurationSettings(configurationSettings),
    m_timeStampOfPlayerCurrentFrame(0),
    m_timeStampOfPlayerPreviousFrame(0),
    m_timeStampOfWorldCurrentFrame(0),
    m_timeStampOfWorldPreviousFrame(0),
    m_level(nullptr),
    m_playerInventory(game),
    m_difficultyLevel(Easy),
    m_godModeIsOn(false),
    m_playerInput(keyboardInput),
    m_keyToTake(KeyId::NoKey),
    m_victoryState(VictoryStatePlayGetBolt),
    m_state(Introduction),
    m_statusMessage(nullptr),
    m_readingScroll(255),
    m_takingChest(false),
    m_warpToLevel(0),
    m_warpCheatTextField(""),
    m_playerActions(),
    m_lastFreezeTimeTick(0),
    m_timeStampEndOfStatusMessage(0),
    m_startTakeKey(0),
    m_timeStampToEnterGame(0),
    m_timeStampLastMouseMoveForward(0),
    m_timeStampLastMouseMoveBackward(0),
    m_timeStampFadeEffect(0),
    m_gameTimer(),
    m_system(system),
    m_savedGames(),
    m_scrollsArePresent(AreScrollsPresent()),
    m_setOverlayOnNextDraw(false),
    m_renderable3DScene(m_game.GetOriginal3DViewArea()),
    m_renderableAutoMapIso(*m_game.GetEgaGraph()->GetFont(3), m_game.GetOriginal3DViewArea()),
    m_renderableAutoMapTopDown(*m_game.GetEgaGraph()->GetFont(3), m_game.GetOriginal3DViewArea(), *m_game.GetEgaGraph()->GetTilesSize16(), *m_game.GetEgaGraph()->GetTilesSize16Masked()),
    m_manaBar(m_game.GetManaBarConfig()),
    m_overscanBorder(),
    m_renderableOverscanBorder(m_overscanBorder),
    m_insideBorderFlashLocation(false),
    m_levelStatistics(),
    m_renderableLevelStatistics(m_levelStatistics),
    m_savedGamesInDosFormat(m_game.GetSavedGameInDosFormatConfig())
{
    m_messageInPopup[0] = 0;
    m_gameTimer.Reset();

    const fs::path filenamePath = m_system.GetConfigurationFilePath();
    const fs::path savedGamesPath = filenamePath / m_game.GetSavedGamesPath();
    m_system.GetSavedGameNamesFromFolder(savedGamesPath, m_savedGames);

#ifdef SAVEDGAMESINDOSFORMAT
    if (game.GetId() == 5)
    {
        // Retrieve Catacomb 3-D saved DOS games
        const fs::path& gameDataPath = configurationSettings.GetCVarString(CVarIdPathCatacomb3Dv122).Get();
        for (uint8_t index = 0; index < 10; index++)
        {
            const fs::path fullFilename = gameDataPath / ( "SAVEGAM" + std::to_string(index) + ".C3D");
            std::ifstream file;
            file.open(fullFilename, std::ifstream::binary);
            if (file.is_open())
            {
                const std::streampos beginPos = file.tellg();
                file.seekg(0, std::ios::end);
                const int32_t fileSize = (int32_t)(file.tellg() - beginPos);
                FileChunk* fileChunk = new FileChunk(fileSize);
                file.seekg(0, std::ios::beg);
                file.read((char*)fileChunk->GetChunk(), fileSize);
                file.close();
                m_savedGamesInDosFormat.AddSavedGame(fileChunk);
                delete fileChunk;
            }
        }
    }
    else
    {
        // Retrieve Catacomb Adventure Trilogy saved DOS games
        const fs::path& gameDataPath =
            (game.GetId() == 1) ? configurationSettings.GetCVarString(CVarIdPathAbyssv113).Get() :
            (game.GetId() == 2) ? configurationSettings.GetCVarString(CVarIdPathAbyssv124).Get() :
            (game.GetId() == 3) ? configurationSettings.GetCVarString(CVarIdPathArmageddonv102).Get() :
            configurationSettings.GetCVarString(CVarIdPathApocalypsev101).Get();
        std::vector<std::string> filesFound;
        m_system.GetSavedGameNamesFromFolder(gameDataPath, filesFound);
        for (const auto& filename : filesFound)
        {
            const fs::path fullFilename = gameDataPath / ( filename + ".SAV" );
            std::ifstream file;
            file.open(fullFilename, std::ifstream::binary);
            if (file.is_open())
            {
                const std::streampos beginPos = file.tellg();
                file.seekg(0, std::ios::end);
                const int32_t fileSize = (int32_t)(file.tellg() - beginPos);
                FileChunk* fileChunk = new FileChunk(fileSize);
                file.seekg(0, std::ios::beg);
                file.read((char*)fileChunk->GetChunk(), fileSize);
                file.close();
                m_savedGamesInDosFormat.AddSavedGame(fileChunk, filename);
                delete fileChunk;
            }
        }
    }
#endif
    m_menu = game.CreateMenu(configurationSettings, keyboardInput, m_savedGames, m_savedGamesInDosFormat);

    // Pre-load game data from disk
    m_game.GetAudioRepository();
    m_game.GetGameMaps();
}

EngineCore::~EngineCore()
{
    UnloadLevel();

    delete m_menu;
}

const std::string EngineCore::GetVersionInfo()
{
    return std::to_string(versionMajor) + "." +
        std::to_string(versionMinor) + "." +
        std::to_string(versionLevel) + " " +
        versionPhase;
}

void EngineCore::LoadLevel(const uint8_t mapIndex)
{
    // The playerActor will be unloaded and reloaded in the next map. The health of the playerActor
    // must be preserved when transfering to the next map.
    const int16_t health = (m_level == nullptr) ? 100 : m_level->GetPlayerActor()->GetHealth();
    UnloadLevel();

    m_level = m_game.GetGameMaps()->GetLevelFromStart(mapIndex);

    // Fix bugs in levels by manipulating the tiles
    if (m_configurationSettings.GetCVarBool(CVarIdPreventSoftlock).IsEnabled())
    {
        // Prevent soft lock in The Town of Morbidity
        if (m_game.GetId() == GameId::CatacombArmageddonv102 && m_level->GetLevelIndex() == 0)
        {
            // Change yellow key to red key
            m_level->SetFloorTile(29, 24, 8);

            // Change yellow door to red door
            m_level->SetFloorTile(2, 19, 256);
        }
    }

    // Remove unreachable skeleton in Mike's Blastable Passage
    if ((m_game.GetId() == GameId::CatacombAbyssv113 || m_game.GetId() == GameId::CatacombAbyssv124) && m_level->GetLevelIndex() == 5)
    {
        m_level->SetFloorTile(7, 17, 0);
    }

    m_game.SpawnActors(m_level, m_difficultyLevel);
    m_level->GetPlayerActor()->SetHealth(health);
    m_autoMap.ResetOrigin(*m_level, m_configurationSettings.GetCVarEnum(CVarIdAutoMapMode).GetItemIndex());
    m_levelStatistics.SetCountersAtStartOfLevel(*m_level);

    m_timeStampOfPlayerCurrentFrame = 0;
    m_timeStampOfPlayerPreviousFrame = 0;
    m_timeStampOfWorldCurrentFrame = 0;
    m_timeStampOfWorldPreviousFrame = 0;
    m_timeStampLastMouseMoveForward = 0;
    m_timeStampLastMouseMoveBackward = 0;
    m_gameTimer.Reset();
    m_playerActions.ResetForNewLevel();
    m_manaBar.Reset(m_configurationSettings.GetCVarBool(CVarIdManaBar).IsEnabled());
    if (m_difficultyLevel == Easy)
    {
        DisplayStatusMessage("*** NOVICE ***", 3000);
    }
    else
    {
        DisplayStatusMessage("*** WARRIOR ***", 3000);
    }
    m_warpToLevel = mapIndex;
}

void EngineCore::DrawScene(IRenderer& renderer)
{
    m_framesCounter.AddFrame(m_gameTimer.GetActualTime());

    if (m_setOverlayOnNextDraw)
    {
        m_fadeEffect.SetOverlay(renderer);
        m_timeStampFadeEffect = m_gameTimer.GetActualTime();
        m_setOverlayOnNextDraw = false;
        m_gameTimer.Pause();
    }

    IRenderer::FrameSettings frameSettings;
    frameSettings.textureFilter = (m_configurationSettings.GetCVarEnum(CVarIdTextureFilter).GetItemIndex() == CVarItemIdTextureFilterNearest) ? IRenderer::Nearest : IRenderer::Linear;
    frameSettings.vSyncEnabled = m_configurationSettings.GetCVarBool(CVarIdVSync).IsEnabled();
    renderer.SetFrameSettings(frameSettings);

    if (m_state == AutoMapDialog && m_level != nullptr)
    {
        const float aspectRatio = aspectRatios[m_configurationSettings.GetCVarEnum(CVarIdAspectRatio).GetItemIndex()];
        const uint8_t autoMapMode = m_configurationSettings.GetCVarEnum(CVarIdAutoMapMode).GetItemIndex();
        if (autoMapMode == CVarItemIdAutoMapIsometric)
        {
            m_autoMap.SetupIso(m_renderableAutoMapIso, *m_game.GetEgaGraph(), *m_level, aspectRatio);
            renderer.RenderAutoMapIso(m_renderableAutoMapIso);
        }
        else if (autoMapMode == CVarItemIdAutoMapTopDown || autoMapMode == CVarItemIdAutoMapTopDownHD)
        {
            const uint16_t tileSize = (autoMapMode == CVarItemIdAutoMapTopDown) ? 16 : 64;
            m_autoMap.SetupTopDown(m_renderableAutoMapTopDown, *m_game.GetEgaGraph(), *m_level, aspectRatio, tileSize, renderer.GetAdditionalMarginDueToWideScreen(aspectRatio));
            renderer.RenderAutoMapTopDown(m_renderableAutoMapTopDown);
        }
    }
    else
    {
        if (!m_menu->IsActive() || m_game.GetId() != GameId::Catacomb3Dv122)
        {
            if (m_readingScroll == NotReadingAnyScroll && (m_state == InGame || m_state == WarpCheatDialog || m_state == GodModeCheatDialog || m_state == FreeItemsCheatDialog || m_state == AutoMapDialog || (m_state == Victory && m_victoryState != VictoryStateDone) || m_state == VerifyGateExit))
            {
                m_renderable3DScene.PrepareFrame(
                    aspectRatios[m_configurationSettings.GetCVarEnum(CVarIdAspectRatio).GetItemIndex()],
                    m_level->GetPlayerActor()->GetX(),
                    m_level->GetPlayerActor()->GetY(),
                    m_level->GetPlayerActor()->GetAngle(),
                    m_configurationSettings.GetCVarBool(CVarIdDepthShading).IsEnabled(),
                    m_configurationSettings.GetCVarInt(CVarIdFov).GetValue(),
                    renderer.IsOriginalScreenResolutionSupported() && m_configurationSettings.GetCVarEnum(CVarIdScreenResolution).GetItemIndex() == CVarItemIdScreenResolutionOriginal);
                m_level->Setup3DScene(
                    *m_game.GetEgaGraph(),
                    m_renderable3DScene,
                    m_timeStampOfWorldCurrentFrame,
                    m_gameTimer.GetTicksForWorld()
                );
                m_renderable3DScene.FinalizeFrame();

                renderer.Render3DScene(m_renderable3DScene);
            }
        }
    }

    renderer.Prepare2DRendering(m_state == Help);

    if (m_state == Help)
    {
        DrawTiledWindow(renderer, 1, 1, 78, 23);
        m_game.DrawHelpPage();
    }

    if (m_state == InGame)
    {
        if (m_readingScroll != NotReadingAnyScroll)
        {
            // Read scroll
            m_game.DrawScroll(m_readingScroll);
        }
        else
        {
            if (m_playerActions.GetHandHeight() > 0)
            {
                const uint16_t statusbarOffset = (m_game.GetId() == GameId::Catacomb3Dv122) ? 144 : 120;
                uint16_t pictureIndex = m_game.GetEgaGraph()->GetHandPictureIndex();
                // When shot power is building up in Catacomb 3D, check if the hand with the fire effect needs to be shown.
                if (m_game.GetId() == GameId::Catacomb3Dv122 && m_playerActions.GetShotPower() > 0 && m_gameTimer.GetTicksForPlayer() % 16 < 8)
                {
                    pictureIndex++;
                }
                const Picture* handPicture = m_game.GetEgaGraph()->GetMaskedPicture(pictureIndex);
                const int16_t offsetX = (m_game.GetId() == GameId::Catacomb3Dv122) ? 92 : 120;
                // Draw only the segment of the hand graphic that is visible above the statusbar.
                renderer.Render2DPictureSegment(handPicture, offsetX, statusbarOffset - m_playerActions.GetHandHeight(), 0, 0, handPicture->GetImageWidth(), m_playerActions.GetHandHeight());
            }

            m_manaBar.Draw(renderer);

#ifdef DRAWTILEINFO
            char tileStr[40];
            std::snprintf(tileStr, 40, "tile: %d", m_level->GetFloorTile((uint16_t)m_level->GetPlayerActor()->GetX(), (uint16_t)m_level->GetPlayerActor()->GetY()));
            renderer.RenderTextLeftAligned(tileStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,2,2);
            std::snprintf(tileStr, 40, "wall: %d", m_level->GetWallTile((uint16_t)m_level->GetPlayerActor()->GetX(), (uint16_t)m_level->GetPlayerActor()->GetY()));
            renderer.RenderTextLeftAligned(tileStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,2,12);
            std::snprintf(tileStr, 40, "X: %d Y: %d Angle: %d", (uint16_t)m_level->GetPlayerActor()->GetX(), (uint16_t)m_level->GetPlayerActor()->GetY(), (uint16_t)m_level->GetPlayerActor()->GetAngle());
            renderer.RenderTextLeftAligned(tileStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,2,22);
            std::snprintf(tileStr, 40, "MouseX: %d", m_playerInput.GetMouseXPos());
            renderer.RenderTextLeftAligned(tileStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,2,32);
#endif
        }
    }
    std::string locationMessage("");
    if (m_state == InGame || m_state == Victory || m_state == VerifyGateExit)
    {
        if (m_level->GetPlayerActor()->IsDead() ||
            ((m_state == Victory) && (m_victoryState == VictoryStateDone)))
        {
            locationMessage = "Press ESC to open the menu.";
        }
        else if (m_state == VerifyGateExit)
        {
            locationMessage = "Pass this way?      Y/N";
        }
        else if (m_readingScroll != NotReadingAnyScroll)
        {
            // The original game also gave the option to close the scroll via ESC, but CatacombGL has ESC reserved for the new menu.
            locationMessage = "Press ENTER to exit.";
        }
        else
        {
            const uint16_t floorTile = m_level->GetWallTile((uint16_t)(m_level->GetPlayerActor()->GetX()), (uint16_t)(m_level->GetPlayerActor()->GetY()));
            if (floorTile > 180 && m_level->GetLevelIndex() < m_game.GetEgaGraph()->GetNumberOfWorldLocationNames())
            {
                locationMessage = m_game.GetEgaGraph()->GetWorldLocationNames(m_level->GetLevelIndex())->GetLocationName(floorTile - 180);
                const bool borderShouldFlash = m_game.GetEgaGraph()->GetWorldLocationNames(m_level->GetLevelIndex())->BorderShouldFlash(floorTile - 180);
                if (!m_insideBorderFlashLocation && borderShouldFlash)
                {
                    m_insideBorderFlashLocation = true;
                    const uint32_t borderFlashTime = (uint32_t)(20 * (1000.0 / 70.0));
                    m_overscanBorder.SetColor(m_gameTimer.GetActualTime(), 15 | 56, borderFlashTime);

                }
                else if (m_insideBorderFlashLocation && !borderShouldFlash)
                {
                    m_insideBorderFlashLocation = false;
                }
            }
        }
    }

    if (((m_state == InGame || m_state == EnteringLevel || m_state == WarpCheatDialog || m_state == GodModeCheatDialog || m_state == FreeItemsCheatDialog || m_state == AutoMapDialog || m_state == VerifyGateExit || m_state == ExitGame) && (!m_menu->IsActive() || m_game.GetId() != GameId::Catacomb3Dv122)) || (m_state == Victory && m_game.GetId() != GameId::Catacomb3Dv122))
    {
        const int16_t playerHealth = (m_level != 0) ? m_level->GetPlayerActor()->GetHealth() : 100;
        const float playerAngle = (m_level != 0) ? m_level->GetPlayerActor()->GetAngle() : 0.0f;
        const uint8_t levelIndex = (m_level != 0) ? m_level->GetLevelIndex() : 0;
        m_game.DrawStatusBar(playerHealth, locationMessage, m_playerInventory, renderer.GetAdditionalMarginDueToWideScreen(aspectRatios[m_configurationSettings.GetCVarEnum(CVarIdAspectRatio).GetItemIndex()]), playerAngle, levelIndex, m_playerActions.GetShotPower(), m_score.GetPoints());

        if (m_state != Victory)
        {
            if (m_game.GetId() != GameId::Catacomb3Dv122)  // Status message and radar not in Catacomb 3-D
            {
                const int32_t remainingFreezeTime = m_gameTimer.GetRemainingFreezeTime();
                const int32_t remainingFreezeTimeTicks = (remainingFreezeTime * 70) / 30000;
                if (remainingFreezeTimeTicks != m_lastFreezeTimeTick)
                {
                    m_lastFreezeTimeTick = remainingFreezeTimeTicks;
                    m_game.PlaySoundFreezeTimeTick(m_lastFreezeTimeTick);
                }
                char freezeMessage[100];
                std::snprintf(freezeMessage, 100, "Time Stopped: %d", remainingFreezeTimeTicks);
                const char* statusMessage = (m_statusMessage != nullptr) ? m_statusMessage : (remainingFreezeTime != 0) ? freezeMessage : m_playerActions.GetStatusMessage();
                RenderableText renderableText(*m_game.GetEgaGraph()->GetFont(3));
                renderableText.Centered(statusMessage, EgaBrightYellow, 156, 189);
                renderer.RenderText(renderableText);

                // Radar
                if (m_state == InGame || m_state == WarpCheatDialog || m_state == GodModeCheatDialog || m_state == FreeItemsCheatDialog || m_state == AutoMapDialog || m_state == VerifyGateExit)
                {
                    const float radarCenterX = 276.5f;
                    const float radarCenterY = 153.5f;
                    const float radarXRadius = 21.0f;
                    const float radarYRadius = 15.0f;
                    const float playerAngleInRadians = m_level->GetPlayerActor()->GetAngle() * 3.14159265f / 180.0f;
                    const float northIconoffsetFromCenterX = radarXRadius * sin(playerAngleInRadians);
                    const float northIconoffsetFromCenterY = radarYRadius * cos(playerAngleInRadians);
                    const uint16_t northIconScreenOffsetX = (uint16_t)(radarCenterX - northIconoffsetFromCenterX - 4.5f);
                    const uint16_t northIconScreenOffsetY = (uint16_t)(radarCenterY - northIconoffsetFromCenterY - 2.5f);
                    renderer.Render2DPicture(m_game.GetEgaGraph()->GetSprite(m_game.GetNorthIconSprite()), northIconScreenOffsetX, northIconScreenOffsetY);
                    renderer.Render2DBar((int16_t)radarCenterX, (int16_t)radarCenterY, 1u, 1u, EgaBrightWhite);

                    for (uint16_t blipIndex = 0; blipIndex < m_radarModel.GetNumberOfBlips(); blipIndex++)
                    {
                        const radarBlip blip = m_radarModel.GetRadarBlip(blipIndex);
                        renderer.Render2DBar((int16_t)(radarCenterX + blip.offsetX), (int16_t)(radarCenterY + blip.offsetY), 1u, 1u, blip.color);
                    }
                }
            }
        }
    }
    
    if ( m_state == EnteringLevel)
    {
        const uint16_t margin = renderer.GetAdditionalMarginDueToWideScreen(aspectRatios[m_configurationSettings.GetCVarEnum(CVarIdAspectRatio).GetItemIndex()]);
        RenderableText renderableText(*m_game.GetEgaGraph()->GetFont(3));
        if (m_game.GetId() == GameId::Catacomb3Dv122)
        {
            // Catacomb 3-D
            renderer.Render2DBar(0 - margin, 0, 264 + (2 * margin), 144, EgaBrightBlue);
            renderer.Render2DPicture(m_game.GetEgaGraph()->GetPicture(159), 57, 52);
            renderableText.Centered(m_level->GetLevelName(), EgaDarkGray, 132, 76);
        }
        else
        {
            // Catacomb Adventure series
            renderer.Render2DBar(0 - margin, 0, 320 + (2 * margin), 120, EgaBlack);
            uint16_t width = (uint16_t)strlen(m_level->GetLevelName());
            if (width == 0)
            {

                const char* enterAreaText = (m_game.GetId() == GameId::CatacombArmageddonv102) ? "You enter a new area ..." : "A new challenge awaits you.";
                width = (uint16_t)strlen(enterAreaText);
                DrawCenteredTiledWindow(renderer, width, 3);
                renderableText.Centered(enterAreaText, EgaDarkGray, 160, 56);
            }
            else
            {
                if (width < 20)
                {
                    width = 20;
                }
                DrawCenteredTiledWindow(renderer, width, 5);
                renderableText.Centered("You have arrived at", EgaDarkGray, 160, 49);
                renderableText.Centered(m_level->GetLevelName(), EgaDarkGray, 160, 58);
            }
        }
        renderer.RenderText(renderableText);
    }

    if (m_state == InGame)
    {
        if (STR_CASE_CMP(m_messageInPopup, "") != 0)
        {
            DrawCenteredTiledWindow(renderer, 20, 4);
            RenderableText renderableText(*m_game.GetEgaGraph()->GetFont(3));
            renderableText.Centered(m_messageInPopup, EgaDarkGray, 160, 49);
            renderer.RenderText(renderableText);
        }
    }

    if (m_state == WarpCheatDialog)
    {
        DrawCenteredTiledWindow(renderer, 26, 3);
        // In Catacomb 3-D, the on-screen level indices start at 1 instead of 0.
        const bool onScreenLevelIndexStartsAtZero = (m_game.GetId() != GameId::Catacomb3Dv122);
        const uint8_t firstLevel = onScreenLevelIndexStartsAtZero ? 0 : 1;
        const uint8_t totalNumberOfLevels = m_game.GetGameMaps()->GetNumberOfLevels();
        const uint8_t lastLevel = onScreenLevelIndexStartsAtZero ? totalNumberOfLevels - 1 : totalNumberOfLevels;
        const std::string warpText = "Warp to which level(" + std::to_string(firstLevel) + "-" + std::to_string(lastLevel) + "):" + m_warpCheatTextField;
        RenderableText renderableText(*m_game.GetEgaGraph()->GetFont(3));
        renderableText.LeftAligned(warpText, EgaDarkGray, 70, 56);
        renderer.RenderText(renderableText);
    }

    if (m_state == AutoMapDialog && m_level != nullptr && m_configurationSettings.GetCVarEnum(CVarIdAutoMapMode).GetItemIndex() == CVarItemIdAutoMapOriginal)
    {
        m_autoMap.DrawClassic(renderer, *m_game.GetEgaGraph(), *m_level, renderer.GetAdditionalMarginDueToWideScreen(aspectRatios[m_configurationSettings.GetCVarEnum(CVarIdAspectRatio).GetItemIndex()]));
    }

    if (m_state == AutoMapDialog && m_level != nullptr)
    {
        const uint8_t autoMapMode = m_configurationSettings.GetCVarEnum(CVarIdAutoMapMode).GetItemIndex();
        if (autoMapMode != CVarItemIdAutoMapOriginal)
        {
            const float aspectRatio = aspectRatios[m_configurationSettings.GetCVarEnum(CVarIdAspectRatio).GetItemIndex()];
            m_renderableLevelStatistics.Draw(
                renderer,
                *m_game.GetEgaGraph()->GetFont(3),
                renderer.GetAdditionalMarginDueToWideScreen(aspectRatio),
                m_gameTimer.GetMillisecondsForPlayer(),
                m_level->GetGroundColor());
        }
    }

    if (m_state == GodModeCheatDialog)
    {
        DrawCenteredTiledWindow(renderer, 12, 2);
        const std::string godModeText = m_godModeIsOn ? "God mode ON" : "God mode OFF";
        RenderableText renderableText(*m_game.GetEgaGraph()->GetFont(3));
        renderableText.Centered(godModeText, EgaDarkGray, 160, 52);
        renderer.RenderText(renderableText);
    }

    if (m_state == FreeItemsCheatDialog)
    {
        DrawCenteredTiledWindow(renderer, 12, 2);
        RenderableText renderableText(*m_game.GetEgaGraph()->GetFont(3));
        renderableText.Centered("Free items!", EgaDarkGray, 160, 52);
        renderer.RenderText(renderableText);
    }

    if (m_level != nullptr && m_level->GetPlayerActor()->IsDead() && m_playerInventory.GetPotions() > 0 && m_game.GetId() != GameId::Catacomb3Dv122)
    {
        DrawCenteredTiledWindow(renderer, 35, 3);
        RenderableText renderableText(*m_game.GetEgaGraph()->GetFont(3));
        renderableText.Centered("You should use your Cure Potions wisely", EgaDarkGray, 160, 56);
        renderer.RenderText(renderableText);
    }

    if (m_state == Victory && m_victoryState == VictoryStateDone)
    {
        m_game.DrawFinal();
    }
    
    if (m_state == Introduction)
    {
        m_game.GetIntroView()->DrawIntroduction(m_timeStampOfPlayerCurrentFrame);
    }
    if (m_state == RequestDifficultyLevel)
    {
        m_game.GetIntroView()->DrawRequestDifficultyLevel();
    }
    if (m_state == ShowDifficultyLevel && m_difficultyLevel == Easy)
    {
        m_game.GetIntroView()->DrawNoviceSelected();
    }
    if (m_state == ShowDifficultyLevel && m_difficultyLevel == Hard)
    {
        m_game.GetIntroView()->DrawWarriorSelected();
    }
    if (m_state == StandBeforeGate)
    {
        m_game.GetIntroView()->DrawStandBeforeGate();
    }

    if (m_state == InGame || m_state == EnteringLevel)
    {
        if (m_timeStampFadeEffect + 1000 > m_gameTimer.GetActualTime())
        {
            m_fadeEffect.DrawOverlay(renderer, m_gameTimer.GetActualTime() - m_timeStampFadeEffect);
        }
        else if (m_timeStampFadeEffect != 0 && !m_menu->IsActive() && (STR_CASE_CMP(m_messageInPopup, "") == 0) && m_readingScroll == NotReadingAnyScroll && m_level != nullptr && !m_level->GetPlayerActor()->IsDead())
        {
            m_timeStampFadeEffect = 0;
            m_gameTimer.Resume();
        }
    }

    if (m_menu->IsActive())
    {
        if (m_game.GetId() != GameId::Catacomb3Dv122)
        {
            DrawTiledWindow(renderer, 2, 1, 36, 13);
        }
        m_menu->Draw(renderer, m_game.GetEgaGraph(), m_game.GetMenuCursorPic(), m_gameTimer.GetActualTime());
    }

    const uint8_t showFpsMode = m_configurationSettings.GetCVarEnum(CVarIdShowFpsMode).GetItemIndex();
    if (showFpsMode != CVarItemIdShowFpsOff)
    {
        const int16_t offsetX = 6 - renderer.GetAdditionalMarginDueToWideScreen(aspectRatios[m_configurationSettings.GetCVarEnum(CVarIdAspectRatio).GetItemIndex()]);
        const std::string fpsStr =
            (showFpsMode == CVarItemIdShowFpsMinimal) ? std::to_string(m_framesCounter.GetFramesPerSecond()) :
            std::to_string(m_framesCounter.GetFramesPerSecond()) + " FPS";
        RenderableText renderableTextFont10(*DefaultFont::Get(renderer, 10));
        renderableTextFont10.LeftAligned(fpsStr, EgaBrightYellow, offsetX, 2);
        renderer.RenderText(renderableTextFont10);
        if (showFpsMode == CVarItemIdShowFpsExtended)
        {
            const std::string windowDimensionsStr = std::to_string(renderer.GetWindowWidth()) + " x " + std::to_string(renderer.GetWindowHeight());
            RenderableText renderableTextFont7(*DefaultFont::Get(renderer, 7));
            renderableTextFont7.LeftAligned(windowDimensionsStr, EgaBrightYellow, offsetX, 14);
            renderableTextFont7.LeftAligned(renderer.GetGraphicsApiVersion(), EgaBrightYellow, offsetX, 22);
            renderableTextFont7.LeftAligned(renderer.GetGraphicsAdapterVendor(), EgaBrightYellow, offsetX, 30);
            renderableTextFont7.LeftAligned(renderer.GetGraphicsAdapterModel(), EgaBrightYellow, offsetX, 38);
            renderer.RenderText(renderableTextFont7);
        }
    }

    const int16_t margin = renderer.GetAdditionalMarginDueToWideScreen(aspectRatios[m_configurationSettings.GetCVarEnum(CVarIdAspectRatio).GetItemIndex()]);
    m_renderableOverscanBorder.Draw(renderer, margin, m_gameTimer.GetActualTime(), m_state == Help);
    
    renderer.Unprepare2DRendering();
}

// Based on US_CenterWindow in ID_US.C of the Catacomb Abyss source code.
void EngineCore::DrawCenteredTiledWindow(IRenderer& renderer, const uint16_t width, const uint16_t height)
{
    DrawTiledWindow(renderer, (40 - width) / 2,(15 - height) / 2,width,height);
}

void EngineCore::DrawTiledWindow(IRenderer& renderer, const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height)
{
    renderer.Render2DBar(x * 8, y * 8, width * 8, height * 8, EgaLightGray);

    RenderableTiles tiles(*m_game.GetEgaGraph()->GetTilesSize8Masked());
    tiles.DrawWindow(x, y, width, height);
    renderer.RenderTiles(tiles);
}

void EngineCore::EnterKeyReleased()
{
    if (!m_menu->IsActive())
    {
        if (m_readingScroll != NotReadingAnyScroll)
        {
            m_readingScroll = NotReadingAnyScroll;
            if (!m_takingChest)
            {
                m_gameTimer.Resume();
            }
        }

        if (m_state == Introduction)
        {
            if (m_game.GetId() == GameId::Catacomb3Dv122)
            {
                OpenMenu();
            }
            else
            {
                m_state = RequestDifficultyLevel;
            }
        }
        else if (m_state == ShowDifficultyLevel)
        {
            m_state = StandBeforeGate;
        }
        else if (m_state == StandBeforeGate)
        {
            m_state = EnteringLevel;
            m_timeStampToEnterGame = m_gameTimer.GetActualTime() + 2000u;
            m_warpToLevel = 0;
        }
        else if (m_state == WarpCheatDialog)
        {
            m_state = InGame;
            // Warp to level
            const int levelIndexOnScreen = atoi(m_warpCheatTextField.c_str());
            // In Catacomb 3-D, the on-screen level indices start at 1 instead of 0.
            const int levelIndexInGameMaps = (m_game.GetId() == GameId::Catacomb3Dv122) ? levelIndexOnScreen - 1 : levelIndexOnScreen;
            if (levelIndexInGameMaps >= 0 && levelIndexInGameMaps < m_game.GetGameMaps()->GetNumberOfLevels())
            {
                m_warpToLevel = (uint8_t)levelIndexInGameMaps;
            }
            m_warpCheatTextField.clear();
            if (!m_takingChest)
            {
                m_gameTimer.Resume();
            }
        }
        else if (m_state == GodModeCheatDialog || m_state == FreeItemsCheatDialog || m_state == AutoMapDialog)
        {
            m_state = InGame;
            if (!m_takingChest)
            {
                m_gameTimer.Resume();
            }
        }
    }
}

bool EngineCore::Think()
{
    const uint32_t currentTimestampOfPlayer = m_gameTimer.GetMillisecondsForPlayer();
    const uint32_t currentTimestampOfWorld = m_gameTimer.GetMilliSecondsForWorld();

    m_timeStampOfPlayerPreviousFrame = m_timeStampOfPlayerCurrentFrame;
    m_timeStampOfPlayerCurrentFrame = currentTimestampOfPlayer;
    m_timeStampOfWorldPreviousFrame = m_timeStampOfWorldCurrentFrame;
    m_timeStampOfWorldCurrentFrame = currentTimestampOfWorld;
    if (m_timeStampOfPlayerPreviousFrame == 0)
    {
        return false;
    }

    const SDMode soundMode = SD_GetSoundMode();
    const uint8_t soundModeIndex = m_configurationSettings.GetCVarEnum(CVarIdSoundMode).GetItemIndex();
    if (soundModeIndex == CVarItemIdSoundModeOff && soundMode != sdm_Off)
    {
        SD_SetSoundMode(sdm_Off);
    }
    else if (soundModeIndex == CVarItemIdSoundModePCSpeaker && soundMode != sdm_PC)
    {
        SD_SetSoundMode(sdm_PC);
    }
    else if (soundModeIndex == CVarItemIdSoundModeAdlib && soundMode != sdm_AdLib)
    {
        SD_SetSoundMode(sdm_AdLib);
    }

    const SMMode musicMode = SD_GetMusicMode();
    const bool isMusicSetToAdlibInConfiguration = IsMusicSetToAdlibInConfiguration();
    if (isMusicSetToAdlibInConfiguration && musicMode == smm_Off)
    {
        SD_SetMusicMode(smm_AdLib);
    }
    else if (!isMusicSetToAdlibInConfiguration && musicMode == smm_AdLib)
    {
        SD_SetMusicMode(smm_Off);
    }

    if (m_menu->IsActive())
    {
        m_menu->SetSaveGameEnabled((m_state == InGame || m_state == WarpCheatDialog || m_state == GodModeCheatDialog || m_state == FreeItemsCheatDialog || m_state == AutoMapDialog) && !m_level->GetPlayerActor()->IsDead());
        const MenuCommand command = m_menu->ProcessInput(m_playerInput);
        if (command == MenuCommandStartNewGame)
        {
            StartNewGameWithDifficultySelection();
        }
        else if (command == MenuCommandStartNewGameEasy)
        {
            m_difficultyLevel = Easy;
            StartNewGame();
            CloseMenu();
        }
        else if (command == MenuCommandStartNewGameNormal)
        {
            m_difficultyLevel = Normal;
            StartNewGame();
            CloseMenu();
        }
        else if (command == MenuCommandStartNewGameHard)
        {
            m_difficultyLevel = Hard;
            StartNewGame();
            CloseMenu();
        }
        else if (command == MenuCommandExitGame)
        {
            m_state = ExitGame;
            if (m_game.GetId() != GameId::Catacomb3Dv122)
            {
                DisplayStatusMessage("FARE THEE WELL!", 2000);
            }
            m_gameTimer.Resume();
        }
        else if (command == MenuCommandEndGame)
        {
            UnloadLevel();
            m_state = Introduction;
            m_gameTimer.Reset();
        }
        else if (command == MenuCommandSaveGame)
        {
            const std::string& saveGameName = m_menu->GetNewSaveGameName();
            if (StoreGameToFile(saveGameName))
            {
                m_menu->ShowSavingPopup(saveGameName, m_gameTimer.GetActualTime());
                bool existingSaveOverwritten = false;
                uint16_t i = 0;
                while (i < m_savedGames.size() && !existingSaveOverwritten)
                {
                    existingSaveOverwritten = (m_savedGames.at(i).compare(saveGameName) == 0);
                    i++;
                }
                if (!existingSaveOverwritten)
                {
                    m_savedGames.push_back(saveGameName);
                    m_playerInput.ClearJustPressed();
                    m_menu->AddNewSavedGame(m_playerInput, saveGameName);
                    return false;
                }
            }
        }
        else if (command == MenuCommandLoadGame)
        {
            const std::string& saveGameName = m_menu->GetNewSaveGameName();
            LoadGameFromFile(saveGameName);
            StartMusicIfNeeded();
            m_playerInput.ClearJustPressed();
            return false;
        }
        else if (command == MenuCommandLoadDosGame)
        {
            const std::string& saveGameName = m_menu->GetNewSaveGameName();
            LoadDosGameFromFile(saveGameName);
            StartMusicIfNeeded();
            m_playerInput.ClearJustPressed();
            return false;
        }
        else if (command == MenuCommandCloseMenu)
        {
            CloseMenu();
            StartMusicIfNeeded();
            m_playerInput.ClearJustPressed();
            return false;
        }
    }

    if (m_state == InGame && !m_menu->IsActive())
    {
        if (m_score.Update(m_timeStampOfPlayerCurrentFrame))
        {
            m_game.PlaySoundGetPoints();
        }

        for (uint8_t i = (uint8_t)MoveForward; i < (uint8_t)MaxControlAction; i++)
        {
	        m_playerActions.SetActionActive((ControlAction)i, IsActionActive((ControlAction)i));
        }

        if (!m_configurationSettings.GetCVarBool(CVarIdMouseLook).IsEnabled())
        {
            // When mouse look is disabled, the Y-movement of the mouse will let the player walk forward and backward.
            if (!IsActionActive(MoveForward))
            {
                if (m_playerInput.GetMouseYPos() < 0)
                {
                    m_timeStampLastMouseMoveForward = m_timeStampOfPlayerCurrentFrame;
                    m_playerInput.SetMouseYPos(0);
                }
                if (m_timeStampLastMouseMoveForward + 100 > m_timeStampOfPlayerCurrentFrame)
                {
                    m_playerActions.SetActionActive(MoveForward, true);
                }
            }
            if (!IsActionActive(MoveBackward))
            {
                if (m_playerInput.GetMouseYPos() > 0)
                {
                    m_timeStampLastMouseMoveBackward = m_timeStampOfPlayerCurrentFrame;
                    m_playerInput.SetMouseYPos(0);
                }
                if (m_timeStampLastMouseMoveBackward + 100 > m_timeStampOfPlayerCurrentFrame)
                {
                    m_playerActions.SetActionActive(MoveBackward, true);
                }
            }
        }
    }

    if (m_state == WarpCheatDialog && !m_menu->IsActive())
    {
        for (int i = SDLK_0; i <= SDLK_9; i++)
        {
            if (m_playerInput.IsKeyJustPressed(i))
            {
                TextFieldInput((char)i);
            }
        }
        for (int i = SDLK_KP_1; i <= SDLK_KP_0; i++)
        {
            if (m_playerInput.IsKeyJustPressed(i))
            {
                const char numericChar = (char)(i == SDLK_KP_0 ? SDLK_0 : i - SDLK_KP_1 + SDLK_1);
                TextFieldInput(numericChar);
            }
        }

        if (m_playerInput.IsKeyJustPressed(SDLK_BACKSPACE))
        {
            BackspacePressed();
        }
    }

    if (m_playerInput.IsKeyPressed(SDLK_F10) && m_state == Introduction && m_game.GetId() == GameId::CatacombAbyssv113)
    {
        // Catacomb Abyss Shareware Demo
        m_menu->OpenDemo();
    }

    if (!m_playerInput.HasFocus() && m_state == InGame && !m_menu->IsActive())
    {
        OpenMenu();
        m_game.GetAudioPlayer()->StopMusic();
        m_playerInput.ClearAll();
    }

    if (m_state == AutoMapDialog && !m_menu->IsActive() && m_level != nullptr)
    {
        m_autoMap.ProcessInput(
            m_playerInput,
            (float)m_configurationSettings.GetCVarInt(CVarIdMouseSensitivity).GetValue(),
            *m_level,
            m_gameTimer.GetActualTime(),
            m_configurationSettings.GetCVarEnum(CVarIdAutoMapMode).GetItemIndex(),
            m_game.GetOriginal3DViewArea());
        m_playerInput.SetMouseXPos(0);
        m_playerInput.SetMouseYPos(0);
    }

    if (m_playerInput.IsKeyJustPressed(SDLK_RETURN))
    {
        EnterKeyReleased();
    }
    if (!m_menu->IsActive())
    {
        if (m_playerInput.IsKeyPressed(m_game.GetCheatsKeyCode()))
        {
            // Check for cheat codes
            if (m_playerInput.IsKeyPressed(SDLK_w)) // W = warp to level
            {
                ShowWarpCheatDialog();
            }
            if (m_playerInput.IsKeyPressed(SDLK_g)) // G = toggle God mode
            {
                ShowGodModeCheatDialog();
            }
            if (m_playerInput.IsKeyPressed(SDLK_i)) // I = free items
            {
                ShowFreeItemsCheatDialog();
            }
            if (m_playerInput.IsKeyPressed(SDLK_z)) // Z = freeze time
            {
                FreezeTimeCheat();
            }
            if (m_playerInput.IsKeyPressed(SDLK_o)) // O = overhead map
            {
                ShowAutoMap(true);
            }
            if (m_playerInput.IsKeyPressed(SDLK_e)) // E = Exit level (Catacomb 3D)
            {
                if (m_game.GetId() == GameId::Catacomb3Dv122 &&
                    m_state == InGame &&
                    m_level->GetLevelIndex() + 1 < m_game.GetGameMaps()->GetNumberOfLevels())
                {
                    m_warpToLevel = m_level->GetLevelIndex() + 1;
                }
            }
        }
        else
        {
            if (m_state == InGame)
            {
                for (uint8_t i = 0x31; i < 0x39; i++)
                {
                    if (m_playerInput.IsKeyJustPressed(i))
                    {
                        ReadScroll(i - 0x31);
                    }
                }
            }

            if (m_playerInput.IsKeyJustPressed(SDLK_n)) // N
            {
                KeyNPressed();
            }
            if (m_state == InGame && IsActionJustPressed(UsePotion))
            {
                PlayerUsesPotion();
            }
            if (IsActionJustPressed(ControlAction::ShowAutoMap))
            {
                if (m_state == InGame)
                {
                    ShowAutoMap(false);
                }
                else if (m_state == AutoMapDialog)
                {
                    m_state = InGame;
                    if (!m_takingChest)
                    {
                        m_gameTimer.Resume();
                    }
                }
            }

            if (m_playerInput.IsKeyJustPressed(SDLK_w)) // W
            {
                KeyWPressed();
            }
            if (m_playerInput.IsKeyJustPressed(SDLK_y)) // Y
            {
                KeyYPressed();
            }
        }
        if (m_playerInput.IsKeyPressed(SDLK_F2))
        {
            m_menu->OpenSoundMenu();
            m_gameTimer.Pause();
        }
        if (m_playerInput.IsKeyPressed(SDLK_F3))
        {
            m_menu->OpenSaveGameMenu();
            m_gameTimer.Pause();
        }
        if (m_playerInput.IsKeyPressed(SDLK_F4))
        {
            m_menu->OpenRestoreGameMenu();
            m_gameTimer.Pause();
        }
    }

    if (m_state != Help &&
        m_state != WarpCheatDialog &&
        m_state != GodModeCheatDialog &&
        m_state != FreeItemsCheatDialog &&
        !m_menu->IsActive() &&
        m_playerInput.IsKeyJustPressed(SDLK_ESCAPE)) // Escape
    {
        OpenMenu();
        m_game.GetAudioPlayer()->StopMusic();
        if (m_level != NULL && m_level->GetPlayerActor()->IsDead())
        {
            m_score.UpdateAll();
            m_menu->CheckHighScore(m_level->GetLevelIndex(), m_score.GetPoints());
        }
    }
    else if (m_state == Help)
    {
        if (m_game.ProcessInputOnHelpPage(m_playerInput))
        {
            m_state = InGame;
        }
    }
    else if (m_state == InGame && m_game.GetId() != GameId::Catacomb3Dv122 && m_playerInput.IsKeyJustPressed(SDLK_F1))
    {
        m_state = Help;

        // Close menu
        if (m_menu->IsActive())
        {
            m_menu->SetActive(false);
        }
    }
    else if (m_state == Victory && m_victoryState == VictoryStateDone && m_playerInput.IsAnyKeyPressed() && !m_menu->IsActive())
    {
        // Open the menu when any key is pressed in the victory screen.
        m_score.UpdateAll();
        m_menu->SetActive(true);
        m_menu->CheckHighScore(m_level->GetLevelIndex(), m_score.GetPoints());
    }
    else if ((m_state == WarpCheatDialog || m_state == GodModeCheatDialog || m_state == FreeItemsCheatDialog)
             && m_playerInput.IsKeyJustPressed(SDLK_ESCAPE))
    {
        // Close the dialog and erase the input
        m_state = InGame;
        m_warpCheatTextField.clear();
        if (!m_takingChest)
        {
            m_gameTimer.Resume();
        }
    }

    // Status message
    if (m_timeStampOfPlayerCurrentFrame > m_timeStampEndOfStatusMessage)
    {
        m_statusMessage = nullptr;
    }

    if (m_state == ExitGame && m_statusMessage == nullptr)
    {
        return true;
    }

    if (m_state == EnteringLevel)
    {
        if (m_timeStampToEnterGame < m_gameTimer.GetActualTime())
        {
            m_state = InGame;
            m_setOverlayOnNextDraw = true;
            StartMusicIfNeeded();
        }
    }

    if (m_state == InGame || m_state == EnteringLevel)
    {
        if (m_level == nullptr)
        {
            LoadLevel(m_warpToLevel);
        }
    }

    if (m_state == Victory)
    {
        switch (m_victoryState)
        {
        case VictoryStatePlayGetBolt:
            {
                m_game.PlaySoundGetBolt();
                m_victoryState++;
                break;
            }
        case VictoryStatePlayGetNuke:
            {
                m_game.PlaySoundGetNuke();
                m_victoryState++;
                break;
            }
        case VictoryStatePlayGetPotion:
            {
                m_game.PlaySoundGetPotion();
                m_victoryState++;
                break;
            }
        case VictoryStatePlayGetKey:
            {
                m_game.PlaySoundGetKey();
                m_victoryState++;
                break;
            }
        case VictoryStatePlayGetScroll:
            {
                m_game.PlaySoundGetScroll();
                m_victoryState++;
                break;
            }
        case VictoryStatePlayGetPoint:
            {
                m_game.PlaySoundGetPoints();
                m_victoryState++;
                break;
            }
        case VictoryStatePlayingGetBolt:
        case VictoryStatePlayingGetNuke:
        case VictoryStatePlayingGetPotion:
        case VictoryStatePlayingGetKey:
        case VictoryStatePlayingGetScroll:
        case VictoryStatePlayingGetPoint:
            {
                if (!m_game.GetAudioPlayer()->IsPlaying())
                {
                    m_victoryState++;
                }
                break;
            }
        }
    }

    if (m_state == InGame)
    {
        const bool manaBarEnabledInConfig = m_configurationSettings.GetCVarBool(CVarIdManaBar).IsEnabled();
        if (manaBarEnabledInConfig != m_manaBar.IsEnabled())
        {
            m_manaBar.Reset(manaBarEnabledInConfig);
        }

        if (!m_level->GetPlayerActor()->IsDead())
        {
            if (m_keyToTake != NoKey)
            {
                if (m_game.GetId() == GameId::Catacomb3Dv122)
                {
                    // In Catacomb 3D there is no popup. Use the key immediately.
                    m_playerInventory.TakeKey(m_keyToTake);
                    m_startTakeKey = 0;
                    m_keyToTake = NoKey;
                    m_game.PlaySoundUseKey();
                }
                else
                {
                    if (m_startTakeKey != 0)
                    {
                        if (m_startTakeKey + 2000 < m_gameTimer.GetActualTime())
                        {
                            m_messageInPopup[0] = 0;
                            m_playerInventory.TakeKey(m_keyToTake);
                            m_gameTimer.Resume();
                            m_startTakeKey = 0;
                            m_keyToTake = NoKey;
                        }
                    }
                    else
                    {
                        m_startTakeKey = m_gameTimer.GetActualTime();
                        m_gameTimer.Pause();
                        std::snprintf(m_messageInPopup, 256, "You use a %s key", GetKeyName((KeyId)m_keyToTake));
                        m_game.PlaySoundUseKey();
                    }
                }
            }

            if (m_messageInPopup[0] != 0)
            {
                if (m_startTakeKey == 0 && m_playerInput.IsAnyKeyPressed())
                {
                    m_messageInPopup[0] = 0;
                    m_gameTimer.Resume();
                }
            }

            if (m_timeStampOfPlayerCurrentFrame > m_timeStampOfPlayerPreviousFrame)
            {
                m_manaBar.Update(m_timeStampOfPlayerCurrentFrame);

                bool shoot = false;
                const bool autoFire = m_configurationSettings.GetCVarBool(CVarIdAutoFire).IsEnabled();
                if (m_game.GetId() != GameId::Catacomb3Dv122)
                {
                    shoot = m_playerActions.UpdateShoot(m_timeStampOfPlayerCurrentFrame, autoFire, m_manaBar);
                }
                else
                {
                    shoot = m_playerActions.UpdateShootWithCharge(m_timeStampOfPlayerCurrentFrame, autoFire, m_manaBar);
                }
                if (shoot)
                {
                    uint16_t projectileId = m_level->GetPlayerActor()->GetDecorateActor().projectileId;
                    if (m_playerActions.GetShotPower() == 56 && !m_playerActions.GetActionActive(Shoot))
                    {
                        // Big shot
                        projectileId++;
                        m_game.PlaySoundBoom();
                    }
                    else
                    {
                        m_game.PlaySoundShoot();
                    }
                    const auto decorateProjectilePair = m_game.GetDecorateActors().find(projectileId);
                    if (decorateProjectilePair != m_game.GetDecorateActors().end())
                    {
                        Actor* projectile = new Actor(m_level->GetPlayerActor()->GetX(), m_level->GetPlayerActor()->GetY(), m_timeStampOfPlayerCurrentFrame, decorateProjectilePair->second);
                        projectile->SetAngle(m_level->GetPlayerActor()->GetAngle());
                        projectile->SetActive(true);
                        m_level->AddNonBlockingActor(projectile);
                    }

                    
                }
                if (m_playerActions.IsReadyToShootBolt(m_timeStampOfPlayerCurrentFrame))
                {
                    if (m_playerInventory.TakeBolt())
                    {
                        m_playerActions.ShootBolt(m_timeStampOfPlayerCurrentFrame);
                        m_game.PlaySoundUseBolt();
                    }
                    else
                    {
                        m_game.PlaySoundNoItem();
                    }
                }

                if (m_playerActions.UpdateContinueBolt(m_timeStampOfPlayerCurrentFrame))
                {
                    const uint16_t projectileId = (m_game.GetId() == GameId::Catacomb3Dv122) ? m_level->GetPlayerActor()->GetDecorateActor().projectileId + 1 : m_level->GetPlayerActor()->GetDecorateActor().projectileId;
                    const auto decorateProjectilePair = m_game.GetDecorateActors().find(projectileId);
                    if (decorateProjectilePair != m_game.GetDecorateActors().end())
                    {
                        Actor* projectile = new Actor(m_level->GetPlayerActor()->GetX(), m_level->GetPlayerActor()->GetY(), m_timeStampOfWorldCurrentFrame, decorateProjectilePair->second);
                        projectile->SetAngle(m_level->GetPlayerActor()->GetAngle());
                        projectile->SetActive(true);
                        m_level->AddNonBlockingActor(projectile);
                    }
                }             
           
                if (m_playerActions.UpdateShootNuke(m_timeStampOfPlayerCurrentFrame))
                {
                    if (m_playerInventory.TakeNuke())
                    {
                        m_game.PlaySoundUseNuke();

                        const auto decorateProjectilePair = m_game.GetDecorateActors().find(m_level->GetPlayerActor()->GetDecorateActor().projectileId + 1);
                        if (decorateProjectilePair != m_game.GetDecorateActors().end())
                        {
                            for (uint16_t i = 0; i < 16; i++)
                            {
                                Actor* projectile = new Actor(m_level->GetPlayerActor()->GetX(), m_level->GetPlayerActor()->GetY(), m_timeStampOfWorldCurrentFrame, decorateProjectilePair->second);
                                projectile->SetAngle(i * 22.5f);
                                projectile->SetActive(true);
                                m_level->AddNonBlockingActor(projectile);
                            }
                        }
                    }
                    else
                    {
                        m_game.PlaySoundNoItem();
                    }
                }
                const float degreesPerTic = (m_playerActions.GetActionActive(QuickTurn)) ? 3.0f : 1.0f;
                const uint32_t deltaTimeInMs = m_timeStampOfPlayerCurrentFrame - m_timeStampOfPlayerPreviousFrame;
                const uint32_t truncatedDeltaTimeInMs = (deltaTimeInMs < 50) ? deltaTimeInMs : 50;
                const float deltaTimeInTics = (truncatedDeltaTimeInMs * 70.0f) / 1000.0f;
                const float turnSpeedFactor = m_configurationSettings.GetCVarInt(CVarIdTurnSpeed).GetValue() / 100.0f;
                if (m_playerActions.GetActionActive(TurnLeft) && !m_playerActions.GetActionActive(Strafe))
                {
                    const float deltaDegrees = degreesPerTic * deltaTimeInTics * turnSpeedFactor;
                    m_level->GetPlayerActor()->SetAngle(m_level->GetPlayerActor()->GetAngle() - deltaDegrees);
                }
                if (m_playerActions.GetActionActive(TurnRight) && !m_playerActions.GetActionActive(Strafe))
                {
                    const float deltaDegrees = degreesPerTic * deltaTimeInTics * turnSpeedFactor;
                    m_level->GetPlayerActor()->SetAngle(m_level->GetPlayerActor()->GetAngle() + deltaDegrees);
                }
                constexpr float playerSpeed = 5120.0f / 65536.0f;
                const float tics = ((float)(truncatedDeltaTimeInMs) / 1000.0f) * 70.0f;
                const bool isRunning = m_configurationSettings.GetCVarBool(CVarIdAlwaysRun).IsEnabled() != m_playerActions.GetActionActive(Run);
                const float distance = isRunning ? playerSpeed * tics * 1.5f : playerSpeed * tics;
                const bool strafeLeft = (m_playerActions.GetActionActive(StrafeLeft) ||
                    (m_playerActions.GetActionActive(Strafe) && m_playerActions.GetActionActive(TurnLeft)));
                const bool strafeRight = (m_playerActions.GetActionActive(StrafeRight) ||
                    (m_playerActions.GetActionActive(Strafe) && m_playerActions.GetActionActive(TurnRight)));
                if (m_playerActions.GetActionActive(MoveForward) && strafeLeft)
                { 
                    Thrust(315, distance);
                }
                else if (m_playerActions.GetActionActive(MoveBackward) && strafeLeft)
                {
                    Thrust(225, distance);
                }
                else if (m_playerActions.GetActionActive(MoveForward) && strafeRight)
                {
                    Thrust(45, distance);
                }
                else if (m_playerActions.GetActionActive(MoveBackward) && strafeRight)
                {
                    Thrust(135, distance);
                }
                else if (m_playerActions.GetActionActive(MoveForward))
                {
                    Thrust(0, distance);
                }
                else if (m_playerActions.GetActionActive(MoveBackward))
                {
                    Thrust(180, distance);
                }
                else if (strafeLeft)
                {
                    Thrust(270, distance);
                }
                else if (strafeRight)
                {
                    Thrust(90, distance);
                }
                if (m_playerInput.GetMouseXPos() != 0)
                {
                    const float mouseMovement = (float)m_playerInput.GetMouseXPos() * ((float)m_configurationSettings.GetCVarInt(CVarIdMouseSensitivity).GetValue() / 50.0f);
                    m_level->GetPlayerActor()->SetAngle(m_level->GetPlayerActor()->GetAngle() + mouseMovement);
                    m_playerInput.SetMouseXPos(0);
                }

                m_level->UpdateVisibilityMap();

                ThinkActors();
                ThinkNonBlockingActors();
            }

            if (m_takingChest && !m_menu->IsActive() && m_readingScroll == NotReadingAnyScroll)
            {
                if (!m_game.GetAudioPlayer()->IsPlaying())
                {
                    if (m_playerInventory.HasItemsInChest())
                    {
                        m_playerInventory.GiveNextItemInChest();
                    }
                    else
                    {
                        m_takingChest = false;
                        m_gameTimer.Resume();
                    }
                }
            }
        }

        // Check for player dead
        if (m_level->GetPlayerActor()->IsDead())
        {
            if (!m_gameTimer.IsPaused())
            {
                m_gameTimer.Pause();

                if (m_game.GetId() == GameId::Catacomb3Dv122)
                {
                    m_setOverlayOnNextDraw = true;
                }
            }
        }

        // Update radar
        
        m_radarModel.ResetRadar(m_level->GetPlayerActor(), m_playerInventory, m_timeStampOfPlayerCurrentFrame);
        const uint16_t mapSize = m_level->GetLevelWidth() * m_level->GetLevelHeight();
        m_radarModel.AddActors((const Actor**)m_level->GetBlockingActors(), mapSize);
        m_radarModel.AddActors((const Actor**)m_level->GetNonBlockingActors(), m_level->GetMaxNonBlockingActors());

        if (m_level->GetLevelIndex() != m_warpToLevel && m_state != VerifyGateExit && m_keyToTake == NoKey)
        {
            LoadLevel(m_warpToLevel);
            m_playerActions.ResetForNewLevel();
            m_state = EnteringLevel;
            m_timeStampToEnterGame = m_gameTimer.GetActualTime() + 2000u;
            if (m_game.GetId() == GameId::Catacomb3Dv122)
            {
                m_setOverlayOnNextDraw = true;
                m_timeStampToEnterGame += 1000;
                m_game.GetAudioPlayer()->StopMusic();
            }
        }
    }

    m_playerInput.ClearJustPressed();

    return false;
}

void EngineCore::ThinkActors()
{
    const uint16_t mapSize = m_level->GetLevelWidth() * m_level->GetLevelHeight();
    for ( uint16_t i = 0; i < mapSize; i++)
    {
        if (m_level->GetBlockingActors()[i] != nullptr)
        {
            if (!m_level->GetBlockingActors()[i]->IsActive() && m_level->IsTileVisibleForPlayer(i % m_level->GetLevelWidth(), i / m_level->GetLevelWidth()))
            {
                m_level->GetBlockingActors()[i]->SetActive(true);
            }
            if (m_level->GetBlockingActors()[i]->IsActive())
            {
                PerformActionOnActor(m_level->GetBlockingActors()[i]);

                // Another check on nullptr is necessary, as the actor could have been deleted by performing ActionRemove.
                if (m_level->GetBlockingActors()[i] != nullptr)
                {
                    m_level->GetBlockingActors()[i]->Think(m_timeStampOfWorldCurrentFrame);
                }
            }
        }
    }
}

void EngineCore::PerformActionOnActor(Actor* actor)
{
    const actorAction action = actor->GetAction();

    if (IsOneTimeAction(action) && actor->IsActionPerformed())
    {
        return;
    }

    switch (action)
    {
    case ActionChase:
    {
        Chase(actor, true, ChasePlayer);
        break;
    }
    case ActionChaseLikeMage:
    {
        if (actor->GetTimeToNextAction() == 0)
        {
            actor->SetTimeToNextAction(m_timeStampOfWorldCurrentFrame + ((uint32_t)10000));
        }
        if (m_timeStampOfWorldCurrentFrame >= actor->GetTimeToNextAction())
        {
            // Choose next target
            const bool shouldChasePlayer = (rand() % 2 == 0);
            if (shouldChasePlayer)
            {
                actor->SetTemp1(ChasePlayer);
            }
            else
            {
                int16_t otherTarget = (rand() % 4);
                actor->SetTemp1(otherTarget + 1);
            }
            actor->SetTimeToNextAction(m_timeStampOfWorldCurrentFrame + ((uint32_t)10000));
        }
        ChaseTarget target = (ChaseTarget)actor->GetTemp1();
        Chase(actor, true, target);
        break;
    }
    case ActionChaseLikeInvisDude:
    {
        if (actor->GetTimeToNextAction() == 0)
        {
            // Flash once every few seconds
            actor->SetTimeToNextAction(m_timeStampOfWorldCurrentFrame + (rand() % 10000));
        }
        if (m_timeStampOfWorldCurrentFrame >= actor->GetTimeToNextAction())
        {
            actor->SetState(StateIdPeek, m_timeStampOfWorldCurrentFrame);
            actor->SetTimeToNextAction(0);
        }
        else
        {
            Chase(actor, true, ChasePlayer);
            break;
        }
    }
    case ActionRunAway:
    {
        RunAway(actor);
        break;
    }
    case ActionBounce:
    {
        Bounce(actor);
        break;
    }
    case ActionHide:
    case ActionStatue:
    case ActionHangingSkeleton:
    {
        if (actor->GetTimeToNextAction() == 0)
        {
            actor->SetTimeToNextAction(m_timeStampOfWorldCurrentFrame + ((uint32_t)actor->GetTemp2() * 1000 / 70));
            actor->SetSolid(action == ActionStatue);
        }
        if (m_timeStampOfWorldCurrentFrame >= actor->GetTimeToNextAction())
        {
            if ((std::abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) > 1.1f + actor->GetDecorateActor().size) ||
                (std::abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) > 1.1f + actor->GetDecorateActor().size))
            {
                actor->SetState(StateIdRise, m_timeStampOfWorldCurrentFrame);
                actor->SetSolid(true);
                actor->SetTimeToNextAction(0);
            }
        }
        break;
    }
    case ActionChaseLikeWetMan:
    {
        if (actor->GetTimeToNextAction() == 0)
        {
            // Sink after between 4 and 7 seconds
            actor->SetTimeToNextAction(m_timeStampOfWorldCurrentFrame + 4000 + (rand() % 4) * 1000);
        }
        if (m_timeStampOfWorldCurrentFrame >= actor->GetTimeToNextAction() &&
            ((std::abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) > 1.1f + actor->GetDecorateActor().size) ||
            (std::abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) > 1.1f + actor->GetDecorateActor().size)))
        {
            actor->SetState(StateIdSink, m_timeStampOfWorldCurrentFrame);
            actor->SetTimeToNextAction(0);
        }
        else
        {
            Chase(actor, true, ChasePlayer);
        }
        break;
    }
    case ActionChaseLikeRunningEye:
    {
        ChaseLikeRunningEye(actor);
        break;
    }
    case ActionHideUnderWater:
    {
        if (actor->GetTimeToNextAction() == 0)
        {
            actor->SetSolid(false);
            // Rise after between 4 and 7 seconds
            actor->SetTimeToNextAction(m_timeStampOfWorldCurrentFrame + 4000 + (rand() % 4) * 1000);
        }
        if ((m_timeStampOfWorldCurrentFrame >= actor->GetTimeToNextAction()) ||
            (std::abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) < 2.0f + actor->GetDecorateActor().size) && (std::abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) < 2.0f + actor->GetDecorateActor().size))
        {
            if ((std::abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) > 1.1f + actor->GetDecorateActor().size) ||
                (std::abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) > 1.1f + actor->GetDecorateActor().size))
            {
                actor->SetState(StateIdRise, m_timeStampOfWorldCurrentFrame);
                actor->SetSolid(true);
                actor->SetActionPerformed(true);
                actor->SetTimeToNextAction(0);
            }
        }
        else if ((m_timeStampOfWorldCurrentFrame / 1000) % 10 == 0)
        {
            // Take a peek every 10 seconds
            const bool isPeekAltAvailable = (actor->GetDecorateActor().states.find(StateIdPeekAlternative) != actor->GetDecorateActor().states.end());
            const DecorateStateId newState = (isPeekAltAvailable && rand() % 2 == 0) ? StateIdPeekAlternative : StateIdPeek;
            actor->SetState(newState, m_timeStampOfWorldCurrentFrame);
            actor->SetActionPerformed(true);
        }
        else
        {
            // Not ready to rise yet, so keep chasing
            Chase(actor, true, ChasePlayer);
        }
        break;
    }
    case ActionAttack:
    {
        if (actor->GetDecorateActor().damage > 0)
        {
            // Melee attack
            if (actor->WouldCollideWithActor(m_level->GetPlayerActor()->GetX(), m_level->GetPlayerActor()->GetY(), 1.1f))
            {
                if (!m_godModeIsOn)
                {
                    const uint8_t damage = (m_difficultyLevel == Easy && m_game.GetId() != GameId::Catacomb3Dv122) ? actor->GetDecorateActor().damage / 2 : actor->GetDecorateActor().damage;
                    m_level->GetPlayerActor()->Damage(damage);
                    m_game.PlaySoundPlayerHurt(m_level->GetPlayerActor()->GetHealth());
                    const uint8_t borderSignal = (m_game.GetId() != GameId::Catacomb3Dv122) ? 12 : 5;
                    const uint32_t borderFlashTime = (uint32_t)(16 * (1000.0 / 70.0));
                    m_overscanBorder.SetColor(m_gameTimer.GetActualTime(), borderSignal, borderFlashTime);
                    DisplayStatusMessage("Damaging blows!", (uint16_t)(80 * (1000.0 / 70.0)));
                }
            }
        }
        else
        {
            // Ranged attack with projectiles
            const int16_t angleInt = m_level->AngleNearPlayer(actor);
            if (angleInt != -1)
            {
                Actor* projectile = nullptr;
                if (actor->GetDecorateActor().projectileId != 0)
                {
                    const auto iterator = m_game.GetDecorateActors().find(actor->GetDecorateActor().projectileId);
                    if (iterator != m_game.GetDecorateActors().end())
                    {
                        projectile = new Actor(actor->GetX(), actor->GetY(), m_timeStampOfWorldCurrentFrame, iterator->second);
                        if (actor->GetTemp2() > 0 && actor->GetDecorateActor().initialState != StateIdHidden)
                        {
                            projectile->SetTemp2(actor->GetTemp2()); // Shot power
                        }
                        projectile->SetAngle((float)angleInt);
                        m_level->AddNonBlockingActor(projectile);
                    }
                }
            }
        }

        actor->SetActionPerformed(true);
        break;
    }
    case ActionDropItem:
    {
        actor->SetActionPerformed(true);
        const int16_t itemId = actor->GetTemp1();
        if (itemId != 0)
        {
            const auto decorateItemPair = m_game.GetDecorateActors().find(itemId);
            if (decorateItemPair != m_game.GetDecorateActors().end())
            {
                Actor* actorItem = new Actor(actor->GetX(), actor->GetY(), m_timeStampOfWorldCurrentFrame, decorateItemPair->second);
                actorItem->SetActive(true);
                m_level->AddNonBlockingActor(actorItem);
            }
        }

        m_game.PlaySoundBossDeath();
        m_levelStatistics.UpdateItems(*m_level);
        
        break;
    }
    case ActionSpawnSkeleton:
    {
        int16_t xofs[] = {0,0,-1,+1};
        int16_t yofs[] = {-1,+1,0,0};
        bool spawnSkeleton = false;
        if (!actor->IsSolid())
        {
            actor->SetSolid(true);
        }

        if (m_timeStampOfWorldCurrentFrame >= actor->GetTimeToNextAction())
        {
            bool tileFound = false;
            int16_t loop = 0;
            while (loop < 4 && !tileFound)
            {
                const uint16_t tile = m_level->GetWallTile(actor->GetTileX() + xofs[loop], actor->GetTileY() + yofs[loop]);
                const std::vector<std::vector<uint16_t>>& wallSkeletonAnimations = m_game.GetWallSkeletonAnimations();
                uint8_t anim = 0;
                while ( anim < wallSkeletonAnimations.size() && !tileFound)
                {
                    const std::vector<uint16_t>& wallSkeletonAnimation = wallSkeletonAnimations.at(anim);
                    uint8_t frame = 0;
                    while (frame < wallSkeletonAnimation.size() - 1 && !tileFound)
                    {
                        if (wallSkeletonAnimation.at(frame) == tile)
                        {
                            tileFound = true;
                            m_level->SetWallTile(actor->GetTileX() + xofs[loop], actor->GetTileY() + yofs[loop], wallSkeletonAnimation.at(frame + 1));
                            if (frame == wallSkeletonAnimation.size() - 2)
                            {
                                spawnSkeleton = true;
                            }
                            else
                            {
                                actor->SetTimeToNextAction(m_timeStampOfWorldCurrentFrame + 2000);
                            }
                        }
                        frame++;
                    }
                    anim++;
                }
                loop++;
            }

            if (spawnSkeleton)
            {
                m_level->AddNonBlockingActor(actor);
                actor->SetActionPerformed(true);
                const uint16_t skeletonId = actor->GetDecorateActor().actionParameter;
                const auto decorateSkeletonPair = m_game.GetDecorateActors().find(skeletonId);
                if (decorateSkeletonPair != m_game.GetDecorateActors().end())
                {
                    Actor* skeletonActor = new Actor(actor->GetX(), actor->GetY(), m_timeStampOfWorldCurrentFrame, decorateSkeletonPair->second);
                    skeletonActor->SetTile(actor->GetTileX(), actor->GetTileY());
                    for (uint16_t i = 0; i < m_level->GetLevelWidth() * m_level->GetLevelHeight(); i++)
                    {
                        if (m_level->GetBlockingActors()[i] == actor)
                        {
                            m_level->GetBlockingActors()[i] = skeletonActor;
                        }
                    }
                }
            }
        }
        break;
    }
    case ActionRemove:
    {
        m_level->RemoveActor(actor);
        m_levelStatistics.UpdateItems(*m_level);
        break;
    }
    case ActionWaitForPickup:
    case ActionWaitForPickupDestructable:
    {
        actor->SetSolid(false);
        if ((std::abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) < m_level->GetPlayerActor()->GetDecorateActor().size + actor->GetDecorateActor().size) && (std::abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) < m_level->GetPlayerActor()->GetDecorateActor().size + actor->GetDecorateActor().size))
        {
            actor->SetState(StateIdPickup, m_timeStampOfWorldCurrentFrame);
        }
        break;
    }
    case ActionItemDestroyed:
    {
        const bool isBonusItem = (actor->GetDecorateActor().initialState == StateIdWaitForPickup);
        if (isBonusItem)
        {
            DisplayStatusMessage("Item destroyed", 80 * 14);
            const uint32_t borderFlashTime = (uint32_t)(16 * (1000.0 / 70.0));
            m_overscanBorder.SetColor(m_gameTimer.GetActualTime(), 14 | 56, borderFlashTime);
        }
        m_level->SpawnBigExplosion(actor->GetX(),actor->GetY(),12,(16l<<16L), m_timeStampOfWorldCurrentFrame, m_game.GetExplosionActor());

        actor->SetActionPerformed(true);
        m_levelStatistics.UpdateItems(*m_level);
        break;
    }
    case ActionFreezeTime:
    {
        FreezeTimeCheat();
        actor->SetSolid(false);
        actor->SetActionPerformed(true);
        break;
    }
    case ActionGiveChest:
    {
        m_playerInventory.GiveChest();
        m_takingChest = true;
        m_gameTimer.Pause();
        actor->SetActionPerformed(true);
        break;
    }
    case ActionGiveBolt:
    {
        m_playerInventory.GiveBolt();
        actor->SetActionPerformed(true);
        break;
    }
    case ActionGiveNuke:
    {
        m_playerInventory.GiveNuke();
        actor->SetActionPerformed(true);
        break;
    }
    case ActionGivePotion:
    {
        m_playerInventory.GivePotion();
        actor->SetActionPerformed(true);
        break;
    }
    case ActionGiveKey:
    {
        m_playerInventory.GiveKey((uint8_t)actor->GetDecorateActor().actionParameter);
        actor->SetActionPerformed(true);
        break;
    }
    case ActionGiveGem:
    {
        m_playerInventory.GiveGem((uint8_t)actor->GetDecorateActor().actionParameter);
        actor->SetActionPerformed(true);
        break;
    }
    case ActionGivePoints:
    {
        m_score.AddPoints((int32_t)actor->GetDecorateActor().actionParameter);
        actor->SetActionPerformed(true);
        break;
    }
    case ActionGivePointsForChest:
    {
        m_score.AddPoints((int32_t)(m_level->GetLevelIndex() + 1) * 100);
        m_game.PlaySoundGetPoints();
        actor->SetActionPerformed(true);
        break;
    }
    case ActionGiveScroll:
    {
        m_playerInventory.GiveScroll((uint8_t)actor->GetDecorateActor().actionParameter);
        ReadScroll((uint8_t)actor->GetDecorateActor().actionParameter);
        actor->SetActionPerformed(true);
        break;
    }
    case ActionExplodeWall1:
    {
        const uint8_t x = actor->GetTileX();
        const uint8_t y = actor->GetTileY();
        const DecorateActor& explodingWallActor = m_game.GetExplodingWallActor();
        if (x > 1 && m_level->IsExplosiveWall(x - 1, y))
        {
            m_level->ExplodeWall(x - 1, y, m_timeStampOfWorldCurrentFrame, explodingWallActor);
        }
        if (x < m_level->GetLevelWidth() - 2 && m_level->IsExplosiveWall(x + 1, y))
        {
            m_level->ExplodeWall(x + 1, y, m_timeStampOfWorldCurrentFrame, explodingWallActor);
        }
        if (y > 1 && m_level->IsExplosiveWall(x, y - 1))
        {
            m_level->ExplodeWall(x, y - 1, m_timeStampOfWorldCurrentFrame, explodingWallActor);
        }
        if (y < m_level->GetLevelHeight() - 2 && m_level->IsExplosiveWall(x, y + 1))
        {
            m_level->ExplodeWall(x, y + 1, m_timeStampOfWorldCurrentFrame, explodingWallActor);
        }

        const uint16_t tileExplosion = m_game.GetGameMaps()->GetTileWallExplosion(m_level->IsWaterLevel());
        m_level->SetWallTile(x, y, tileExplosion);
        uint16_t floorTile = m_level->GetFloorTile(x, y);
        floorTile &= 0xff;
        m_level->SetFloorTile(x, y, floorTile );

        actor->SetActionPerformed(true);
        break;
    }
    case ActionExplodeWall2:
    {
        const uint8_t x = actor->GetTileX();
        const uint8_t y = actor->GetTileY();

        const uint16_t tileExplosion = m_game.GetGameMaps()->GetTileWallExplosion(m_level->IsWaterLevel()) + 1;
        m_level->SetWallTile(x, y, tileExplosion);

        actor->SetActionPerformed(true);
        break;
    }
    case ActionExplodeWall3:
    {
        const uint8_t x = actor->GetTileX();
        const uint8_t y = actor->GetTileY();

        const uint16_t tileExplosion = m_game.GetGameMaps()->GetTileWallExplosion(m_level->IsWaterLevel()) + 2;
        m_level->SetWallTile(x, y, tileExplosion);

        actor->SetActionPerformed(true);
        break;
    }
    case ActionExplodeWall4:
    {
        const uint8_t x = actor->GetTileX();
        const uint8_t y = actor->GetTileY();

        m_level->SetWallTile(x, y, 0);
        m_level->SetFloorTile(x, y, 0);

        actor->SetActionPerformed(true);
        m_levelStatistics.UpdateSecrets(*m_level);
        break;
    }
    case ActionWarpToOtherLevel:
    {
        if (m_game.GetId() == GameId::Catacomb3Dv122)
        {
            // Catacomb 3D
            const uint16_t wallTile = m_level->GetWallTile(actor->GetTileX(), actor->GetTileY());
            const uint8_t nextLevel = (wallTile > 180) ? (uint8_t)(wallTile - 181) : m_level->GetLevelIndex() + 1;
            m_warpToLevel = nextLevel;
        }
        else
        {
            // Catacomb Adventure Series
            const uint8_t nextLevel = m_level->GetFloorTile(actor->GetTileX(), actor->GetTileY() + 1) >> 8;
            m_warpToLevel = nextLevel;
        }
        
        m_game.PlaySoundWarpUpOrDown(true);
        actor->SetActionPerformed(true);
        break;
    }
    case ActionWarpInsideLevel:
    {
        WarpInsideLevel(actor);
        m_game.PlaySoundWarp();
        actor->SetActionPerformed(true);
        break;
    }
    case ActionExplosionDelay:
    {
        if (actor->GetTimeToNextAction() == 0)
        {
            actor->SetTimeToNextAction(m_timeStampOfWorldCurrentFrame + ((uint32_t)actor->GetTemp2() * 1000 / 70));
        }
        if (m_timeStampOfWorldCurrentFrame >= actor->GetTimeToNextAction())
        {
            actor->SetState(StateIdDying, m_timeStampOfWorldCurrentFrame);
            actor->SetTimeToNextAction(0);
            m_game.PlaySoundBoom();
        }
        break;
    }
    case ActionForceField:
    {
        if (m_level->GetPlayerActor()->WouldCollideWithActor(actor->GetX(), actor->GetY(), actor->GetDecorateActor().size))
        {
            // Player touches forcefield
            if (!m_godModeIsOn)
            {
                const uint8_t baseDamage = (actor->GetTemp2() > 0) ? (uint8_t)actor->GetTemp2() : actor->GetDecorateActor().damage;
                const uint8_t damage = (m_difficultyLevel == Easy && m_game.GetId() != GameId::Catacomb3Dv122) ? baseDamage / 2 : baseDamage;
                m_level->GetPlayerActor()->Damage(damage);
                m_game.PlaySoundPlayerHurt(m_level->GetPlayerActor()->GetHealth());
                const uint8_t borderSignal = (m_game.GetId() != GameId::Catacomb3Dv122) ? 12 : 5;
                const uint32_t borderFlashTime = (uint32_t)(16 * (1000.0 / 70.0));
                m_overscanBorder.SetColor(m_gameTimer.GetActualTime(), borderSignal, borderFlashTime);
                DisplayStatusMessage("Damaging blows!", (uint16_t)(80 * (1000.0 / 70.0)));
            }
        }
        actor->SetActionPerformed(true);
        break;
    }
    case ActionPlayerProjectile:
    case ActionMonsterProjectile:
    {
        const float speed =  actor->GetDecorateActor().speed;
        if (speed > 0.0f)
        {
            const uint32_t deltaTime = m_timeStampOfWorldCurrentFrame - m_timeStampOfWorldPreviousFrame;
            const uint32_t truncatedDeltaTime = (deltaTime < 50) ? deltaTime : 50;
            float distance = speed * ((float)(truncatedDeltaTime) / 14.3f) / 65536.0f;
            if (distance > 1.0)
            {
                distance = 1.0;
            }
            const float angle = actor->GetAngle();
            const float xmove = distance * (float)sin(angle * 3.14159265 / 180.0);
            const float ymove = - distance * (float)cos(angle * 3.14159265 / 180.0);

            const float basex = actor->GetX() + xmove;
            const float basey = actor->GetY() + ymove;

            const float size = actor->GetDecorateActor().size;
            const float radius = actor->GetDecorateActor().size + 1.0f;
            const uint16_t xl = (radius > basex) ? 0u : (uint16_t)(basex - radius);
            const uint16_t xh = (uint16_t)(basex + radius) > m_level->GetLevelWidth() - 1 ? m_level->GetLevelWidth() - 1 : (uint16_t)(basex + radius);
            const uint16_t yl = (radius > basey) ? 0u : (uint16_t)(basey - radius);
            const uint16_t yh = (uint16_t)(basey + radius) > m_level->GetLevelHeight() - 1 ? m_level->GetLevelHeight() - 1 : (uint16_t)(basey + radius);

            bool moveOk = true;
            for (uint16_t y=yl;y<=yh;y++)
            {
                for (uint16_t x=xl;x<=xh;x++)
                {
                    if (m_level->IsSolidWall(x, y))
                    {
                        if ((std::abs(basex - (float)x - 0.5f) < size + 0.5f) &&
                            (std::abs(basey - (float)y - 0.5f) < size + 0.5f))
                        {
                            m_game.PlaySoundShootWall();
                            moveOk = false;
                        }
                    }

                    if (m_level->IsExplosiveWall(x, y))
                    {
                        if ((std::abs(basex - (float)x - 0.5f) < size + 0.5f) &&
                            (std::abs(basey - (float)y - 0.5f) < size + 0.5f))
                        {
                            if (action == ActionPlayerProjectile)
                            {
                                m_level->ExplodeWall(x, y, m_timeStampOfWorldCurrentFrame, m_game.GetExplodingWallActor());
                            }
                            moveOk = false;
                        }
                    }

                    Actor* otherActor = m_level->GetBlockingActor(x, y);
                    if (otherActor != nullptr)
                    {
                        if (action == ActionPlayerProjectile)
                        {
                            if (((otherActor->IsSolid() && otherActor->GetState() != StateIdRise && otherActor->GetState() != StateIdSink) || otherActor->GetAction() == ActionWaitForPickupDestructable || otherActor->GetAction() == ActionForceField || otherActor->GetAction() == ActionHangingSkeleton || otherActor->GetAction() == ActionBurningTree
                                ) && 
                                (std::abs(basex - otherActor->GetX()) < size + otherActor->GetDecorateActor().size) &&
                                (std::abs(basey - otherActor->GetY()) < size + otherActor->GetDecorateActor().size))
                            {
                                if (otherActor->GetAction() == ActionStatue || otherActor->GetAction() == ActionHangingSkeleton || otherActor->GetAction() == ActionBurningTree)
                                {
                                    m_game.PlaySoundShootWall();
                                }
                                else
                                {
                                    const bool isEasyModeFromAdventureSeries = (m_difficultyLevel == Easy && m_game.GetId() != GameId::Catacomb3Dv122);
                                    const uint8_t damage = actor->GetDecorateActor().damage;
                                    if (isEasyModeFromAdventureSeries)
                                    {
                                        otherActor->DamageInEasyMode(damage);
                                    }
                                    else
                                    {
                                        otherActor->Damage(damage);
                                    }
                                    
                                    m_game.GetAudioPlayer()->Play(otherActor->GetDecorateActor().hitSound);

                                    if (otherActor->IsDead())
                                    {
                                        otherActor->SetState(StateIdDying, m_timeStampOfWorldCurrentFrame);
                                        otherActor->SetSolid(false);

                                        auto deadStatePair = otherActor->GetDecorateActor().states.find(StateIdDead);
                                        const actorAction deadAction = (deadStatePair != otherActor->GetDecorateActor().states.end()) ? deadStatePair->second.animation.at(0).action : ActionNone;
                                        if (deadAction != ActionBurningTree)
                                        {
                                            m_level->AddNonBlockingActor(otherActor);
                                            m_level->SetBlockingActor(x, y, nullptr);
                                        }
                                        m_levelStatistics.UpdateMonstersKilled(*m_level);
                                    }
                                    else
                                    {
                                        if (m_game.GetId() == GameId::CatacombArmageddonv102 && otherActor->GetDecorateActor().id == 56 && rand() % 8 != 0)
                                        {
                                            // The red demon from Armageddon has a pain chance of 1:8, see C5_STATE.C.
                                        }
                                        else if (m_game.GetId() == GameId::CatacombApocalypsev101 && otherActor->GetDecorateActor().id == 64 && rand() % 5 != 0)
                                        {
                                            // The yellow troll from Apocalypse has a pain chance of 1:5, see C6_STATE.C.
                                        }
                                        else if (m_game.GetId() == GameId::CatacombApocalypsev101 && otherActor->GetDecorateActor().id == 57 && rand() % 8 != 0)
                                        {
                                            // The cyber demon from Apocalypse has a pain chance of 1:8, see C6_STATE.C.
                                        }
                                        else if (m_game.GetId() == GameId::CatacombApocalypsev101 && otherActor->GetDecorateActor().id == 62 && rand() % 8 != 0)
                                        {
                                            // The demon from Apocalypse has a pain chance of 1:8, see C6_STATE.C.
                                        }
                                        else
                                        {
                                            // Every other monster is always in pain when hit.
                                            otherActor->SetState(StateIdPain, m_timeStampOfWorldCurrentFrame);
                                        }
                                    }
                                }
                                moveOk = false;
                            }
                        }
                    }

                }
            }
            if (action == ActionMonsterProjectile)
            {
                if (m_level->GetPlayerActor()->WouldCollideWithActor(basex, basey, size))
                {
                    // Projectile from monster hits player
                    if (!m_godModeIsOn)
                    {
                        const uint8_t baseDamage = (actor->GetTemp2() > 0) ? (uint8_t)actor->GetTemp2() : actor->GetDecorateActor().damage;
                        const uint8_t damage = (m_difficultyLevel == Easy && m_game.GetId() != GameId::Catacomb3Dv122 && baseDamage > 1) ? baseDamage / 2 : baseDamage;
                        m_level->GetPlayerActor()->Damage(damage);
                        m_game.PlaySoundPlayerHurt(m_level->GetPlayerActor()->GetHealth());
                        const uint8_t borderSignal = (m_game.GetId() != GameId::Catacomb3Dv122) ? 12 : 5;
                        const uint32_t borderFlashTime = (uint32_t)(16 * (1000.0 / 70.0));
                        m_overscanBorder.SetColor(m_gameTimer.GetActualTime(), borderSignal, borderFlashTime);
                        DisplayStatusMessage("Damaging blows!", (uint16_t)(80 * (1000.0 / 70.0)));
                    }
                    moveOk = false;
                }
            }

            if (moveOk)
            {
                actor->SetX(basex);
                actor->SetY(basey);
            }
            else
            {
                actor->SetState(StateIdDying, m_timeStampOfWorldCurrentFrame);
            }
        }
        break;
    }
    case ActionExplodeSound:
    {
        m_game.PlaySoundBodyExplode();
        actor->SetActionPerformed(true);
        break;
    }
    case ActionSmallMonsterSound:
    {
        m_game.PlaySoundSmallMonster();
        actor->SetActionPerformed(true);
        break;
    }
    case ActionLargeMonsterSound:
    {
        m_game.PlaySoundLargeMonster();
        actor->SetActionPerformed(true);
        break;
    }
    case ActionPortalSound:
    {
        m_game.PlaySoundPortal();
        actor->SetActionPerformed(true);
        break;
    }
    case ActionBurningTree:
    {
        int16_t fireFrame = actor->GetTemp1();
        const uint8_t nextFireFrame = rand() % 2;

        if (fireFrame == 3)
        {
            actor->SetAnimationFrame(nextFireFrame == 0 ? 1 : 2);
            fireFrame = 4;
        }
        else if (fireFrame == 4)
        {
            actor->SetAnimationFrame(nextFireFrame == 0 ? 0 : 2);
            fireFrame = 5;
        }
        else
        {
            actor->SetAnimationFrame(nextFireFrame == 0 ? 0 : 1);
            fireFrame = 3;
        }
        actor->SetTemp1(fireFrame);

        if (m_level->GetPlayerActor()->WouldCollideWithActor(actor->GetX(), actor->GetY(), 1.1f))
        {
            if (!m_godModeIsOn)
            {
                const int16_t damage = 1;
                m_level->GetPlayerActor()->Damage(damage);
                m_game.PlaySoundPlayerHurt(m_level->GetPlayerActor()->GetHealth());
                const uint8_t borderSignal = (m_game.GetId() != GameId::Catacomb3Dv122) ? 12 : 5;
                const uint32_t borderFlashTime = (uint32_t)(16 * (1000.0 / 70.0));
                m_overscanBorder.SetColor(m_gameTimer.GetActualTime(), borderSignal, borderFlashTime);
            }
        }
        actor->SetActionPerformed(true);
        break;
    }
    case ActionFlash:
    {
        if (actor->GetTemp2() == 0)
        {
            int16_t flashFrame = actor->GetTemp1();
            actor->SetAnimationFrame(flashFrame);
            actor->SetTemp1(flashFrame + 1 % 3);
            actor->SetTemp2(1);
        }
        else
        {
            actor->SetState(StateIdWalk, m_timeStampOfWorldCurrentFrame);
            actor->SetTemp2(0);
        }
        actor->SetActionPerformed(true);
        break;
    }
    case ActionHarmlessBunny:
    {
        if (actor->GetTimeToNextAction() == 0)
        {
            actor->SetTimeToNextAction(m_timeStampOfWorldCurrentFrame + ((uint32_t)actor->GetTemp2() * 1000 / 70));
            actor->SetSolid(false);
        }
        if (m_timeStampOfWorldCurrentFrame >= actor->GetTimeToNextAction())
        {
            // Time to transform from harmless bunny to evil bunny!
            // Check that the player is not on top of the bunny.
            if ((std::abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) > 1.1f + actor->GetDecorateActor().size) ||
                (std::abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) > 1.1f + actor->GetDecorateActor().size))
            {
                // Put the bunny at the center of the tile, such that the chase routine can start from there.
                actor->SetX((float)(actor->GetTileX()) + 0.5f);
                actor->SetY((float)(actor->GetTileY()) + 0.5f);

                // Enter the transformation state.
                actor->SetState(StateIdRise, m_timeStampOfWorldCurrentFrame);
                actor->SetSolid(true);
                actor->SetTimeToNextAction(0);
            }
        }
        else
        {
            // Hop from left to right as a harmless bunny.
            BunnyHopping(actor);
        }
        break;
    }
    case ActionFakeWall:
    {
        const bool playerTouchesFakeWall =
            ((std::abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) < 0.9f + actor->GetDecorateActor().size) &&
            (std::abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) < 0.9f + actor->GetDecorateActor().size));
        m_level->SetWallTile(actor->GetTileX(), actor->GetTileY(), playerTouchesFakeWall ? 0 : actor->GetTemp1());
        break;
    }
    case ActionVictory:
    {
        m_state = Victory;
        m_victoryState = VictoryStatePlayGetPoint;
        break;
    }
    case ActionNone:
    {
        break;
    }
    default:
    {
        Logging::Instance().FatalError("Unknown action (" + std::to_string(action) + ") for actor with id (" + std::to_string(actor->GetDecorateActor().id) + ") and state (" + std::to_string(actor->GetState()) + ")");
        break;
    }
    }
}

void EngineCore::ThinkNonBlockingActors()
{
    for ( uint16_t i = 0; i < m_level->GetMaxNonBlockingActors(); i++)
    {
        if (m_level->GetNonBlockingActors()[i] != nullptr)
        {
            PerformActionOnActor(m_level->GetNonBlockingActors()[i]);

            // Another check on nullptr is necessary, as the actor could have been deleted by performing ActionRemove.
            if (m_level->GetNonBlockingActors()[i] != nullptr)
            {
                m_level->GetNonBlockingActors()[i]->Think(m_timeStampOfWorldCurrentFrame);
            }
        }
    }
}

const char* EngineCore::GetKeyName(const KeyId keyId) const
{
    const char* KeyNames[4] = {"RED", "YELLOW", "GREEN", "BLUE"};
    return (keyId < 4) ? KeyNames[keyId] : "";
}

// Based on ClipXMove() in C4_WIZ.C of the Catacomb Abyss source code.
bool EngineCore::ClipXMove (const float xmove)
{
    //
    // move player and check to see if any corners are in solid tiles
    //
    const float basex = m_level->GetPlayerActor()->GetX() + xmove;
    const float basey = m_level->GetPlayerActor()->GetY();

    const float radius = m_level->GetPlayerActor()->GetDecorateActor().size + 1.0f;
    const uint16_t xl = (radius > basex) ? 0u : (uint16_t)(basex - radius);
    const uint16_t xh = (uint16_t)(basex + radius) > m_level->GetLevelWidth() - 1 ? m_level->GetLevelWidth() - 1 : (uint16_t)(basex + radius);
    const uint16_t yl = (radius > basey) ? 0u : (uint16_t)(basey - radius);
    const uint16_t yh = (uint16_t)(basey + radius) > m_level->GetLevelHeight() - 1 ? m_level->GetLevelHeight() - 1 : (uint16_t)(basey + radius);

    bool moveOk = true;
    for (uint16_t y=yl;y<=yh;y++)
        for (uint16_t x=xl;x<=xh;x++)
        {
            moveOk &= ClipWithTile(x, y, basex, basey);
        }
    if (moveOk)
    {
        m_level->GetPlayerActor()->SetX(basex);
    }

    return moveOk;
}

// Based on ClipYMove() in C4_WIZ.C of the Catacomb Abyss source code.
bool EngineCore::ClipYMove (const float ymove)
{
    const float basex = m_level->GetPlayerActor()->GetX();
    const float basey = m_level->GetPlayerActor()->GetY() + ymove;

    const float radius = m_level->GetPlayerActor()->GetDecorateActor().size + 1.0f;
    const uint16_t xl = (radius > basex) ? 0u : (uint16_t)(basex - radius);
    const uint16_t xh = (uint16_t)(basex + radius) > m_level->GetLevelWidth() - 1 ? m_level->GetLevelWidth() - 1 : (uint16_t)(basex + radius);
    const uint16_t yl = (radius > basey) ? 0u : (uint16_t)(basey - radius);
    const uint16_t yh = (uint16_t)(basey + radius) > m_level->GetLevelHeight() - 1 ? m_level->GetLevelHeight() - 1 : (uint16_t)(basey + radius);

    bool moveOk = true;
    for (uint16_t y=yl;y<=yh;y++)
        for (uint16_t x=xl;x<=xh;x++)
        {
            moveOk &= ClipWithTile(x, y, basex, basey);
        }

    if (moveOk)
    {
        m_level->GetPlayerActor()->SetY(basey);
    }

    return moveOk;
}

bool EngineCore::ClipWithTile(const uint16_t tileX, const uint16_t tileY, const float playerX, const float playerY)
{
    bool moveOk = true;
    const float playerSize = m_level->GetPlayerActor()->GetDecorateActor().size;
    const bool playerTouchesTile = ((std::abs(playerX - (float)tileX - 0.5f) < playerSize + 0.5f) &&
        (std::abs(playerY - (float)tileY - 0.5f) < playerSize + 0.5f));
    if (playerTouchesTile && m_level->IsSolidWall(tileX, tileY))
    {
        moveOk = false;

        // Play sound while sliding along a wall in Catacomb 3D
        if (m_game.GetId() == GameId::Catacomb3Dv122 && m_state == InGame && !m_game.GetAudioPlayer()->IsPlaying())
        {
            m_game.GetAudioPlayer()->Play(0);
        }
    }

    if (playerTouchesTile && m_level->IsDoor(tileX, tileY))
    {
        moveOk = false;

        const KeyId requiredKey = m_level->GetRequiredKeyForDoor(tileX, tileY);
        if (requiredKey != NoKey && m_playerInventory.GetKeys(requiredKey) == 0)
        {
            if (m_game.GetId() != GameId::Catacomb3Dv122)
            {
                m_gameTimer.Pause();
                std::snprintf(m_messageInPopup, 256, "You need a %s key", GetKeyName(requiredKey));
                WaitForAnyKeyPressed();
                m_game.PlaySoundHitGate();
            }
        }
        else
        {
            if (requiredKey != NoKey)
            {
                m_keyToTake = requiredKey;
            }
            if (m_level->IsBlockedDoor(tileX, tileY))
            {
                if (m_game.GetId() != GameId::Catacomb3Dv122)
                {
                    m_gameTimer.Pause();
                    std::snprintf(m_messageInPopup, 256, "The door is blocked");
                    WaitForAnyKeyPressed();
                    m_game.PlaySoundHitGate();
                }
            }
            else if (m_level->IsRemovableDoor(tileX, tileY) ||
                     m_game.GetId() == GameId::Catacomb3Dv122) // All doors in the Catacomb 3-D can be removed
            {
                // Open the door
                m_level->SetWallTile(tileX, tileY, 0);
                m_level->SetFloorTile(tileX, tileY, 0);

                // Open connected door tiles which require the same key in all four directions
                if (requiredKey != NoKey)
                {
                    uint16_t x = tileX - 1;
                    while (x > 0 && m_level->GetRequiredKeyForDoor(x, tileY) == requiredKey)
                    {
                        m_level->SetWallTile(x, tileY, 0);
                        m_level->SetFloorTile(x, tileY, 0);
                        x--;
                    }
                    x = tileX + 1;
                    while (x < m_level->GetLevelWidth() - 1 && m_level->GetRequiredKeyForDoor(x, tileY) == requiredKey)
                    {
                        m_level->SetWallTile(x, tileY, 0);
                        m_level->SetFloorTile(x, tileY, 0);
                        x++;
                    }
                    uint16_t y = tileY - 1;
                    while (y > 0 && m_level->GetRequiredKeyForDoor(tileX, y) == requiredKey)
                    {
                        m_level->SetWallTile(tileX, y, 0);
                        m_level->SetFloorTile(tileX, y, 0);
                        y--;
                    }
                    y = tileY + 1;
                    while (y < m_level->GetLevelHeight() - 1 && m_level->GetRequiredKeyForDoor(tileX, y) == requiredKey)
                    {
                        m_level->SetWallTile(tileX, y, 0);
                        m_level->SetFloorTile(tileX, y, 0);
                        y++;
                    }
                }
            }
            else if (m_level->IsExitDoor(tileX, tileY))
            {
                // Exit the map
                m_warpToLevel = m_level->GetLevelIndex() + 1;
                m_gameTimer.Pause();
                m_game.PlaySoundHitGate();
                m_state = VerifyGateExit;
            }
            else
            {
                const uint16_t spot = (m_level->GetFloorTile(tileX,tileY) >> 8);
                m_warpToLevel = (uint8_t)spot;
                m_gameTimer.Pause();
                m_game.PlaySoundHitGate();
                m_state = VerifyGateExit;
            }
        }
    }

    if (playerTouchesTile && m_level->IsVictoryDoor(tileX, tileY))
    {
        moveOk = false;
        m_state = Victory;
    }

    Actor* actor = m_level->GetBlockingActor(tileX, tileY);
    if (actor != nullptr)
    {
        if (actor->WouldCollideWithActor(playerX, playerY, m_level->GetPlayerActor()->GetDecorateActor().size))
        {
            moveOk = false;
        }
    }

    return moveOk;
}

void EngineCore::SelectDifficultyLevelNovice()
{
    m_difficultyLevel = Easy;
    m_state = ShowDifficultyLevel;
}

void EngineCore::SelectDifficultyLevelWarrior()
{
    if (m_state == RequestDifficultyLevel)
    {
        m_difficultyLevel = Hard;
        m_state = ShowDifficultyLevel;
    }
}

void EngineCore::Thrust(const uint16_t angle, const float distance)
{
    if (m_state != InGame || m_readingScroll != NotReadingAnyScroll)
    {
        return;
    }

    float xmove,ymove;

    if (!m_game.GetAudioPlayer()->IsPlaying())
    {
        const uint32_t ticks = m_gameTimer.GetTicksForPlayer();

        if (ticks>>5 != ((ticks - 2)>>5))
        {
            m_game.PlaySoundWalk((ticks & 32) == 0);
        }
    }
    xmove = -distance * (float)std::sin((m_level->GetPlayerActor()->GetAngle() + 180 + angle) * 3.14159265 / 180.0);
    ymove = distance * (float)std::cos((m_level->GetPlayerActor()->GetAngle() + 180 + angle) * 3.14159265 / 180.0);

    const float playerPosXBeforeMove = m_level->GetPlayerActor()->GetX();
    const float playerPosYBeforeMove = m_level->GetPlayerActor()->GetY();

    bool moveOk = ClipXMove(xmove);
    if (m_messageInPopup[0] == 0)
    {
        moveOk &= ClipYMove(ymove);
    }

    if (!moveOk && m_game.GetId() == GameId::Catacomb3Dv122 && m_configurationSettings.GetCVarBool(CVarIdStickyWalls).IsEnabled())
    {
        // Simulate the sticky walls from the Catacomb 3-D by setting the player back to the position before the move
        m_level->GetPlayerActor()->SetX(playerPosXBeforeMove);
        m_level->GetPlayerActor()->SetY(playerPosYBeforeMove);
    }
}

void EngineCore::ReadScroll(const uint8_t scrollIndex)
{
    if (m_playerInventory.GetScroll(scrollIndex) && m_scrollsArePresent)
    {
        m_gameTimer.Pause();
        m_readingScroll = scrollIndex;
    }
}

void EngineCore::KeyWPressed()
{
    if (m_state == RequestDifficultyLevel)
    {
        SelectDifficultyLevelWarrior();
    }
}

// Based on Chase() in C4_STATE.C of the Catacomb Abyss source code.
bool EngineCore::Chase(Actor* actor, const bool diagonal, const ChaseTarget target)
{
    uint16_t speed = actor->GetDecorateActor().speed;

    // The water troll in Abyss and the water dragon in Armageddon move slower when under water.
    // The blob from Apocalypse moves slower when in shadow mode.
    if (actor->GetState() == StateIdHidden &&
        (((m_game.GetId() == GameId::CatacombAbyssv113 || m_game.GetId() == GameId::CatacombAbyssv124 || m_game.GetId() == GameId::CatacombArmageddonv102) && actor->GetDecorateActor().id == 61)) ||
        ((m_game.GetId() == GameId::CatacombApocalypsev101 && actor->GetDecorateActor().id == 54)))
    {
        speed = 1200;
    }

    // The manta ray in Apocalypse move slower when under water.
    if (actor->GetState() == StateIdHidden &&
        ((m_game.GetId() == GameId::CatacombApocalypsev101 && actor->GetDecorateActor().id == 65)))
    {
        speed = 1700;
    }

    const uint32_t deltaTimeInMs = m_timeStampOfWorldCurrentFrame - m_timeStampOfWorldPreviousFrame;
    const uint32_t truncatedDeltaTimeInMs = (deltaTimeInMs < 50) ? deltaTimeInMs : 50;
    float move = (float)speed * ((float)(truncatedDeltaTimeInMs) / 14.3f) / 65536.0f;

    while (move > 0.0f)
    {
        if (actor->GetState() == StateIdWalk || actor->GetState() == StateIdPain)
        {
            // Check if actor can attack
            if (actor->GetDecorateActor().damage > 0)
            {
                // Melee attack
                const bool performRandomAttack = ((m_game.GetId() != GameId::Catacomb3Dv122) && (rand() % (5000 / truncatedDeltaTimeInMs) == 0));
                const bool playerInRange = actor->WouldCollideWithActor(m_level->GetPlayerActor()->GetX(), m_level->GetPlayerActor()->GetY(), 1.0f);
                if (playerInRange || performRandomAttack)
                {
                    actor->SetState(StateIdAttack, m_timeStampOfWorldCurrentFrame);
                    return true;
                }
            }
            else
            {
                if (actor->GetDecorateActor().projectileId != 0)
                {
                    // Projectile attack
                    if ((rand() % (1000 / truncatedDeltaTimeInMs)) == 0 && m_level->AngleNearPlayer(actor) != -1)
                    {
                        actor->SetState(StateIdAttack, m_timeStampOfWorldCurrentFrame);
                    }
                }
            }
        }

        // Clip with player
        if (actor->WouldCollideWithActor(m_level->GetPlayerActor()->GetX(), m_level->GetPlayerActor()->GetY(), 1.0f))
        {
            return true;
        }

        if (move < actor->GetDistanceToTarget())
        {
            m_level->MoveActor(actor, move);
            break;
        }
        
        m_level->GetBlockingActors()[(actor->GetTileY() * m_level->GetLevelWidth()) + actor->GetTileX()] = nullptr;	// pick up marker from goal
        if (actor->GetDirection() == nodir)
            actor->SetDirection(north);

        // Instantly set the actor on its target
        move -= actor->GetDistanceToTarget();
        actor->SetX((float)(actor->GetTileX()) + 0.5f);
        actor->SetY((float)(actor->GetTileY()) + 0.5f);

        m_level->ChaseThink (actor,diagonal, target);

        m_level->GetBlockingActors()[(actor->GetTileY() * m_level->GetLevelWidth()) + actor->GetTileX()] = actor;	// set down a new goal marker
        if (actor->TargetReached())
            break;			// no possible move

    }

    return false;
}

void EngineCore::RunAway(Actor* actor)
{
    const uint16_t speed = actor->GetDecorateActor().speed;

    const uint32_t deltaTimeInMs = m_timeStampOfWorldCurrentFrame - m_timeStampOfWorldPreviousFrame;
    const uint32_t truncatedDeltaTimeInMs = (deltaTimeInMs < 50) ? deltaTimeInMs : 50;
    float move = (float)speed * ((float)(truncatedDeltaTimeInMs) / 14.3f) / 65536.0f;

    while (move > 0.0f)
    {
        // Clip with player
        if (actor->WouldCollideWithActor(m_level->GetPlayerActor()->GetX(), m_level->GetPlayerActor()->GetY(), 1.0f))
        {
            break;
        }

        if (move < actor->GetDistanceToTarget())
        {
            m_level->MoveActor(actor, move);
            break;
        }

        m_level->GetBlockingActors()[(actor->GetTileY() * m_level->GetLevelWidth()) + actor->GetTileX()] = nullptr;	// pick up marker from goal
        if (actor->GetDirection() == nodir)
            actor->SetDirection(north);

        // Instantly set the actor on its target
        move -= actor->GetDistanceToTarget();
        actor->SetX((float)(actor->GetTileX()) + 0.5f);
        actor->SetY((float)(actor->GetTileY()) + 0.5f);

        m_level->RunAwayThink(actor);

        m_level->GetBlockingActors()[(actor->GetTileY() * m_level->GetLevelWidth()) + actor->GetTileX()] = actor;	// set down a new goal marker
        if (actor->TargetReached())
        {
            break;			// no possible move
        }
    }
}

void EngineCore::Bounce(Actor* actor)
{
    const uint16_t speed = actor->GetDecorateActor().speed;

    const uint32_t deltaTimeInMs = m_timeStampOfWorldCurrentFrame - m_timeStampOfWorldPreviousFrame;
    const uint32_t truncatedDeltaTimeInMs = (deltaTimeInMs < 50) ? deltaTimeInMs : 50;
    float move = (float)speed * ((float)(truncatedDeltaTimeInMs) / 14.3f) / 65536.0f;

    while (move > 0.0f)
    {
        // Check if player can be attacked
        if (actor->WouldCollideWithActor(m_level->GetPlayerActor()->GetX(), m_level->GetPlayerActor()->GetY(), 1.0f) && actor->GetTemp2() == 0)
        {
            actor->SetTemp2(1);
            actor->SetState(StateIdAttack, m_timeStampOfWorldCurrentFrame);
        }

        if (move < actor->GetDistanceToTarget())
        {
            m_level->MoveActor(actor, move);
            break;
        }

        m_level->GetBlockingActors()[(actor->GetTileY() * m_level->GetLevelWidth()) + actor->GetTileX()] = nullptr;	// pick up marker from goal

        // Instantly set the actor on its target
        move -= actor->GetDistanceToTarget();
        actor->SetX((float)(actor->GetTileX()) + 0.5f);
        actor->SetY((float)(actor->GetTileY()) + 0.5f);

        if (!m_level->Walk(actor))
        {
            // Bouncing fireball is blocked, reverse direction
            actorDirection reverseDirection =
                (actor->GetDirection() == north) ? south :
                (actor->GetDirection() == south) ? north :
                (actor->GetDirection() == west) ? east :
                west;
            actor->SetDirection(reverseDirection);

            // Ready to damage player again
            actor->SetTemp2(0);
        }

        m_level->GetBlockingActors()[(actor->GetTileY() * m_level->GetLevelWidth()) + actor->GetTileX()] = actor;	// set down a new goal marker
        if (actor->TargetReached())
        {
            break;			// no possible move
        }
    }
}

void EngineCore::ChaseLikeRunningEye(Actor* actor)
{
    const uint16_t speed = actor->GetDecorateActor().speed;

    const uint32_t deltaTimeInMs = m_timeStampOfWorldCurrentFrame - m_timeStampOfWorldPreviousFrame;
    const uint32_t truncatedDeltaTimeInMs = (deltaTimeInMs < 50) ? deltaTimeInMs : 50;
    float move = (float)speed * ((float)(truncatedDeltaTimeInMs) / 14.3f) / 65536.0f;

    while (move > 0.0f)
    {
        // Clip with player
        if (actor->WouldCollideWithActor(m_level->GetPlayerActor()->GetX(), m_level->GetPlayerActor()->GetY(), 1.0f))
        {
            break;
        }

        if (move < actor->GetDistanceToTarget())
        {
            m_level->MoveActor(actor, move);
            break;
        }

        m_level->GetBlockingActors()[(actor->GetTileY() * m_level->GetLevelWidth()) + actor->GetTileX()] = nullptr;	// pick up marker from goal
        if (actor->GetDirection() == nodir)
            actor->SetDirection(north);

        // Instantly set the actor on its target
        move -= actor->GetDistanceToTarget();
        actor->SetX((float)(actor->GetTileX()) + 0.5f);
        actor->SetY((float)(actor->GetTileY()) + 0.5f);

        
        int16_t newTemp2 = m_level->GetFloorTile(actor->GetTileX(), actor->GetTileY()) >> 8;
        if (newTemp2 != 0)
        {
            actor->SetTemp2(newTemp2);
        }

        const int16_t temp2 = actor->GetTemp2();
        const actorDirection dir =
            (temp2 == 1) ? north :
            (temp2 == 2) ? east :
            (temp2 == 3) ? south : west;
        actor->SetDirection(dir);

        m_level->Walk(actor);

        m_level->GetBlockingActors()[(actor->GetTileY() * m_level->GetLevelWidth()) + actor->GetTileX()] = actor;	// set down a new goal marker
        if (actor->TargetReached())
        {
            break;			// no possible move
        }
    }
}

void EngineCore::BunnyHopping(Actor* actor)
{
    // This function is based on T_HarmlessBunnyWalk of C5_ACT2.C, with the simplification that
    // the bunny cannot leave the tile he spawned upon while hopping from left to right.

    actorDirection valid_dir[8][2] =
    { 
    { west,east }, // north
    { north,south }, // east
    { east,west }, // south
    { south,north }, // west
    { northwest,southeast }, // northeast
    { northeast,southwest }, // southeast
    { southeast,northwest }, // southwest
    { southwest,northeast } // northwest
    };

    actorDirection player_dir;
    const float playerAngle = m_level->GetPlayerActor()->GetAngle();
    if ((playerAngle > 337) || (playerAngle <= 22))
    {
        player_dir = north;
    }
    else if (playerAngle <= 67)
    {
        player_dir = northeast;
    }
    else if (playerAngle <= 112)
    {
        player_dir = east;
    }
    else if (playerAngle <= 157)
    {
        player_dir = southeast;
    }
    else if (playerAngle <= 202)
    {
        player_dir = south;
    }
    else if (playerAngle <= 247)
    {
        player_dir = southwest;
    }
    else if (playerAngle <= 292)
    {
        player_dir = west;
    }
    else if (playerAngle <= 337)
    {
        player_dir = northwest;
    }
                                    
    actor->SetDirection(valid_dir[player_dir][actor->GetState() == StateIdHidden ? 0 : 1]);

    if (actor->GetTemp1() == 0)
    {
        actor->SetTemp1(1000 + rand() % 2000);
    }

    const float harmlessBunnySpeed = 300.0f;
    const uint32_t deltaTimeInMs = m_timeStampOfWorldCurrentFrame - m_timeStampOfWorldPreviousFrame;
    const uint32_t truncatedDeltaTimeInMs = (deltaTimeInMs < 50) ? deltaTimeInMs : 50;
    float move = harmlessBunnySpeed * ((float)(truncatedDeltaTimeInMs) / 14.3f) / 65536.0f;

    // Calculate new (x,y) coordinates based on the direction of the bunny and the distance to travel.
    float x = actor->GetX();
    float y = actor->GetY();
    switch (actor->GetDirection())
    {
    case north:
        y -= move;
        break;
    case northeast:
        x += move;
        y -= move;
        break;
    case east:
        x += move;
        break;
    case southeast:
        x += move;
        y += move;
        break;
    case south:
        y += move;
        break;
    case southwest:
        x -= move;
        y += move;
        break;
    case west:
        x -= move;
        break;
    case northwest:
        x -= move;
        y -= move;
        break;
    case nodir:
        break;
    }

    bool changeDir = false;
    if (x > actor->GetTileX() + 0.2f && x < actor->GetTileX() + 0.8f &&
        y > actor->GetTileY() + 0.2f && y < actor->GetTileY() + 0.8f &&
        move < actor->GetTemp1() / 10000.0f)
    {
        actor->SetX(x);
        actor->SetY(y);
    }
    else
    {
        // The bunny is getting near the edge of his tile or the intended distance is traveled.
        // Change direction and go the other way
        changeDir = true;
    }

    if (changeDir)
    {
        // Set a new intended distance to travel.
        actor->SetTemp1(1000 + rand() % 2000);
        actor->SetState(actor->GetState() == StateIdPeekAlternative ? StateIdPeek : StateIdPeekAlternative, m_timeStampOfWorldCurrentFrame);
    }
    else
    {
        // Deduct the traveled distance from the intended distance.
        actor->SetTemp1(actor->GetTemp1() - (int16_t)(move * 10000.0f));
    }
}

void EngineCore::WarpInsideLevel(const Actor* sourceWarp)
{
    for (uint16_t y = 0; y < m_level->GetLevelHeight(); y++)
    {
        for (uint16_t x = 0; x < m_level->GetLevelWidth(); x++)
        {
            const Actor* actor = m_level->GetBlockingActor(x, y);
            if (actor != nullptr &&
                actor != sourceWarp &&
                actor->GetDecorateActor().spawnOnAllDifficulties == sourceWarp->GetDecorateActor().spawnOnAllDifficulties)
            {
                m_level->GetPlayerActor()->SetX(actor->GetX());
                m_level->GetPlayerActor()->SetY(actor->GetY());
                Thrust(0, 0.5f);
                Thrust(0, 0.5f);
                Thrust(0, 0.5f);

                // Update visibility map based on new location of player
                m_level->UpdateVisibilityMap();
            }
        }
    }
    m_setOverlayOnNextDraw = true;
}

void EngineCore::DisplayStatusMessage(const char* message, const uint16_t delayInMS)
{
    m_statusMessage = message;
    m_timeStampEndOfStatusMessage = m_timeStampOfPlayerCurrentFrame + delayInMS;
}

void EngineCore::KeyNPressed()
{
    if (m_state == RequestDifficultyLevel)
    {
        SelectDifficultyLevelNovice();
    }
    else if (m_state == VerifyGateExit)
    {
        // Stay on level and resume play
        m_state = InGame;
        m_warpToLevel = m_level->GetLevelIndex();
        m_gameTimer.Resume();

        // Keep the key
        if (m_keyToTake != NoKey)
        {
            m_keyToTake = NoKey;
        }
    }
}

void EngineCore::KeyYPressed()
{
    if (m_state == VerifyGateExit)
    {
        m_state = InGame;
        m_gameTimer.Resume();
    }
}
        
void EngineCore::PlayerUsesPotion()
{
    if (!m_level->GetPlayerActor()->IsDead() && m_level->GetPlayerActor()->GetHealth() < 100)
    {
        if (m_playerInventory.UsePotion())
        {
            m_level->GetPlayerActor()->SetHealth(100);
            DisplayStatusMessage("Curing", (uint16_t)(30 * (1000.0 / 70.0)));
        }
    }
}

void EngineCore::ShowGodModeCheatDialog()
{
    if (m_state == InGame && m_level != nullptr && m_level->GetPlayerActor()->GetHealth() > 0)
    {
        m_state = GodModeCheatDialog;
        m_godModeIsOn = !m_godModeIsOn;
        m_gameTimer.Pause();
    }
}

void EngineCore::ShowWarpCheatDialog()
{
    if (m_state == InGame && m_level != nullptr && m_level->GetPlayerActor()->GetHealth() > 0)
    {
        m_state = WarpCheatDialog;
        m_gameTimer.Pause();
    }
}

void EngineCore::ShowFreeItemsCheatDialog()
{
    if (m_state == InGame && m_level != nullptr && m_level->GetPlayerActor()->GetHealth() > 0)
    {
        m_state = FreeItemsCheatDialog;
        for (uint8_t i = 0; i < 4; i++)
        {
            m_playerInventory.GiveBolt();
            m_playerInventory.GiveNuke();
            m_playerInventory.GivePotion();
            m_playerInventory.GiveKey(i);
        }
        for (uint8_t i = 0; i < 5; i++)
        {
            m_playerInventory.GiveGem(i);
        }
        for (uint8_t i = 0; i < 8; i++)
        {
            m_playerInventory.GiveScroll(i);
        }
        m_gameTimer.Pause();
    }
}

void EngineCore::ShowAutoMap(const bool cheat)
{
    if (m_state == InGame && m_level != nullptr)
    {
        m_state = AutoMapDialog;
        m_autoMap.SetCheat(cheat);
        m_autoMap.ResetOrigin(*m_level, m_configurationSettings.GetCVarEnum(CVarIdAutoMapMode).GetItemIndex());
        m_gameTimer.Pause();
    }
}

void EngineCore::TextFieldInput(const char c)
{
    if (m_warpCheatTextField.size() < 2)
    {
        m_warpCheatTextField.append(&c, 1);
    }
}

void EngineCore::BackspacePressed()
{
    if (m_warpCheatTextField.size() > 0)
    {
        m_warpCheatTextField.erase(m_warpCheatTextField.size() - 1, 1);
    }
}

void EngineCore::FreezeTimeCheat()
{
    if (m_state != InGame)
    {
        return;
    }

    if (m_gameTimer.GetRemainingFreezeTime() == 0)
    {
        m_game.PlaySoundFreezeTime();
    }
    m_gameTimer.FreezeTime();
}

void EngineCore::WaitForAnyKeyPressed()
{
    m_playerActions.SetActionActive(MoveForward, false);
    m_playerActions.SetActionActive(MoveBackward, false);
    m_playerActions.SetActionActive(Shoot, false);
    m_playerActions.SetActionActive(ShootZappper, false);
    m_playerActions.SetActionActive(ShootXterminator, false);
    m_playerActions.SetActionActive(StrafeLeft, false);
    m_playerActions.SetActionActive(StrafeRight, false);
    m_playerActions.SetActionActive(TurnLeft, false);
    m_playerActions.SetActionActive(TurnRight, false);
    m_playerActions.SetActionActive(QuickTurn, false);
    m_playerActions.SetActionActive(Run, false);
}

bool EngineCore::RequiresMouseCapture() const
{
    return (!m_menu->IsActive() &&
           (m_state == EnteringLevel ||
            m_state == InGame ||
            m_state == WarpCheatDialog ||
            m_state == GodModeCheatDialog ||
            m_state == FreeItemsCheatDialog ||
            m_state == AutoMapDialog ||
            m_state == Victory ||
            m_state == VerifyGateExit));
}

bool EngineCore::IsOneTimeAction(const actorAction action)
{
    return (action == ActionAttack ||
            action == ActionDropItem ||
            action == ActionSpawnSkeleton ||
            action == ActionItemDestroyed ||
            action == ActionFreezeTime ||
            action == ActionGiveChest ||
            action == ActionGiveBolt ||
            action == ActionGiveNuke ||
            action == ActionGivePotion ||
            action == ActionGiveKey ||
            action == ActionGiveGem ||
            action == ActionGiveScroll ||
            action == ActionGivePoints ||
            action == ActionGivePointsForChest ||
            action == ActionExplodeWall1 ||
            action == ActionExplodeWall2 ||
            action == ActionExplodeWall3 ||
            action == ActionExplodeWall4 ||
            action == ActionWarpToOtherLevel ||
            action == ActionWarpInsideLevel ||
            action == ActionForceField ||
            action == ActionExplodeSound ||
            action == ActionSmallMonsterSound ||
            action == ActionLargeMonsterSound ||
            action == ActionPortalSound ||
            action == ActionBurningTree ||
            action == ActionFlash ||
            action == ActionVictory
        );
}

void EngineCore::OpenMenu()
{
    m_menu->SetActive(true);
    if (m_state == InGame || m_state == EnteringLevel)
    {
        m_gameTimer.Pause();
    }
    if (m_game.GetId() != GameId::Catacomb3Dv122)
    {
        m_game.PlaySoundWarpUpOrDown(true);
    }
}

void EngineCore::CloseMenu()
{
    m_menu->SetActive(false);
    if (m_state == InGame && !m_takingChest)
    {
        m_gameTimer.Resume();
    }
    if (m_game.GetId() != GameId::Catacomb3Dv122)
    {
        m_game.PlaySoundWarpUpOrDown(false);
    }
}

bool EngineCore::IsActionActive(const ControlAction action) const
{
    bool isActive = false;

    const std::vector<SDL_Keycode> keys = m_configurationSettings.GetConstControlsMap().GetKeysFromAction(action);
    for (SDL_Keycode key : keys)
    {
        isActive |= m_playerInput.IsKeyPressed(key);
    }
    if(!isActive)
    {
        const std::vector<uint8_t> buttons = m_configurationSettings.GetConstControlsMap().GetMouseButtonsFromAction(action);
        for (uint8_t button : buttons)
        {
            isActive |= m_playerInput.IsMouseButtonPressed(button);
        }
    }
    return isActive;
}

bool EngineCore::IsActionJustPressed(const ControlAction action) const
{
    bool isJustPressed = false;


    const std::vector<SDL_Keycode> keys = m_configurationSettings.GetConstControlsMap().GetKeysFromAction(action);
    for (SDL_Keycode key : keys)
    {
        isJustPressed |= m_playerInput.IsKeyJustPressed(key);
    }
    if(!isJustPressed)
    {
        const std::vector<uint8_t> buttons = m_configurationSettings.GetConstControlsMap().GetMouseButtonsFromAction(action);
        for (uint8_t button : buttons)
        {
            isJustPressed |= m_playerInput.IsMouseButtonJustPressed(button);
        }
    }
    return isJustPressed;
}

void EngineCore::StartNewGameWithDifficultySelection()
{
    m_state = RequestDifficultyLevel;
    m_playerInventory.ResetForNewGame();
    UnloadLevel();
    m_score.Reset();
}

void EngineCore::StartNewGame()
{
    m_state = EnteringLevel;
    m_playerInventory.ResetForNewGame();
    UnloadLevel();
    m_timeStampToEnterGame = m_gameTimer.GetActualTime() + 2000u;
    m_warpToLevel = 0;
    m_score.Reset();
}

void EngineCore::UnloadLevel()
{
    delete m_level;
    m_level = nullptr;
}

bool EngineCore::StoreGameToFileWithFullPath(const fs::path filename) const
{
    bool result = false;

    if (m_level == nullptr)
    {
        return false;
    }

    std::ofstream file;
    file.open(filename, std::ofstream::binary);
    if (file.is_open())
    {
        const char headerString[11] = "CATACOMBGL";
        file.write(headerString, sizeof(headerString));
        file.write((const char*)&versionMajor, sizeof(versionMajor));
        file.write((const char*)&versionMinor, sizeof(versionMinor));
        const uint8_t persistentGameId = getPersistentGameId(m_game.GetId());
        file.write((const char*)&persistentGameId, sizeof(persistentGameId));
        file.write((const char*)&m_difficultyLevel, sizeof(m_difficultyLevel));
        file.write((const char*)&m_godModeIsOn, sizeof(m_godModeIsOn));
        m_playerInventory.StoreToFile(file);
        m_level->StoreToFile(file);
        m_gameTimer.StoreToFile(file);
        // The amount of points scored by the player gets stored since version 0.4.0
        const uint32_t points = m_score.GetPoints();
        file.write((const char*)&points, sizeof(points));
        file.close();
        result = true;
    }
    else
    {
        Logging::Instance().AddLogMessage("WARNING: Unable to write to file " + filename.string());
    }
    return result;
}

bool EngineCore::StoreGameToFile(const std::string filename)
{
    const fs::path filenamePath = m_system.GetConfigurationFilePath();
    const fs::path filenamePathForGame = filenamePath / m_game.GetSavedGamesPath();
    if (m_system.CreatePath(filenamePathForGame))
    {
        const fs::path fullPath = filenamePathForGame / ( filename + ".sav" );
        return StoreGameToFileWithFullPath(fullPath);
    }
    else
    {
        Logging::Instance().AddLogMessage("WARNING: Unable to create path " + filenamePathForGame.string());
    }

    return false;
}

void EngineCore::LoadGameFromFileWithFullPath(const fs::path filename)
{
    std::ifstream file;
    file.open(filename, std::ifstream::binary);
    if (file.is_open())
    {
        Logging::Instance().AddLogMessage("Loading saved game " + filename.string());
        char headerString[11];
        file.read(headerString, 11);
        if (file.fail())
        {
            Logging::Instance().AddLogMessage("WARNING: Unable to read header from " + filename.string());
            file.close();
            return;
        }
        uint8_t versionMajorRead = 0;
        file.read((char*)&versionMajorRead, sizeof(versionMajorRead));
        uint8_t versionMinorRead = 0;
        file.read((char*)&versionMinorRead, sizeof(versionMinorRead));
        uint8_t gameId = 0;
        file.read((char*)&gameId, sizeof(gameId));
        file.read((char*)&m_difficultyLevel, sizeof(m_difficultyLevel));
        file.read((char*)&m_godModeIsOn, sizeof(m_godModeIsOn));
        m_playerInventory.LoadFromFile(file);
        UnloadLevel();
        m_level = m_game.GetGameMaps()->GetLevelFromSavedGame(file);
        m_level->LoadActorsFromFile(file, m_game.GetDecorateActors());
        if (versionMajorRead > 0 || versionMinorRead >= 5)
        {
            // The fog of war map gets stored since version 0.5.0
            m_level->LoadFogOfWarFromFile(file);
        }
        m_gameTimer.LoadFromFile(file);
        if (versionMajorRead > 0 || versionMinorRead >= 4)
        {
            // The amount of points scored by the player gets stored since version 0.4.0
            long points = 0;
            file.read((char*)&points, sizeof(long));
            m_score.SetPoints(points);
        }
        file.close();

        // Temporarily load the same level from scratch to setup the level statistics correctly.
        Level* levelFromScratch = m_game.GetGameMaps()->GetLevelFromStart(m_level->GetLevelIndex());
        m_game.SpawnActors(levelFromScratch, m_difficultyLevel);
        m_levelStatistics.SetCountersAtStartOfLevel(*levelFromScratch);
        delete levelFromScratch;

        // Now count how many monsters/secrets/items are remaining
        m_levelStatistics.UpdateMonstersKilled(*m_level);
        m_levelStatistics.UpdateSecrets(*m_level);
        m_levelStatistics.UpdateItems(*m_level);

        m_playerActions.ResetForNewLevel();
        m_manaBar.Reset(m_configurationSettings.GetCVarBool(CVarIdManaBar).IsEnabled());
        m_warpToLevel = m_level->GetLevelIndex();
        m_menu->SetActive(false);
        m_state = InGame;

        const uint32_t currentTimestampOfPlayer = m_gameTimer.GetMillisecondsForPlayer();
        const uint32_t currentTimestampOfWorld = m_gameTimer.GetMilliSecondsForWorld();

        m_timeStampOfPlayerPreviousFrame = m_timeStampOfPlayerCurrentFrame;
        m_timeStampOfPlayerCurrentFrame = currentTimestampOfPlayer;
        m_timeStampOfWorldPreviousFrame = m_timeStampOfWorldCurrentFrame;
        m_timeStampOfWorldCurrentFrame = currentTimestampOfWorld;
        m_timeStampFadeEffect = 0;
    }
    else
    {
        Logging::Instance().AddLogMessage("WARNING: Unable to open file " + filename.string());
    }
}

void EngineCore::LoadGameFromFile(const std::string filename)
{
    const fs::path filenamePath = m_system.GetConfigurationFilePath();
    const fs::path filenamePathAbyss = filenamePath / m_game.GetSavedGamesPath();
    const fs::path fullPath = filenamePathAbyss / ( filename + ".sav" );
    LoadGameFromFileWithFullPath(fullPath);
}

void EngineCore::LoadDosGameFromFile(const std::string filename)
{
    const SavedGameInDosFormat* const savedGame = m_savedGamesInDosFormat.GetSavedGameInDosFormat(filename);

    if (savedGame == nullptr)
    {
        Logging::Instance().AddLogMessage("ERROR: Unable to load DOS saved game " + filename);
    }
    else
    {
        m_playerInventory.LoadFromDosGame(*savedGame);
        UnloadLevel();
        m_level = m_game.GetGameMaps()->GetLevelFromDosSavedGame(savedGame);
        m_level->LoadActorsFromDosSavedGame(*savedGame, m_game.GetSavedGameConverter(), m_game.GetDecorateActors());
        m_score.SetPoints(savedGame->GetScore());

        m_difficultyLevel =
            (savedGame->GetDifficulty() == 1) ? DifficultyLevel::Easy :
            (savedGame->GetDifficulty() == 2) ? DifficultyLevel::Normal :
            DifficultyLevel::Hard;

        // Information that is not stored in the DOS format is set to default values.
        m_godModeIsOn = false;

        // TODO
        // m_level->LoadActors
        // m_gameTimer.LoadFromFile(file)
        m_gameTimer.Reset();

        // Temporarily load the same level from scratch to setup the level statistics correctly.
        Level* levelFromScratch = m_game.GetGameMaps()->GetLevelFromStart(m_level->GetLevelIndex());
        m_game.SpawnActors(levelFromScratch, m_difficultyLevel);
        m_levelStatistics.SetCountersAtStartOfLevel(*levelFromScratch);
        delete levelFromScratch;

        // Now count how many monsters/secrets/items are remaining
        m_levelStatistics.UpdateMonstersKilled(*m_level);
        m_levelStatistics.UpdateSecrets(*m_level);
        m_levelStatistics.UpdateItems(*m_level);

        m_playerActions.ResetForNewLevel();
        m_manaBar.Reset(m_configurationSettings.GetCVarBool(CVarIdManaBar).IsEnabled());
        m_warpToLevel = m_level->GetLevelIndex();
        m_menu->SetActive(false);
        m_state = InGame;

        const uint32_t currentTimestampOfPlayer = m_gameTimer.GetMillisecondsForPlayer();
        const uint32_t currentTimestampOfWorld = m_gameTimer.GetMilliSecondsForWorld();

        m_timeStampOfPlayerPreviousFrame = m_timeStampOfPlayerCurrentFrame;
        m_timeStampOfPlayerCurrentFrame = currentTimestampOfPlayer;
        m_timeStampOfWorldPreviousFrame = m_timeStampOfWorldCurrentFrame;
        m_timeStampOfWorldCurrentFrame = currentTimestampOfWorld;
        m_timeStampFadeEffect = 0;
    }
}

uint8_t EngineCore::GetScreenMode() const
{
    return m_configurationSettings.GetCVarEnum(CVarIdScreenMode).GetItemIndex();
}

bool EngineCore::AreScrollsPresent() const
{
    bool scrollsArePresent = false;
    auto decorateActors = m_game.GetDecorateActors();
    for (auto decorateActorPair : decorateActors)
    {
        for (auto decorateStatePair : decorateActorPair.second.states)
        {
            auto anim = decorateStatePair.second.animation;
            for (uint8_t frameIndex = 0; frameIndex < anim.size(); frameIndex++)
            {
                if (anim.at(frameIndex).action == ActionGiveScroll)
                {
                    scrollsArePresent = true;
                }
            }
        }
    }

    return scrollsArePresent;
}

void EngineCore::StartMusicIfNeeded()
{
    if (IsMusicSetToAdlibInConfiguration() &&
        (m_state == InGame || m_state == GodModeCheatDialog || m_state == FreeItemsCheatDialog || m_state == WarpCheatDialog || m_state == AutoMapDialog) &&
        !m_menu->IsActive())
    {
        m_game.GetAudioPlayer()->StartMusic(0);
    }
}

bool EngineCore::IsMusicSetToAdlibInConfiguration() const
{
    const GameId gameId = m_game.GetId();
    const uint8_t applicableMusicModeCVar = (gameId == GameId::Catacomb3Dv122) ? CVarIdMusicMode : CVarIdMusicModeAdventureTrilogy;
    return (m_configurationSettings.GetCVarEnum(applicableMusicModeCVar).GetItemIndex() == CVarItemIdMusicModeAdlib);
}
