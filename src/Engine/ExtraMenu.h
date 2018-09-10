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
// ExtraMenu
//
// Offers menu functionality that is not present in the original game, but required
// for setting the various new graphics and controls options.
//
#pragma once

#include <stdint.h>
#include "PlayerInput.h"
#include "AudioPlayer.h"
#include "ConfigurationSettings.h"
#include "IRenderer.h"
#include "EgaGraph.h"

enum MenuCommand
{
    MenuCommandNone,
    MenuCommandStartNewGame,
    MenuCommandExitGame,
    MenuCommandSaveGame,
    MenuCommandLoadGame
};

class ExtraMenu
{
public:
    ExtraMenu(ConfigurationSettings& configurationSettings, AudioPlayer& audioPlayer, std::vector<std::string>& savedGames);
    bool IsActive() const;
    void SetActive(bool active);
    MenuCommand ProcessInput(const PlayerInput& playerInput);
    void Draw(IRenderer& renderer, EgaGraph* const egaGraph);
    void SetSaveGameEnabled(const bool enabled);
    const std::string& GetNewSaveGameName() const;
    void OpenRestoreGameMenu();
    void OpenSaveGameMenu();
    void OpenSoundMenu();

private:
    void MenuDown();
    void MenuUp();
    void MenuLeft();
    void MenuRight();
    MenuCommand EnterKeyPressed();
    static bool KeyIsSuitableForSaveGameName(const SDL_Keycode keyCode);

    bool m_menuActive;
    uint8_t m_menuItemSelected;
    uint8_t m_subMenuSelected;
    uint8_t m_menuItemOffset;
	bool m_waitingForKeyToBind;
    bool m_waitingForNewSaveGameName;
    bool m_saveGameEnabled;

    ConfigurationSettings& m_configurationSettings;
    AudioPlayer& m_audioPlayer;
    std::vector<std::string>& m_savedGames;
    std::string m_newSaveGameName;
    bool m_askForOverwrite;
};