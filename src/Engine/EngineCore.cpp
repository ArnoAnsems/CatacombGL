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
#include "..\..\ThirdParty\RefKeen\id_sd.h"
#include "LevelLocationNames.h"
#include <math.h>
#include <fstream>

const uint8_t versionMajor = 0;
const uint8_t versionMinor = 3;
const uint8_t versionLevel = 0;
const std::string versionPhase = "Alpha";

const uint8_t VictoryStatePlayGetBolt = 0;
const uint8_t VictoryStatePlayingGetBolt = 1;
const uint8_t VictoryStatePlayGetNuke = 2;
const uint8_t VictoryStatePlayingGetNuke = 3;
const uint8_t VictoryStatePlayGetPotion = 4;
const uint8_t VictoryStatePlayingGetPotion = 5;
const uint8_t VictoryStatePlayGetKey = 6;
const uint8_t VictoryStatePlayingGetKey = 7;
const uint8_t VictoryStatePlayGetScroll = 8;
const uint8_t VictoryStatePlayingGetScroll = 9;
const uint8_t VictoryStatePlayGetPoint = 10;
const uint8_t VictoryStatePlayingGetPoint = 11;
const uint8_t VictoryStateDone = 12;

EngineCore::EngineCore(IGame& game, const ISystem& system, PlayerInput& keyboardInput, ConfigurationSettings& configurationSettings) :
    m_gameTimer(),
    m_game(game),
    m_system(system),
    m_level(NULL),
    m_readingScroll(255),
    m_takingChest(false),
    m_warpToLevel(0),
    m_timeStampOfPlayerCurrentFrame(0),
    m_timeStampOfPlayerPreviousFrame(0),
    m_timeStampOfWorldCurrentFrame(0),
    m_timeStampOfWorldPreviousFrame(0),
    m_state(Introduction),
    m_difficultyLevel(Easy),
    m_statusMessage(NULL),
    m_timeStampEndOfStatusMessage(0),
    m_warpCheatTextField(""),
    m_godModeIsOn(false),
    m_victoryState(VictoryStatePlayGetBolt),
    m_lastFreezeTimeTick(0),
    m_playerInventory(game),
    m_playerActions(),
    m_startTakeKey(0),
    m_timeStampToEnterGame(0),
    m_timeStampLastMouseMoveForward(0),
    m_timeStampLastMouseMoveBackward(0),
    m_keyToTake(KeyId::NoKey),
    m_playerInput(keyboardInput),
    m_savedGames(),
    m_menu(game.CreateMenu(configurationSettings, m_savedGames)),
    m_configurationSettings(configurationSettings),
    m_scrollsArePresent(AreScrollsPresent())
{
    _sprintf_p(m_messageInPopup, 256, "");
    m_gameTimer.Reset();

    const std::string filenamePath = m_system.GetConfigurationFilePath();
    const std::string savedGamesAbyssPath = filenamePath + m_game.GetSavedGamesPath();
    m_system.GetSavedGameNamesFromFolder(savedGamesAbyssPath, m_savedGames);

    // Pre-load game data from disk
    m_game.GetEgaGraph();
    m_game.GetAudioRepository();
    m_game.GetGameMaps();
}

