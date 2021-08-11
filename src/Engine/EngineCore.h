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

//
// EngineCore
//
// Main class for the engine. Instantiates and orchestrates all the other engine classes.
//
#pragma once

#include "IGame.h"
#include "Shape.h"
#include "Radar.h"
#include "PlayerInventory.h"
#include "PlayerActions.h"
#include "FramesCounter.h"
#include "ControlsMap.h"
#include "PlayerInput.h"
#include "ConfigurationSettings.h"
#include "ExtraMenu.h"
#include "GameTimer.h"
#include "ISystem.h"
#include "Logging.h"
#include "Score.h"
#include "FadeEffect.h"
#include "AutoMap.h"
#include "ManaBar.h"
#include "RenderableOverscanBorder.h"
#include "RenderableLevelStatistics.h"

class EngineCore
{
public:
    EngineCore(IGame& game, const ISystem& system, PlayerInput& playerInput, ConfigurationSettings& configurationSettings);
    ~EngineCore();

    // Draw the current state of the game.
    void DrawScene(IRenderer& renderer);

    // Update the game state based on time and user input.
    bool Think();

    // Returns true when the mouse is actively used by the game and needs to be bound to the application window.
    bool RequiresMouseCapture() const;

    // CatacombGL version info; to be displayed at the top of the application window.
    static const std::string GetVersionInfo();

    // Get the screen mode from the current configuration
    uint8_t GetScreenMode() const;

private:
    enum State
    {
        Introduction,
        RequestDifficultyLevel,
        ShowDifficultyLevel,
        StandBeforeGate,
        EnteringLevel,
        InGame,
        WarpCheatDialog,
        GodModeCheatDialog,
        FreeItemsCheatDialog,
        Victory,
        VerifyGateExit,
        ExitGame,
        Help,
        AutoMapDialog
    };

    void LoadLevel(const uint8_t mapIndex);
    void ThinkActors();
    void ThinkNonBlockingActors();

    void ClipXMove (const float xmove);
    void ClipYMove (const float ymove);
    bool ClipWithTile(const uint16_t tileX, const uint16_t tileY, const float playerX, const float playerY);

    void DrawCenteredTiledWindow(IRenderer& renderer, const uint16_t width, const uint16_t height);
    void DrawTiledWindow(IRenderer& renderer, const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height);

    bool Chase(Actor* actor, const bool diagonal, const ChaseTarget target);
    void RunAway(Actor* actor);
    void ChaseLikeRunningEye(Actor* actor);
    void BunnyHopping(Actor* actor);
    void Bounce(Actor* actor);

    const char* GetKeyName(const KeyId keyId) const;

    void WarpInsideLevel(const Actor* sourceWarp);
    void DisplayStatusMessage(const char* message, const uint16_t delayInMS);

    void WaitForAnyKeyPressed();
    void PerformActionOnActor(Actor* actor);
    static bool IsOneTimeAction(const actorAction action);
    void Thrust (const uint16_t angle, const float distance);

    void ShowWarpCheatDialog();
    void ShowGodModeCheatDialog();
    void ShowFreeItemsCheatDialog();
    void ShowAutoMap(const bool cheat);
    void FreezeTimeCheat();

    void SelectDifficultyLevelNovice();
    void SelectDifficultyLevelWarrior();

    void OpenMenu();
    void CloseMenu();

    void EnterKeyReleased();
    void ReadScroll(const uint8_t scrollIndex);
    void KeyWPressed();
    void KeyYPressed();
    void KeyNPressed();
    void PlayerUsesPotion();
    void TextFieldInput(const char c);
    void BackspacePressed();
    bool IsActionActive(const ControlAction action) const;
    bool IsActionJustPressed(const ControlAction action) const;
    void StartNewGameWithDifficultySelection();
    void StartNewGame();
    void UnloadLevel();
    bool StoreGameToFileWithFullPath(const std::string filename) const;
    bool StoreGameToFile(const std::string filename);
    void LoadGameFromFileWithFullPath(const std::string filename);
    void LoadGameFromFile(const std::string filename);
    bool AreScrollsPresent() const;
    void StartMusicIfNeeded();

    IGame& m_game;
    ConfigurationSettings& m_configurationSettings;

    // Timer
    uint32_t m_timeStampOfPlayerCurrentFrame;
    uint32_t m_timeStampOfPlayerPreviousFrame;
    uint32_t m_timeStampOfWorldCurrentFrame;
    uint32_t m_timeStampOfWorldPreviousFrame;

    // Data to store when the game is saved
    Level* m_level;
    PlayerInventory m_playerInventory;
    DifficultyLevel m_difficultyLevel;
    bool m_godModeIsOn;

    // Volatile data
    FramesCounter m_framesCounter;
    PlayerInput& m_playerInput;
    uint8_t m_keyToTake;
    uint8_t m_victoryState;
    char m_messageInPopup[256];
    State m_state;
    const char* m_statusMessage;
    Radar m_radarModel;
    uint8_t m_readingScroll;
    bool m_takingChest;
    uint8_t m_warpToLevel;
    std::string m_warpCheatTextField;
    PlayerActions m_playerActions;
    int32_t m_lastFreezeTimeTick;
    uint32_t m_timeStampEndOfStatusMessage;
    uint32_t m_startTakeKey;
    uint32_t m_timeStampToEnterGame;
    uint32_t m_timeStampLastMouseMoveForward;
    uint32_t m_timeStampLastMouseMoveBackward;
    uint32_t m_timeStampFadeEffect;
    IMenu* m_menu;
    GameTimer m_gameTimer;
    const ISystem& m_system;
    std::vector<std::string> m_savedGames;
    const bool m_scrollsArePresent;
    Score m_score;
    FadeEffect m_fadeEffect;
    bool m_setOverlayOnNextDraw;
    AutoMap m_autoMap;
    Renderable3DScene m_renderable3DScene;
    RenderableAutoMapIso m_renderableAutoMapIso;
    RenderableAutoMapTopDown m_renderableAutoMapTopDown;
    ManaBar m_manaBar;
    OverscanBorder m_overscanBorder;
    RenderableOverscanBorder m_renderableOverscanBorder;
    bool m_insideBorderFlashLocation;
    LevelStatistics m_levelStatistics;
    RenderableLevelStatistics m_renderableLevelStatistics;
};
