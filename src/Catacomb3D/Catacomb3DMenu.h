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

//
// Catacomb3DMenu
//
// The menu for Catacomb 3D
//
#pragma once

#include <stdint.h>
#include "../Engine/PlayerInput.h"
#include "../Engine/AudioPlayer.h"
#include "../Engine/ConfigurationSettings.h"
#include "../Engine/IRenderer.h"
#include "EgaGraphCatacomb3D.h"
#include "../Engine/IMenu.h"
#include "../Engine/HighScores.h"
#include "SkullNBones.h"
#include "../Engine/GuiMenu.h"
#include "GuiElementButtonCat3D.h"
#include "../Engine/SavedGamesInDosFormat.h"

class Catacomb3DMenu: public IMenu
{
public:
    Catacomb3DMenu(
        ConfigurationSettings& configurationSettings,
        AudioPlayer& audioPlayer,
        PlayerInput& playerInput,
        EgaGraph* const egaGraph,
        std::vector<std::string>& savedGames,
        HighScores& highScores,
        SavedGamesInDosFormat& savedGamesInDosFormat);
    bool IsActive() const override;
    void SetActive(bool active) override;
    MenuCommand ProcessInput(const PlayerInput& playerInput) override;
    void Draw(IRenderer& renderer, EgaGraph* const egaGraph, const uint16_t menuCursorPic, const uint32_t timeStamp) override;
    void SetSaveGameEnabled(const bool enabled) override;
    const std::string& GetNewSaveGameName() const override;
    void AddNewSavedGame(const PlayerInput& playerInput, const std::string& name) override;
    void OpenRestoreGameMenu() override;
    void OpenSaveGameMenu() override;
    void OpenSoundMenu() override;
    void OpenDemo() override;
    void CheckHighScore(const uint16_t level, const uint32_t score) override;
    void ShowSavingPopup(const std::string& name, const uint32_t timeStamp) override;

private:
    bool IsNewSaveGameNameAlreadyInUse() const;
    static void DrawConfirmationDialog(IRenderer& renderer, EgaGraph& egaGraph, const uint16_t width, const std::string& message1, const std::string& message2, const std::string& message3);
    void DrawCenteredTiledWindow(IRenderer& renderer, EgaGraph* const egaGraph, const uint16_t width, const uint16_t height);
    void DrawTiledWindow(IRenderer& renderer, EgaGraph* const egaGraph, const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height);
    static bool RepliedWithYes(const SDL_Keycode keyCode);
    static bool RepliedWithNo(const SDL_Keycode keyCode);

    bool m_menuActive;
    bool m_saveGameEnabled;

    ConfigurationSettings& m_configurationSettings;
    AudioPlayer& m_audioPlayer;
    std::vector<std::string>& m_savedGames;
    std::string m_newSaveGameName;
    bool m_askForOverwrite;
    bool m_askForEndGame;
    bool m_askForQuit;
    bool m_askForReset;
    bool m_askForResetClassic;
    HighScores& m_highScores;
    uint32_t m_menuActivatedTimestamp;
    uint32_t m_savingPopupTimestamp;
    std::string m_savingPopupName;

    GuiMenu m_guiMenu;
    uint8_t m_askForEndGameGuiAction;
    RenderableText m_renderableText;
    RenderableText m_renderableTextDefaultFont;
    RenderableTiles m_renderableTiles;
    bool m_flashIcon;
    uint32_t m_timeStamp;
    GuiElementButtonCat3D* m_returnToGameButton;
    SavedGamesInDosFormat& m_savedGamesInDosFormat;
};