EngineCore::~EngineCore()
{
    UnloadLevel();
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
    const int16_t health = (m_level == NULL) ? 100 : m_level->GetPlayerActor()->GetHealth();
    UnloadLevel();

    m_level = m_game.GetGameMaps()->GetLevelFromStart(mapIndex);

    m_game.SpawnActors(m_level, m_difficultyLevel);
    m_level->GetPlayerActor()->SetHealth(health);

    m_timeStampOfPlayerCurrentFrame = 0;
    m_timeStampOfPlayerPreviousFrame = 0;
    m_timeStampOfWorldCurrentFrame = 0;
    m_timeStampOfWorldPreviousFrame = 0;
    m_timeStampLastMouseMoveForward = 0;
    m_timeStampLastMouseMoveBackward = 0;
    m_gameTimer.Reset();
    m_playerActions.ResetForNewLevel();
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
    if (m_level == NULL)
    {
        renderer.SetPlayerAngle(0.0f);
        renderer.SetPlayerPosition(0.0f, 0.0f);
    }
    else
    {
        renderer.SetPlayerAngle(m_level->GetPlayerActor()->GetAngle());
        renderer.SetPlayerPosition(m_level->GetPlayerActor()->GetX(), m_level->GetPlayerActor()->GetY());
    }

    renderer.SetVSync(m_configurationSettings.GetVSync());

    m_framesCounter.AddFrame(m_gameTimer.GetActualTime());
    renderer.SetTextureFilter(m_configurationSettings.GetTextureFilter());

    renderer.Prepare3DRendering(m_configurationSettings.GetDepthShading(), aspectRatios[m_configurationSettings.GetAspectRatio()].ratio, m_configurationSettings.GetFov(), m_game.GetOriginal3DViewArea());

    if (m_readingScroll == 255 && (m_state == InGame || m_state == WarpCheatDialog || m_state == GodModeCheatDialog || m_state == FreeItemsCheatDialog || (m_state == Victory && m_victoryState != VictoryStateDone) || m_state == VerifyGateExit))
    {
        m_level->DrawFloorAndCeiling(renderer, m_timeStampOfWorldCurrentFrame);
         
        m_level->DrawWalls(renderer, m_game.GetEgaGraph(), m_gameTimer.GetTicksForWorld());

#ifdef DRAWVISIBILITYMAP
        m_level->DrawVisibilityMap(renderer);
#endif
        m_level->DrawActors(renderer, m_game.GetEgaGraph());
    }

    renderer.Prepare2DRendering(m_state == Help);

    if (m_state == Help)
    {
        DrawTiledWindow(renderer, 1, 1, 78, 23);
        m_game.DrawHelpPage();
    }

    if (m_state == InGame)
    {
        if (m_readingScroll != 255)
        {
            // Read scroll
            m_game.DrawScroll(m_readingScroll);
        }
        else
        {
            const uint16_t statusbarHeight = (m_game.GetId() == 5) ? 144 : 120;
            renderer.Render2DPicture(m_game.GetEgaGraph()->GetMaskedPicture(m_game.GetEgaGraph()->GetHandPictureIndex()), 120, statusbarHeight - m_playerActions.GetHandHeight()/*50*/);
                    
#ifdef DRAWTILEINFO
            char tileStr[40];
            sprintf_s(tileStr, 40, "tile: %d", m_level->GetFloorTile((uint16_t)GetPlayerState()->GetX(), (uint16_t)GetPlayerState()->GetY()));
            renderer.RenderTextLeftAligned(tileStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,2,2);
            sprintf_s(tileStr, 40, "wall: %d", m_level->GetWallTile((uint16_t)GetPlayerState()->GetX(), (uint16_t)GetPlayerState()->GetY()));
            renderer.RenderTextLeftAligned(tileStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,2,12);
            sprintf_s(tileStr, 40, "X: %d Y: %d Angle: %d", (uint16_t)GetPlayerState()->GetX(), (uint16_t)GetPlayerState()->GetY(), (uint16_t)GetPlayerState()->GetAngle());
            renderer.RenderTextLeftAligned(tileStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,2,22);
            sprintf_s(tileStr, 40, "MouseX: %d", m_playerControls.GetMouseXPos());
            renderer.RenderTextLeftAligned(tileStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,2,32);
#endif
        }

        if (m_configurationSettings.GetShowFps())
        {
            char fpsStr[10];
            sprintf_s(fpsStr, 10, "%d", m_framesCounter.GetFramesPerSecond());
            renderer.RenderTextLeftAligned(fpsStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,280,2);
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
        else if (m_readingScroll != 255)
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
            }
        }
    }

    if (m_state == InGame || m_state == EnteringLevel || m_state == WarpCheatDialog || m_state == GodModeCheatDialog || m_state == FreeItemsCheatDialog || m_state == Victory || m_state == VerifyGateExit || m_state == ExitGame)
    {
        const int16_t playerHealth = (m_level != 0) ? m_level->GetPlayerActor()->GetHealth() : 100;
        const float playerAngle = (m_level != 0) ? m_level->GetPlayerActor()->GetAngle() : 0.0f;
        const uint8_t levelIndex = (m_level != 0) ? m_level->GetLevelIndex() : 0;
        m_game.DrawStatusBar(playerHealth, locationMessage, m_playerInventory, renderer.GetAdditionalMarginDueToWideScreen(aspectRatios[m_configurationSettings.GetAspectRatio()].ratio), playerAngle, levelIndex);

        if (m_state != Victory)
        {
            const int32_t remainingFreezeTime = m_gameTimer.GetRemainingFreezeTime();
            const int32_t remainingFreezeTimeInSec = remainingFreezeTime / 1000;
            if (remainingFreezeTimeInSec != m_lastFreezeTimeTick)
            {
                m_lastFreezeTimeTick = remainingFreezeTimeInSec;
                m_game.PlaySoundFreezeTimeTick(m_lastFreezeTimeTick);
            }

            if (m_game.GetId() != 5)  // Status message and radar not in Catacomb 3-D
            {
                char freezeMessage[100];
                sprintf_s(freezeMessage, "Time Stopped: %d", remainingFreezeTimeInSec);
                const char* statusMessage = (m_statusMessage != NULL) ? m_statusMessage : (remainingFreezeTime != 0) ? freezeMessage : m_playerActions.GetStatusMessage();
                renderer.RenderTextCentered(statusMessage, m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow, 156, 189);

                // Radar
                if (m_state == InGame || m_state == WarpCheatDialog || m_state == GodModeCheatDialog || m_state == FreeItemsCheatDialog || m_state == VerifyGateExit)
                {
                    const float radarCenterX = (31 * 8) + (51 / 2) + 2;
                    const float radarCenterY = (200 - 11 - 8) - (51 / 2) - 2;
                    const float radarXRadius = 113.0f / 5.0f;
                    const float radarYRadius = 113.0f / 7.0f;
                    const float northIconoffsetFromCenterX = -(radarXRadius * sin(m_level->GetPlayerActor()->GetAngle() * 3.14159265f / 180.0f));
                    const float northIconoffsetFromCenterY = -(radarYRadius * cos(m_level->GetPlayerActor()->GetAngle() * 3.14159265f / 180.0f));
                    const uint16_t northIconScreenOffsetX = (uint16_t)(radarCenterX + northIconoffsetFromCenterX - 3);
                    const uint16_t northIconScreenOffsetY = (uint16_t)(radarCenterY + northIconoffsetFromCenterY - 3);
                    renderer.Render2DPicture(m_game.GetEgaGraph()->GetSprite(m_game.GetNorthIconSprite()), northIconScreenOffsetX, northIconScreenOffsetY);
                    renderer.RenderRadarBlip(radarCenterX, radarCenterY, EgaBrightWhite);

                    for (uint16_t blipIndex = 0; blipIndex < m_radarModel.GetNumberOfBlips(); blipIndex++)
                    {
                        const radarBlip blip = m_radarModel.GetRadarBlip(blipIndex);
                        renderer.RenderRadarBlip(radarCenterX + blip.offsetX, radarCenterY + blip.offsetY, blip.color);
                    }
                }
            }
        }
    }
    
    if ( m_state == EnteringLevel)
    {
        if (m_game.GetId() == 5)
        {
            // Catacomb 3-D
            const uint16_t margin = renderer.GetAdditionalMarginDueToWideScreen(aspectRatios[m_configurationSettings.GetAspectRatio()].ratio);
            renderer.Render2DBar(0 - margin, 0, 264 + (2 * margin), 144, EgaBrightBlue);
            renderer.Render2DPicture(m_game.GetEgaGraph()->GetPicture(159), 57, 52);
            renderer.RenderTextCentered(m_level->GetLevelName(), m_game.GetEgaGraph()->GetFont(3), EgaDarkGray, 132, 76);
        }
        else
        {
            // Catacomb Adventure series
            renderer.Render2DBar(0, 0, 320, 120, EgaBlack);
            uint16_t width = (uint16_t)strlen(m_level->GetLevelName());
            if (width == 0)
            {

                const char* enterAreaText = (m_game.GetId() == 3) ? "You enter a new area ..." : "A new challenge awaits you.";
                width = (uint16_t)strlen(enterAreaText);
                DrawCenteredTiledWindow(renderer, width, 3);
                renderer.RenderTextCentered(enterAreaText, m_game.GetEgaGraph()->GetFont(3), EgaDarkGray, 160, 56);
            }
            else
            {
                if (width < 20)
                {
                    width = 20;
                }
                DrawCenteredTiledWindow(renderer, width, 5);
                renderer.RenderTextCentered("You have arrived at", m_game.GetEgaGraph()->GetFont(3), EgaDarkGray, 160, 49);
                renderer.RenderTextCentered(m_level->GetLevelName(), m_game.GetEgaGraph()->GetFont(3), EgaDarkGray, 160, 58);
            }
        }

    }

    if (m_state == InGame)
    {
        if (_strcmpi(m_messageInPopup, "") != 0)
        {
            DrawCenteredTiledWindow(renderer, 20, 4);
            renderer.RenderTextCentered(m_messageInPopup, m_game.GetEgaGraph()->GetFont(3), EgaDarkGray, 160, 49);
        }
    }

    if (m_state == WarpCheatDialog)
    {
        DrawCenteredTiledWindow(renderer, 26, 3);
        const uint8_t lastLevel = m_game.GetGameMaps()->GetNumberOfLevels() - 1;
        const std::string warpText = "Warp to which level(0-" + std::to_string(lastLevel) + "):" + m_warpCheatTextField;
        renderer.RenderTextLeftAligned(warpText.c_str(), m_game.GetEgaGraph()->GetFont(3), EgaDarkGray, 70, 56);
    }

    if (m_state == GodModeCheatDialog)
    {
        DrawCenteredTiledWindow(renderer, 12, 2);
        const std::string godModeText = m_godModeIsOn ? "God mode ON" : "God mode OFF";
        renderer.RenderTextCentered(godModeText.c_str(), m_game.GetEgaGraph()->GetFont(3), EgaDarkGray, 160, 52);
    }

    if (m_state == FreeItemsCheatDialog)
    {
        DrawCenteredTiledWindow(renderer, 12, 2);
        renderer.RenderTextCentered("Free items!", m_game.GetEgaGraph()->GetFont(3), EgaDarkGray, 160, 52);
    }

    if (m_level != NULL && m_level->GetPlayerActor()->IsDead() && m_playerInventory.GetPotions() > 0 && m_game.GetId() != 5)
    {
        DrawCenteredTiledWindow(renderer, 35, 3);
        renderer.RenderTextCentered("You should use your Cure Potions wisely", m_game.GetEgaGraph()->GetFont(3), EgaDarkGray, 160, 56);
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

#ifdef DRAWTIME
    char ticsStr[40];
    sprintf_s(ticsStr, 40, "tics (player): %d", m_gameTimer.GetTicksForPlayer());
    renderer->RenderTextLeftAligned(ticsStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,2,2);
    sprintf_s(ticsStr, 40, "tics (world): %d", m_gameTimer.GetTicksForWorld());
    renderer->RenderTextLeftAligned(ticsStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,150,2);
    sprintf_s(ticsStr, 40, "msec (player): %d", m_gameTimer.GetMillisecondsForPlayer());
    renderer->RenderTextLeftAligned(ticsStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,2,12);
    sprintf_s(ticsStr, 40, "msec (world): %d", m_gameTimer.GetMilliSecondsForWorld());
    renderer->RenderTextLeftAligned(ticsStr,m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow,150,12);
#endif

#ifdef DRAWFONT
    for (int x = 0; x < 16; x++)
    {
        {
            for (int y = 0; y < 16; y++)
            {
                char s[2];
                s[0] = (y * 16) + x;
                s[1] = 0;
                
                renderer.RenderTextLeftAligned(s, m_game.GetEgaGraph()->GetFont(3), EgaBrightYellow, x * 16, y * 10);
            }
        }
    }
#endif

    if (m_menu->IsActive())
    {
        if (m_game.GetId() != 5)
        {
            DrawTiledWindow(renderer, 2, 1, 36, 13);
        }
        m_menu->Draw(renderer, m_game.GetEgaGraph(), m_game.GetMenuCursorPic(), m_gameTimer.GetActualTime());
    }
    
    renderer.Unprepare2DRendering();
}

// Based on US_CenterWindow in ID_US.C of the Catacomb Abyss source code.
void EngineCore::DrawCenteredTiledWindow(IRenderer& renderer, const uint16_t width, const uint16_t height)
{
    DrawTiledWindow(renderer, (40 - width) / 2,(15 - height) / 2,width,height);
}

// Based on US_DrawWindow in ID_US.C of the Catacomb Abyss source code.
void EngineCore::DrawTiledWindow(IRenderer& renderer, const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height)
{
    const uint16_t sx = (x - 1) * 8;
    const uint16_t sy = (y - 1) * 8;
    const uint16_t sw = (width + 1) * 8;
    const uint16_t sh = (height + 1) * 8;

    renderer.Render2DBar(x * 8, y * 8, width * 8, height * 8, EgaLightGray);

    renderer.Render2DPicture(m_game.GetEgaGraph()->GetTilesSize8Masked(0), sx, sy);
    renderer.Render2DPicture(m_game.GetEgaGraph()->GetTilesSize8Masked(6), sx, sy + sh);

    for (uint16_t i = sx + 8;i <= sx + sw - 8;i += 8)
    {
        renderer.Render2DPicture(m_game.GetEgaGraph()->GetTilesSize8Masked(1), i, sy);
        renderer.Render2DPicture(m_game.GetEgaGraph()->GetTilesSize8Masked(7), i, sy + sh);
    }
    renderer.Render2DPicture(m_game.GetEgaGraph()->GetTilesSize8Masked(2), sx + sw, sy);
    renderer.Render2DPicture(m_game.GetEgaGraph()->GetTilesSize8Masked(8), sx + sw, sy + sh);

    for (uint16_t i = sy + 8;i <= sy + sh - 8;i += 8)
    {
        renderer.Render2DPicture(m_game.GetEgaGraph()->GetTilesSize8Masked(3), sx, i);
        renderer.Render2DPicture(m_game.GetEgaGraph()->GetTilesSize8Masked(5), sx + sw, i);
    }
}

void EngineCore::EnterKeyReleased()
{
    if (!m_menu->IsActive())
    {
        if (m_readingScroll != 255)
        {
            m_readingScroll = 255;
            m_gameTimer.Resume();
        }

        if (m_state == Introduction)
        {
            if (m_game.GetId() == 5)
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
            const int level = atoi(m_warpCheatTextField.c_str());
            if (level >= 0 && level < m_game.GetGameMaps()->GetNumberOfLevels())
            {
                m_warpToLevel = (uint8_t)level;
            }
            m_warpCheatTextField.clear();
            m_gameTimer.Resume();
        }
        else if (m_state == GodModeCheatDialog)
        {
            m_state = InGame;
            m_gameTimer.Resume();
        }
        else if (m_state == FreeItemsCheatDialog)
        {
            m_state = InGame;
            m_gameTimer.Resume();
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

    SDMode soundMode = SD_GetSoundMode();
    if (m_configurationSettings.GetSoundMode() == 0 && soundMode != sdm_Off)
    {
        SD_SetSoundMode(sdm_Off);
    }
    else if (m_configurationSettings.GetSoundMode() == 1 && soundMode != sdm_PC)
    {
        SD_SetSoundMode(sdm_PC);
    }
    else if (m_configurationSettings.GetSoundMode() == 2 && soundMode != sdm_AdLib)
    {
        SD_SetSoundMode(sdm_AdLib);
    }

    const SMMode musicMode = SD_GetMusicMode();
    if (m_configurationSettings.GetMusicOn() && musicMode == smm_Off)
    {
        SD_SetMusicMode(smm_AdLib);
    }
    else if (!m_configurationSettings.GetMusicOn() && musicMode == smm_AdLib)
    {
        SD_SetMusicMode(smm_Off);
    }

    if (m_menu->IsActive())
    {
        m_menu->SetSaveGameEnabled((m_state == InGame || m_state == WarpCheatDialog || m_state == GodModeCheatDialog || m_state == FreeItemsCheatDialog) && !m_level->GetPlayerActor()->IsDead());
        const MenuCommand command = m_menu->ProcessInput(m_playerInput);
        if (command == MenuCommandStartNewGame)
        {
            StartNewGameWithDifficultySelection();
        }
        else if (command == MenuCommandStartNewGameEasy)
        {
            m_difficultyLevel = Easy;
            StartNewGame();
        }
        else if (command == MenuCommandStartNewGameNormal)
        {
            m_difficultyLevel = Normal;
            StartNewGame();
        }
        else if (command == MenuCommandStartNewGameHard)
        {
            m_difficultyLevel = Hard;
            StartNewGame();
        }
        else if (command == MenuCommandExitGame)
        {
            m_state = ExitGame;
            DisplayStatusMessage("FARE THEE WELL!", 2000);
            m_gameTimer.Resume();
        }
        else if (command == MenuCommandSaveGame)
        {
            const std::string& saveGameName = m_menu->GetNewSaveGameName();
            if (StoreGameToFile(saveGameName))
            {
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
                    return false;
                }
            }
        }
        else if (command == MenuCommandLoadGame)
        {
            const std::string& saveGameName = m_menu->GetNewSaveGameName();
            LoadGameFromFile(saveGameName);
            if (m_game.GetId() == 5 && m_configurationSettings.GetMusicOn())
            {
                m_game.GetAudioPlayer()->StartMusic(0);
            }
            m_playerInput.ClearJustPressed();
            return false;
        }
        else if (command == MenuCommandCloseMenu)
        {
            CloseMenu();
            if (m_game.GetId() == 5 && m_state == InGame && m_configurationSettings.GetMusicOn())
            {
                m_game.GetAudioPlayer()->StartMusic(0);
            }
            m_playerInput.ClearJustPressed();
            return false;
        }
    }

    if (m_state == InGame && !m_menu->IsActive())
    {
        for (uint8_t i = (uint8_t)MoveForward; i < (uint8_t)MaxControlAction; i++)
        {
	        m_playerActions.SetActionActive((ControlAction)i, IsActionActive((ControlAction)i));
        }

        if (!m_configurationSettings.GetMouseLook())
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

    if (!m_playerInput.HasFocus() && m_state == InGame && !m_menu->IsActive())
    {
        OpenMenu();
        m_game.GetAudioPlayer()->StopMusic();
        m_playerInput.ClearAll();
    }


    if (m_playerInput.IsKeyJustPressed(SDLK_RETURN))
    {
        EnterKeyReleased();
    }
    if (!m_menu->IsActive())
    {
        for (uint8_t i = 0x31; i < 0x39; i++)
        {
            if (m_playerInput.IsKeyJustPressed(i))
            {
                ReadScroll(i - 0x31);
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
        if (m_playerInput.IsKeyJustPressed(SDLK_w)) // W
        {
            KeyWPressed();
        }
        if (m_playerInput.IsKeyJustPressed(SDLK_y)) // Y
        {
            KeyYPressed();
        }
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

    if (m_state != Help && !m_menu->IsActive() && m_playerInput.IsKeyJustPressed(SDLK_ESCAPE)) // Escape
    {
        OpenMenu();
        m_game.GetAudioPlayer()->StopMusic();
    }
    else if (m_state == Help)
    {
        if (m_game.ProcessInputOnHelpPage(m_playerInput))
        {
            m_state = InGame;
        }
    }
    else if (m_state == InGame && m_playerInput.IsKeyJustPressed(SDLK_F1))
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
        m_menu->SetActive(true);
    }

    // Status message
    if (m_timeStampOfPlayerCurrentFrame > m_timeStampEndOfStatusMessage)
    {
        m_statusMessage = NULL;

        if (m_state == ExitGame)
        {
            return true;
        }
    }

    if (m_state == EnteringLevel)
    {
        if (m_timeStampToEnterGame < m_gameTimer.GetActualTime())
        {
            m_state = InGame;
            if (m_game.GetId() == 5 && m_configurationSettings.GetMusicOn())
            {
                m_game.GetAudioPlayer()->StartMusic(0);
            }
        }
    }

    if (m_state == InGame || m_state == EnteringLevel)
    {
        if (m_level == NULL)
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
        if (!m_level->GetPlayerActor()->IsDead())
        {
            if (m_keyToTake != NoKey)
            {
                if (m_startTakeKey != 0)
                {
                    if (m_startTakeKey + 2000 < m_gameTimer.GetActualTime())
                    {
                        _sprintf_p(m_messageInPopup, 256, "");
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
                    _sprintf_p(m_messageInPopup, 256, "You use a %s key", GetKeyName((KeyId)m_keyToTake));
                    m_game.PlaySoundUseKey();
                }   
            }

            if (_strcmpi(m_messageInPopup, "") != 0)
            {
                if (m_startTakeKey == 0 && m_playerInput.IsAnyKeyPressed())
                {
                    _sprintf_p(m_messageInPopup, 256, "");
                    m_gameTimer.Resume();
                }
            }

            if (m_timeStampOfPlayerCurrentFrame > m_timeStampOfPlayerPreviousFrame)
            {
                if (m_playerActions.UpdateShoot(m_timeStampOfPlayerCurrentFrame))
                {
                    const auto decorateProjectilePair = m_game.GetDecorateActors().find(m_level->GetPlayerActor()->GetDecorateActor().projectileId);
                    if (decorateProjectilePair != m_game.GetDecorateActors().end())
                    {
                        Actor* projectile = new Actor(m_level->GetPlayerActor()->GetX(), m_level->GetPlayerActor()->GetY(), m_timeStampOfPlayerCurrentFrame, decorateProjectilePair->second);
                        projectile->SetAngle(m_level->GetPlayerActor()->GetAngle());
                        projectile->SetActive(true);
                        m_level->AddNonBlockingActor(projectile);
                    }

                    m_game.PlaySoundShoot();
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
                    const auto decorateProjectilePair = m_game.GetDecorateActors().find(m_level->GetPlayerActor()->GetDecorateActor().projectileId);
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
                const float deltaTimeInTics = (truncatedDeltaTimeInMs * 60.0f) / 1000.0f;
                const float turnSpeedFactor = m_configurationSettings.GetTurnSpeed() / 100.0f;
                if (m_playerActions.GetActionActive(TurnLeft))
                {
                    const float deltaDegrees = degreesPerTic * deltaTimeInTics * turnSpeedFactor;
                    m_level->GetPlayerActor()->SetAngle(m_level->GetPlayerActor()->GetAngle() - deltaDegrees);
                }
                if (m_playerActions.GetActionActive(TurnRight))
                {
                    const float deltaDegrees = degreesPerTic * deltaTimeInTics * turnSpeedFactor;
                    m_level->GetPlayerActor()->SetAngle(m_level->GetPlayerActor()->GetAngle() + deltaDegrees);
                }
                const float playerSpeed = 5120.0f / 65536.0f;
                const float tics = ((float)(truncatedDeltaTimeInMs) / 1000.0f) * 60.0f;
                const bool isRunning = m_configurationSettings.GetAlwaysRun() != m_playerActions.GetActionActive(Run);
                const float distance = isRunning ? playerSpeed * tics * 1.5f : playerSpeed * tics;
                if (m_playerActions.GetActionActive(MoveForward) && m_playerActions.GetActionActive(StrafeLeft))
                { 
                    Thrust(315, distance);
                }
                else if (m_playerActions.GetActionActive(MoveBackward) && m_playerActions.GetActionActive(StrafeLeft))
                {
                    Thrust(225, distance);
                }
                else if (m_playerActions.GetActionActive(MoveForward) && m_playerActions.GetActionActive(StrafeRight))
                {
                    Thrust(45, distance);
                }
                else if (m_playerActions.GetActionActive(MoveBackward) && m_playerActions.GetActionActive(StrafeRight))
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
                else if (m_playerActions.GetActionActive(StrafeLeft))
                {
                    Thrust(270, distance);
                }
                else if (m_playerActions.GetActionActive(StrafeRight))
                {
                    Thrust(90, distance);
                }
                if (m_playerInput.GetMouseXPos() != 0)
                {
                    const float mouseMovement = m_playerInput.GetMouseXPos() * (m_configurationSettings.GetMouseSensitivity() / 10.0f);
                    m_level->GetPlayerActor()->SetAngle(m_level->GetPlayerActor()->GetAngle() + (mouseMovement * tics * (abs(mouseMovement / 30.0f))));
                    m_playerInput.SetMouseXPos(0);
                }

                m_level->UpdateVisibilityMap();

                ThinkActors();
                ThinkNonBlockingActors();
            }
        }

        if (m_takingChest)
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

        // Check for player dead
        if (m_state == InGame && m_level->GetPlayerActor()->IsDead())
        {
            m_gameTimer.Pause();
        }

        // Update radar
        
        m_radarModel.ResetRadar(m_level->GetPlayerActor(), m_playerInventory, m_timeStampOfPlayerCurrentFrame);
        const uint16_t mapSize = m_level->GetLevelWidth() * m_level->GetLevelHeight();
        m_radarModel.AddActors((const Actor**)m_level->GetBlockingActors(), mapSize);
        m_radarModel.AddActors((const Actor**)m_level->GetNonBlockingActors(), 100);

        if (m_level->GetLevelIndex() != m_warpToLevel && m_state != VerifyGateExit && m_keyToTake == NoKey)
        {
            LoadLevel(m_warpToLevel);
            m_playerActions.ResetForNewLevel();
            m_state = EnteringLevel;
            m_timeStampToEnterGame = m_gameTimer.GetActualTime() + 2000u;
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
        if (m_level->GetBlockingActors()[i] != NULL)
        {
            if (!m_level->GetBlockingActors()[i]->IsActive() && m_level->IsTileVisibleForPlayer(i % m_level->GetLevelWidth(), i / m_level->GetLevelWidth()))
            {
                m_level->GetBlockingActors()[i]->SetActive(true);
            }
            if (m_level->GetBlockingActors()[i]->IsActive())
            {
                PerformActionOnActor(m_level->GetBlockingActors()[i]);

                // Another check on NULL is necessary, as the actor could have been deleted by performing ActionRemove.
                if (m_level->GetBlockingActors()[i] != NULL)
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
            actor->SetTimeToNextAction(m_timeStampOfWorldCurrentFrame + ((uint32_t)actor->GetTemp2() * 1000 / 60));
            actor->SetSolid(action == ActionStatue);
        }
        if (m_timeStampOfWorldCurrentFrame >= actor->GetTimeToNextAction())
        {
            if ((abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) > 1.1f + actor->GetDecorateActor().size) ||
                (abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) > 1.1f + actor->GetDecorateActor().size))
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
            ((abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) > 1.1f + actor->GetDecorateActor().size) ||
            (abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) > 1.1f + actor->GetDecorateActor().size)))
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
            (abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) < 2.0f + actor->GetDecorateActor().size) && (abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) < 2.0f + actor->GetDecorateActor().size))
        {
            if ((abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) > 1.1f + actor->GetDecorateActor().size) ||
                (abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) > 1.1f + actor->GetDecorateActor().size))
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
                    const uint8_t damage = (m_difficultyLevel == Easy && m_game.GetId() != 5) ? actor->GetDecorateActor().damage / 2 : actor->GetDecorateActor().damage;
                    m_level->GetPlayerActor()->Damage(damage);
                    m_game.PlaySoundPlayerHurt(m_level->GetPlayerActor()->GetHealth());
                }
            }
        }
        else
        {
            // Ranged attack with projectiles
            const int16_t angleInt = m_level->AngleNearPlayer(actor);
            if (angleInt != -1)
            {
                Actor* projectile = NULL;
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
        break;
    }
    case ActionWaitForPickup:
    case ActionWaitForPickupDestructable:
    {
        actor->SetSolid(false);
        if ((abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) < m_level->GetPlayerActor()->GetDecorateActor().size + actor->GetDecorateActor().size) && (abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) < m_level->GetPlayerActor()->GetDecorateActor().size + actor->GetDecorateActor().size))
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
            DisplayStatusMessage("Item destroyed", 80 * 17);
        }
        m_level->SpawnBigExplosion(actor->GetX(),actor->GetY(),12,(16l<<16L), m_timeStampOfWorldCurrentFrame, m_game.GetExplosionActor());
        actor->SetActionPerformed(true);
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
        break;
    }
    case ActionWarpToOtherLevel:
    {
        if (m_game.GetId() == 5)
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
            actor->SetTimeToNextAction(m_timeStampOfWorldCurrentFrame + ((uint32_t)actor->GetTemp2() * 1000 / 60));
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
                const uint8_t damage = (m_difficultyLevel == Easy && m_game.GetId() != 5) ? baseDamage / 2 : baseDamage;
                m_level->GetPlayerActor()->Damage(damage);
                m_game.PlaySoundPlayerHurt(m_level->GetPlayerActor()->GetHealth());
            }
        }
        actor->SetActionPerformed(true);
        break;
    }
    case ActionPlayerProjectile:
    case ActionMonsterProjectile:
    {
        const float speed =  actor->GetDecorateActor().speed;
        if (speed > 0)
        {
            const uint32_t deltaTime = m_timeStampOfWorldCurrentFrame - m_timeStampOfWorldPreviousFrame;
            const uint32_t truncatedDeltaTime = (deltaTime < 50) ? deltaTime : 50;
            float distance = (float)speed * ((float)(truncatedDeltaTime) / 14.2f) / 65536.0f;
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
                        if ((abs(basex - (float)x - 0.5f) < size + 0.5f) &&
                            (abs(basey - (float)y - 0.5f) < size + 0.5f))
                        {
                            m_game.PlaySoundShootWall();
                            moveOk = false;
                        }
                    }

                    if (m_level->IsExplosiveWall(x, y))
                    {
                        if ((abs(basex - (float)x - 0.5f) < size + 0.5f) &&
                            (abs(basey - (float)y - 0.5f) < size + 0.5f))
                        {
                            if (action == ActionPlayerProjectile)
                            {
                                m_level->ExplodeWall(x, y, m_timeStampOfWorldCurrentFrame, m_game.GetExplodingWallActor());
                            }
                            moveOk = false;
                        }
                    }

                    Actor* otherActor = m_level->GetBlockingActor(x, y);
                    if (otherActor != NULL)
                    {
                        if (action == ActionPlayerProjectile)
                        {
                            if (((otherActor->IsSolid() && otherActor->GetState() != StateIdRise && otherActor->GetState() != StateIdSink) || otherActor->GetAction() == ActionWaitForPickupDestructable || otherActor->GetAction() == ActionForceField || otherActor->GetAction() == ActionHangingSkeleton || otherActor->GetAction() == ActionBurningTree
                                ) && 
                                (abs(basex - otherActor->GetX()) < size + otherActor->GetDecorateActor().size) &&
                                (abs(basey - otherActor->GetY()) < size + otherActor->GetDecorateActor().size))
                            {
                                if (otherActor->GetAction() == ActionStatue || otherActor->GetAction() == ActionHangingSkeleton || otherActor->GetAction() == ActionBurningTree)
                                {
                                    m_game.PlaySoundShootWall();
                                }
                                else
                                {
                                    // In the original Catacomb Adventure series, the hit points of all monsters were divided by four when playing in easy mode, see function EasyHitPoints in C4_ACT1.C.
                                    // To prevent the Actor class from having to look up the difficulty mode, the damage inflicted by the players' fireball is simply multiplied by 4 here.
                                    const uint8_t damage = (m_difficultyLevel == Easy && m_game.GetId() != 5) ? actor->GetDecorateActor().damage * 4 : actor->GetDecorateActor().damage;
                                    otherActor->Damage(damage);
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
                                            m_level->SetBlockingActor(x, y, NULL);
                                        }
                                    }
                                    else
                                    {
                                        if (m_game.GetId() == 3 && otherActor->GetDecorateActor().id == 56 && rand() % 8 != 0)
                                        {
                                            // The red demon from Armageddon has a pain chance of 1:8, see C5_STATE.C.
                                        }
                                        else if (m_game.GetId() == 4 && otherActor->GetDecorateActor().id == 64 && rand() % 5 != 0)
                                        {
                                            // The yellow troll from Apocalypse has a pain chance of 1:5, see C6_STATE.C.
                                        }
                                        else if (m_game.GetId() == 4 && otherActor->GetDecorateActor().id == 57 && rand() % 8 != 0)
                                        {
                                            // The cyber demon from Apocalypse has a pain chance of 1:8, see C6_STATE.C.
                                        }
                                        else if (m_game.GetId() == 4 && otherActor->GetDecorateActor().id == 62 && rand() % 8 != 0)
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
                        const uint8_t damage = (m_difficultyLevel == Easy && m_game.GetId() != 5 && baseDamage > 1) ? baseDamage / 2 : baseDamage;
                        m_level->GetPlayerActor()->Damage(damage);
                        m_game.PlaySoundPlayerHurt(m_level->GetPlayerActor()->GetHealth());
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
            actor->SetTimeToNextAction(m_timeStampOfWorldCurrentFrame + ((uint32_t)actor->GetTemp2() * 1000 / 60));
            actor->SetSolid(false);
        }
        if (m_timeStampOfWorldCurrentFrame >= actor->GetTimeToNextAction())
        {
            // Time to transform from harmless bunny to evil bunny!
            // Check that the player is not on top of the bunny.
            if ((abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) > 1.1f + actor->GetDecorateActor().size) ||
                (abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) > 1.1f + actor->GetDecorateActor().size))
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
            ((abs(m_level->GetPlayerActor()->GetX() - actor->GetX()) < 0.9f + actor->GetDecorateActor().size) &&
            (abs(m_level->GetPlayerActor()->GetY() - actor->GetY()) < 0.9f + actor->GetDecorateActor().size));
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
    for ( uint16_t i = 0; i < 100; i++)
    {
        if (m_level->GetNonBlockingActors()[i] != NULL)
        {
            PerformActionOnActor(m_level->GetNonBlockingActors()[i]);

            // Another check on NULL is necessary, as the actor could have been deleted by performing ActionRemove.
            if (m_level->GetNonBlockingActors()[i] != NULL)
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
void EngineCore::ClipXMove (const float xmove)
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
}

// Based on ClipYMove() in C4_WIZ.C of the Catacomb Abyss source code.
void EngineCore::ClipYMove (const float ymove)
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
}

bool EngineCore::ClipWithTile(const uint16_t tileX, const uint16_t tileY, const float playerX, const float playerY)
{
    bool moveOk = true;
    const float playerSize = m_level->GetPlayerActor()->GetDecorateActor().size;
    const bool playerTouchesTile = ((abs(playerX - (float)tileX - 0.5f) < playerSize + 0.5f) &&
        (abs(playerY - (float)tileY - 0.5f) < playerSize + 0.5f));
    if (playerTouchesTile && m_level->IsSolidWall(tileX, tileY))
    {
        moveOk = false;
    }

    if (playerTouchesTile && m_level->IsDoor(tileX, tileY))
    {
        moveOk = false;

        const KeyId requiredKey = m_level->GetRequiredKeyForDoor(tileX, tileY);
        if (requiredKey != NoKey && m_playerInventory.GetKeys(requiredKey) == 0)
        {
            m_gameTimer.Pause();
            _sprintf_p(m_messageInPopup, 256, "You need a %s key", GetKeyName(requiredKey));
            WaitForAnyKeyPressed();
            m_game.PlaySoundHitGate();
        }
        else
        {
            if (requiredKey != NoKey)
            {
                m_keyToTake = requiredKey;
            }
            if (m_level->IsBlockedDoor(tileX, tileY))
            {
                m_gameTimer.Pause();
                _sprintf_p(m_messageInPopup, 256, "The door is blocked");
                WaitForAnyKeyPressed();
                m_game.PlaySoundHitGate();
            }
            else if (m_level->IsRemovableDoor(tileX, tileY) ||
                     m_game.GetId() == 5) // All doors in the Catacomb 3-D can be removed
            {
                // Open the door
                m_level->SetWallTile(tileX, tileY, 0);
                m_level->SetFloorTile(tileX, tileY, 0);

                // Open connected door tiles which require the same key in all four directions
                if (requiredKey != NoKey)
                {
                    uint16_t x = tileX - 1;
                    while (x > 1 && m_level->GetRequiredKeyForDoor(x, tileY) == requiredKey)
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
                    while (y > 1 && m_level->GetRequiredKeyForDoor(tileX, y) == requiredKey)
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
    if (actor != NULL)
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
    if (m_state != InGame || m_readingScroll != 255)
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
    xmove = -distance * (float)sin((m_level->GetPlayerActor()->GetAngle() + 180 + angle) * 3.14159265 / 180.0);
    ymove = distance * (float)cos((m_level->GetPlayerActor()->GetAngle() + 180 + angle) * 3.14159265 / 180.0);

    ClipXMove(xmove);
    if (_strcmpi(m_messageInPopup, "") == 0)
    {
        ClipYMove(ymove);
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
        (((m_game.GetId() == 1 || m_game.GetId() == 2 || m_game.GetId() == 3) && actor->GetDecorateActor().id == 61)) ||
        ((m_game.GetId() == 4 && actor->GetDecorateActor().id == 54)))
    {
        speed = 1200;
    }

    // The manta ray in Apocalypse move slower when under water.
    if (actor->GetState() == StateIdHidden &&
        ((m_game.GetId() == 4 && actor->GetDecorateActor().id == 65)))
    {
        speed = 1700;
    }

    const uint32_t deltaTimeInMs = m_timeStampOfWorldCurrentFrame - m_timeStampOfWorldPreviousFrame;
    const uint32_t truncatedDeltaTimeInMs = (deltaTimeInMs < 50) ? deltaTimeInMs : 50;
    float move = (float)speed * ((float)(truncatedDeltaTimeInMs) / 14.2f) / 65536.0f;

    while (move > 0.0f)
    {
        if (actor->GetState() == StateIdWalk || actor->GetState() == StateIdPain)
        {
            // Check if actor can attack
            if (actor->GetDecorateActor().damage > 0)
            {
                // Melee attack
                const bool performRandomAttack = (rand() % (5000 / truncatedDeltaTimeInMs) == 0);
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
        
        m_level->GetBlockingActors()[(actor->GetTileY() * m_level->GetLevelWidth()) + actor->GetTileX()] = NULL;	// pick up marker from goal
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
    float move = (float)speed * ((float)(truncatedDeltaTimeInMs) / 14.2f) / 65536.0f;

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

        m_level->GetBlockingActors()[(actor->GetTileY() * m_level->GetLevelWidth()) + actor->GetTileX()] = NULL;	// pick up marker from goal
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
    float move = (float)speed * ((float)(truncatedDeltaTimeInMs) / 14.2f) / 65536.0f;

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

        m_level->GetBlockingActors()[(actor->GetTileY() * m_level->GetLevelWidth()) + actor->GetTileX()] = NULL;	// pick up marker from goal

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
    float move = (float)speed * ((float)(truncatedDeltaTimeInMs) / 14.2f) / 65536.0f;

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

        m_level->GetBlockingActors()[(actor->GetTileY() * m_level->GetLevelWidth()) + actor->GetTileX()] = NULL;	// pick up marker from goal
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
    float move = harmlessBunnySpeed * ((float)(truncatedDeltaTimeInMs) / 14.2f) / 65536.0f;

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
            Actor* actor = m_level->GetBlockingActor(x, y);
            if (actor != NULL && actor != sourceWarp && actor->GetDecorateActor().spawnOnAllDifficulties == sourceWarp->GetDecorateActor().spawnOnAllDifficulties)
            {
                m_level->GetPlayerActor()->SetX(actor->GetX());
                m_level->GetPlayerActor()->SetY(actor->GetY());
                Thrust(0, 1.5f);
                if (abs(actor->GetX() - m_level->GetPlayerActor()->GetX()) < 0.1f && abs(actor->GetY() - m_level->GetPlayerActor()->GetY()) < 0.1f )
                {
                    Thrust(0, 1.0f);
                    if (abs(actor->GetX() - m_level->GetPlayerActor()->GetX()) < 0.1f && abs(actor->GetY() - m_level->GetPlayerActor()->GetY()) < 0.1f )
                    {
                        Thrust(0, 0.5f);
                    }
                }
            }
        }
    }
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
    if (!m_level->GetPlayerActor()->IsDead())
    {
        if (m_playerInventory.UsePotion())
        {
            m_level->GetPlayerActor()->SetHealth(100);
        }
    }
}

void EngineCore::ShowGodModeCheatDialog()
{
    if (m_state == InGame && m_level != NULL && m_level->GetPlayerActor()->GetHealth() > 0)
    {
        m_state = GodModeCheatDialog;
        m_godModeIsOn = !m_godModeIsOn;
        m_gameTimer.Pause();
    }
}

void EngineCore::ShowWarpCheatDialog()
{
    if (m_state == InGame && m_level != NULL && m_level->GetPlayerActor()->GetHealth() > 0)
    {
        m_state = WarpCheatDialog;
        m_gameTimer.Pause();
    }
}

void EngineCore::ShowFreeItemsCheatDialog()
{
    if (m_state == InGame && m_level != NULL && m_level->GetPlayerActor()->GetHealth() > 0)
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
    if (m_state == InGame)
    {
        m_gameTimer.Pause();
    }
    m_game.PlaySoundWarpUpOrDown(true);
}

void EngineCore::CloseMenu()
{
    m_menu->SetActive(false);
    if (m_state == InGame)
    {
        m_gameTimer.Resume();
    }
    m_game.PlaySoundWarpUpOrDown(false);
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
}

void EngineCore::StartNewGame()
{
    m_state = EnteringLevel;
    m_playerInventory.ResetForNewGame();
    UnloadLevel();
    m_timeStampToEnterGame = m_gameTimer.GetActualTime() + 2000u;
    m_warpToLevel = 0;
}

void EngineCore::UnloadLevel()
{
    if (m_level != NULL)
    {
        delete m_level;
        m_level = NULL;
    }
}

bool EngineCore::StoreGameToFileWithFullPath(const std::string filename) const
{
    bool result = false;

    if (m_level == NULL)
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
        const uint8_t gameId = m_game.GetId();
        file.write((const char*)&gameId, sizeof(gameId));
        file.write((const char*)&m_difficultyLevel, sizeof(m_difficultyLevel));
        file.write((const char*)&m_godModeIsOn, sizeof(m_godModeIsOn));
        m_playerInventory.StoreToFile(file);
        m_level->StoreToFile(file);
        m_gameTimer.StoreToFile(file);
        file.close();
        result = true;
    }
    else
    {
        Logging::Instance().AddLogMessage("WARNING: Unable to write to file " + filename);
    }
    return result;
}

bool EngineCore::StoreGameToFile(const std::string filename)
{
    const std::string filenamePath = m_system.GetConfigurationFilePath();
    const std::string filenamePathForGame = filenamePath + m_game.GetSavedGamesPath();
    if (m_system.CreatePath(filenamePathForGame))
    {
        const std::string fullPath = filenamePathForGame + "\\" + filename + ".sav";
        return StoreGameToFileWithFullPath(fullPath);
    }
    else
    {
        Logging::Instance().AddLogMessage("WARNING: Unable to create path " + filenamePathForGame);
    }

    return false;
}

void EngineCore::LoadGameFromFileWithFullPath(const std::string filename)
{
    std::ifstream file;
    file.open(filename, std::ifstream::binary);
    if (file.is_open())
    {
        Logging::Instance().AddLogMessage("Loading saved game " + filename);
        char headerString[11];
        file.read(headerString, 11);
        if (file.fail())
        {
            Logging::Instance().AddLogMessage("WARNING: Unable to read header from " + filename);
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
        m_gameTimer.LoadFromFile(file);
        file.close();

        m_playerActions.ResetForNewLevel();
        m_warpToLevel = m_level->GetLevelIndex();
        m_menu->SetActive(false);
        m_state = InGame;

        const uint32_t currentTimestampOfPlayer = m_gameTimer.GetMillisecondsForPlayer();
        const uint32_t currentTimestampOfWorld = m_gameTimer.GetMilliSecondsForWorld();

        m_timeStampOfPlayerPreviousFrame = m_timeStampOfPlayerCurrentFrame;
        m_timeStampOfPlayerCurrentFrame = currentTimestampOfPlayer;
        m_timeStampOfWorldPreviousFrame = m_timeStampOfWorldCurrentFrame;
        m_timeStampOfWorldCurrentFrame = currentTimestampOfWorld;
    }
    else
    {
        Logging::Instance().AddLogMessage("WARNING: Unable to open file " + filename);
    }
}

void EngineCore::LoadGameFromFile(const std::string filename)
{
    const std::string filenamePath = m_system.GetConfigurationFilePath();
    const std::string filenamePathAbyss = filenamePath + m_game.GetSavedGamesPath();
    const std::string fullPath = filenamePathAbyss + "\\" + filename + ".sav";
    LoadGameFromFileWithFullPath(fullPath);
}

ScreenMode EngineCore::GetScreenMode() const
{
    return m_configurationSettings.GetScreenMode();
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