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

#include "Catacomb3DMenu.h"
#include "AudioRepositoryCatacomb3D.h"
#include "..\..\ThirdParty\SDL\include\SDL_keyboard.h"
#include "..\Engine\DefaultFont.h"
#include "..\Engine\IRenderer.h"

const uint8_t subMenuMain = 0;
const uint8_t subMenuVideo = 1;
const uint8_t subMenuControls = 2;
const uint8_t subMenuSound = 3;
const uint8_t subMenuRestoreGame = 4;
const uint8_t subMenuSaveGame = 5;
const uint8_t subMenuNewGame = 6;
const uint8_t subMenuConfigure = 7;
const uint8_t subMenuMusic = 8;
const uint8_t subMenuSkullNBones = 9;
const uint8_t subMenuHighScores = 10;

Catacomb3DMenu::Catacomb3DMenu(ConfigurationSettings& configurationSettings, AudioPlayer& audioPlayer, std::vector<std::string>& savedGames, HighScores& highScores) :
    m_menuActive (false),
    m_menuItemSelected (0),
    m_subMenuSelected (0),
    m_menuItemOffset (0),
	m_waitingForKeyToBind (false),
    m_saveGameEnabled (false),
    m_configurationSettings (configurationSettings),
    m_audioPlayer (audioPlayer),
    m_savedGames (savedGames),
    m_newSaveGameName (""),
    m_waitingForNewSaveGameName (false),
    m_askForOverwrite (false),
    m_askForEndGame (false),
    m_askForQuit (false),
    m_highScores(highScores),
    m_skullNBones(audioPlayer),
    m_menuActivatedTimestamp(0u)
{

}

bool Catacomb3DMenu::IsActive() const
{
    return m_menuActive;
}

void Catacomb3DMenu::SetActive(bool active)
{
    m_menuActive = active;
    if (!active)
    {
        m_waitingForKeyToBind = false;
        m_waitingForNewSaveGameName = false;
        m_subMenuSelected = subMenuMain;
        m_menuItemSelected = 0;
        m_menuItemOffset = 0;
    }
}

MenuCommand Catacomb3DMenu::ProcessInput(const PlayerInput& playerInput)
{
    MenuCommand command = MenuCommandNone;
    if (m_askForOverwrite)
    {
        const SDL_Keycode keyCode = playerInput.GetFirstKeyPressed();
        if (keyCode == SDLK_y)
        {
            m_askForOverwrite = false;
            command = MenuCommandSaveGame;
        }
        else if (keyCode != SDLK_UNKNOWN)
        {
            m_askForOverwrite = false;
        }
    }
    if (m_waitingForKeyToBind)
    {
        // Check which key is pressed
        const SDL_Keycode keyCode = playerInput.GetFirstKeyPressed();
        if (keyCode != SDLK_UNKNOWN && m_configurationSettings.GetControlsMap().AssignActionToKey((ControlAction)(m_menuItemSelected + 1), keyCode))
        {
            m_waitingForKeyToBind = false;
        }
        else
        {
            const uint8_t buttonCode = playerInput.GetFirstMouseButtonPressed();
            if (buttonCode != 0 && m_configurationSettings.GetControlsMap().AssignActionToMouseButton((ControlAction)(m_menuItemSelected + 1), buttonCode))
            {
                m_waitingForKeyToBind = false;
            }
        }
    }
    else if (m_waitingForNewSaveGameName)
    {
        const uint16_t maxSaveGameNameLength = 20;
        // Check which key is pressed
        const SDL_Keycode keyCode = playerInput.GetFirstKeyPressed();
        if (KeyIsSuitableForSaveGameName(keyCode) && m_newSaveGameName.length() < maxSaveGameNameLength)
        {
            m_newSaveGameName += std::string(SDL_GetKeyName(keyCode));
        }
        else if (keyCode == SDLK_BACKSPACE && !m_newSaveGameName.empty())
        {
            m_newSaveGameName.pop_back();
        }
    }
    else if (m_askForQuit)
    {
        const SDL_Keycode keyCode = playerInput.GetFirstKeyPressed();
        if (keyCode == SDLK_y)
        {
            m_askForQuit = false;
            return MenuCommandExitGame;
        }
        else if (keyCode != SDLK_UNKNOWN)
        {
            m_askForQuit = false;
            return MenuCommandNone;
        }
    }
    else if (m_askForEndGame)
    {
        const SDL_Keycode keyCode = playerInput.GetFirstKeyPressed();
        if (keyCode == SDLK_y)
        {
            m_askForEndGame = false;
            if (m_subMenuSelected == subMenuNewGame)
            {
                command =
                    (m_menuItemSelected == 0) ? MenuCommandStartNewGameEasy :
                    (m_menuItemSelected == 1) ? MenuCommandStartNewGameNormal :
                    MenuCommandStartNewGameHard;
                m_subMenuSelected = subMenuMain;
                m_menuItemSelected = 0;
                return command;
            }
            else if (m_subMenuSelected == subMenuRestoreGame)
            {
                m_subMenuSelected = subMenuMain;
                m_menuItemSelected = 0;
                return MenuCommandLoadGame;
            }
            else
            {
                return MenuCommandEndGame;
            }
        }
        else if (keyCode != SDLK_UNKNOWN)
        {
            m_askForEndGame = false;
            return MenuCommandNone;
        }
    }
    else if (m_subMenuSelected == subMenuSkullNBones && !playerInput.IsKeyJustPressed(SDLK_ESCAPE))
    {
        if (m_skullNBones.ProcessInput(playerInput))
        {
            // Game over
            m_subMenuSelected = subMenuMain;
            m_menuItemSelected = 0;
            m_menuItemOffset = 0;
        }
        return MenuCommandNone;
    }
    else if (m_subMenuSelected == subMenuHighScores)
    {
        // Check if a name for a new high score is entered
        const SDL_Keycode keyCode = playerInput.GetFirstKeyPressed();
        if (KeyIsSuitableForSaveGameName(keyCode))
        {
            m_highScores.AddCharactersToNameOfNewScore(std::string(SDL_GetKeyName(keyCode)));
        }
        else if (keyCode == SDLK_BACKSPACE)
        {
            m_highScores.RemoveACharacterFromNameOfNewScore();
        }
    }
    else if (playerInput.IsKeyJustPressed(SDLK_UP))
    {
        MenuUp();
    }
    else if (playerInput.IsKeyJustPressed(SDLK_DOWN))
    {
        MenuDown();
    }
    else if (playerInput.IsKeyJustPressed(SDLK_LEFT))
    {
        MenuLeft();
    }
    else if (playerInput.IsKeyJustPressed(SDLK_RIGHT))
    {
        MenuRight();
    }
    
    if (playerInput.IsKeyJustPressed(SDLK_ESCAPE))
    {
        m_menuItemSelected = 0;
        m_menuItemOffset = 0;

        if (m_subMenuSelected == subMenuMain)
        {
            command = MenuCommandCloseMenu;
        }
        else if (m_subMenuSelected == subMenuSound ||
                 m_subMenuSelected == subMenuMusic || 
                 m_subMenuSelected == subMenuVideo || 
                 m_subMenuSelected == subMenuControls)
        {
            m_subMenuSelected = subMenuConfigure;
        }
        else
        {
            m_subMenuSelected = subMenuMain;
        }
    }

    if (playerInput.IsKeyJustPressed(SDLK_RETURN))
    {
        command = EnterKeyPressed();
    }

    return command;
}

void Catacomb3DMenu::MenuDown()
{
    if (m_menuActive)
    {
        if (m_subMenuSelected == subMenuMain)
        {
            if (m_menuItemSelected == 7)
            {
                m_menuItemSelected = 0;
            }
            else
            {
                m_menuItemSelected++;
            }
        }
        else if (m_subMenuSelected == subMenuNewGame)
        {
            if (m_menuItemSelected == 2)
            {
                m_menuItemSelected = 0;
            }
            else
            {
                m_menuItemSelected++;
            }
        }
        else if (m_subMenuSelected == subMenuConfigure)
        {
            if (m_menuItemSelected == 3)
            {
                m_menuItemSelected = 0;
            }
            else
            {
                m_menuItemSelected++;
            }
        }
        else if (m_subMenuSelected == subMenuVideo)
        {
            if (m_menuItemSelected == 7)
            {
                m_menuItemSelected = 0;
            }
            else
            {
                m_menuItemSelected++;
            }
        }
        else if (m_subMenuSelected == subMenuControls)
        {
            if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 3)
            {
                m_menuItemSelected = 0;
                m_menuItemOffset = 0;
            }
            else
            {
                m_menuItemSelected++;
                if (m_menuItemSelected - m_menuItemOffset > 7)
                {
                    m_menuItemOffset = (m_menuItemSelected > 7) ? m_menuItemSelected - 7 : 0;
                }
            }
        }
        else if (m_subMenuSelected == subMenuSound)
        {
            if (m_menuItemSelected < 2)
            {
                m_menuItemSelected++;
            }
            else
            {
                m_menuItemSelected = 0;
            }
        }
        else if (m_subMenuSelected == subMenuMusic)
        {
            if (m_menuItemSelected == 1)
            {
                m_menuItemSelected = 0;
            }
            else
            {
                m_menuItemSelected = 1;
            }
        }
        else if (m_subMenuSelected == subMenuRestoreGame)
        {
            if (m_menuItemSelected == m_savedGames.size() - 1)
            {
                m_menuItemSelected = 0;
                m_menuItemOffset = 0;
            }
            else
            {
                m_menuItemSelected++;
                if (m_menuItemSelected - m_menuItemOffset > 5)
                {
                    m_menuItemOffset = (m_menuItemSelected > 5) ? m_menuItemSelected - 5 : 0;
                }
            }
        }
        else if (m_subMenuSelected == subMenuSaveGame)
        {
            if (!m_askForOverwrite && !m_waitingForNewSaveGameName)  // Do not change the save game slot while typing
            {
                if (m_menuItemSelected == m_savedGames.size())
                {
                    m_menuItemSelected = 0;
                    m_menuItemOffset = 0;
                }
                else
                {
                    m_menuItemSelected++;
                    if (m_menuItemSelected - m_menuItemOffset > 5)
                    {
                        m_menuItemOffset = (m_menuItemSelected > 5) ? m_menuItemSelected - 5 : 0;
                    }
                }
            }
        }
    }
}

void Catacomb3DMenu::MenuUp()
{
    if (m_menuActive)
    {
        if (m_subMenuSelected == subMenuMain)
        {
            if (m_menuItemSelected == 0)
            {
                m_menuItemSelected = 7;
            }
            else
            {
                m_menuItemSelected--;
            }
        }
        else if (m_subMenuSelected == subMenuNewGame)
        {
            if (m_menuItemSelected == 0)
            {
                m_menuItemSelected = 2;
            }
            else
            {
                m_menuItemSelected--;
            }
        }
        else if (m_subMenuSelected == subMenuConfigure)
        {
            if (m_menuItemSelected == 0)
            {
                m_menuItemSelected = 3;
            }
            else
            {
                m_menuItemSelected--;
            }
        }
        else if (m_subMenuSelected == subMenuVideo)
        {
            if (m_menuItemSelected == 0)
            {
                m_menuItemSelected = 7;
            }
            else
            {
                m_menuItemSelected--;
            }
        }
        else if (m_subMenuSelected == subMenuControls)
        {
            if (m_menuItemSelected == 0)
            {
                m_menuItemSelected = (uint8_t)m_configurationSettings.GetControlsMap().GetActionLabels().size() + 3;
                m_menuItemOffset = (m_menuItemSelected > 7) ? m_menuItemSelected - 7 : 0;
            }
            else
            {
                m_menuItemSelected--;
                if (m_menuItemSelected < m_menuItemOffset)
                {
                    m_menuItemOffset = m_menuItemSelected;
                }
            }
        }
        else if (m_subMenuSelected == subMenuSound)
        {
            if (m_menuItemSelected > 0)
            {
                m_menuItemSelected--;
            }
            else
            {
                m_menuItemSelected = 2;
            }
        }
        else if (m_subMenuSelected == subMenuMusic)
        {
            if (m_menuItemSelected == 1)
            {
                m_menuItemSelected = 0;
            }
            else
            {
                m_menuItemSelected = 1;
            }
        }
        else if (m_subMenuSelected == subMenuRestoreGame)
        {
            if (m_menuItemSelected == 0)
            {
                m_menuItemSelected = (uint8_t)m_savedGames.size() - 1;
                m_menuItemOffset = (m_menuItemSelected > 5) ? m_menuItemSelected - 5 : 0;
            }
            else
            {
                m_menuItemSelected--;
                if (m_menuItemSelected < m_menuItemOffset)
                {
                    m_menuItemOffset = m_menuItemSelected;
                }
            }
        }
        else if (m_subMenuSelected == subMenuSaveGame)
        {
            if (!m_askForOverwrite && !m_waitingForNewSaveGameName)  // Do not change the save game slot while typing
            {
                if (m_menuItemSelected == 0)
                {
                    m_menuItemSelected = (uint8_t)m_savedGames.size();
                    m_menuItemOffset = (m_menuItemSelected > 5) ? m_menuItemSelected - 5 : 0;
                }
                else
                {
                    m_menuItemSelected--;
                    if (m_menuItemSelected < m_menuItemOffset)
                    {
                        m_menuItemOffset = m_menuItemSelected;
                    }
                }
            }
        }
    }
}

void Catacomb3DMenu::MenuLeft()
{
    if (m_menuActive)
    {
        if (m_subMenuSelected == subMenuVideo)
        {
            if (m_menuItemSelected == 2)
            {
                if (m_configurationSettings.GetFov() > 25)
                {
                    m_configurationSettings.SetFov(m_configurationSettings.GetFov() - 1);
                }
                else
                {
                    m_configurationSettings.SetFov(45);
                }
            }
        }
        else if (m_subMenuSelected == subMenuControls)
        {
            if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size())
            {
                if (m_configurationSettings.GetMouseSensitivity() > 1)
                {
                    m_configurationSettings.SetMouseSensitivity(m_configurationSettings.GetMouseSensitivity() - 1);
                }
            }
            else if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 1)
            {
                if (m_configurationSettings.GetTurnSpeed() > 100)
                {
                    m_configurationSettings.SetTurnSpeed(m_configurationSettings.GetTurnSpeed() - 10);
                }
            }
        }
    }
}

void Catacomb3DMenu::MenuRight()
{
    if (m_menuActive)
    {
        if (m_subMenuSelected == subMenuVideo)
        {
            if (m_menuItemSelected == 2)
            {
                if (m_configurationSettings.GetFov() < 45)
                {
                    m_configurationSettings.SetFov(m_configurationSettings.GetFov() + 1);
                }
                else
                {
                    m_configurationSettings.SetFov(25);
                }
            }
        }
        else if (m_subMenuSelected == subMenuControls)
        {
            if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size())
            {
                if (m_configurationSettings.GetMouseSensitivity() < 20)
                {
                    m_configurationSettings.SetMouseSensitivity(m_configurationSettings.GetMouseSensitivity() + 1);
                }
            }
            if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 1)
            {
                if (m_configurationSettings.GetTurnSpeed() < 250)
                {
                    m_configurationSettings.SetTurnSpeed(m_configurationSettings.GetTurnSpeed() + 10);
                }
            }
        }
    }
}

MenuCommand Catacomb3DMenu::EnterKeyPressed()
{
    MenuCommand command = MenuCommandNone;
    if (m_subMenuSelected == subMenuMain) // Main menu
    {
        if (m_menuItemSelected == 0)
        {
            // New game
            m_subMenuSelected = subMenuNewGame;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == 1 && m_savedGames.size() > 0)
        {
            m_subMenuSelected = subMenuRestoreGame;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == 2 && m_saveGameEnabled)
        {
            m_subMenuSelected = subMenuSaveGame;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == 3)
        {
            // Configure
            m_subMenuSelected = subMenuConfigure;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == 4)
        {
            // Return to demo or game
            m_menuItemSelected = 0;
            command = MenuCommandCloseMenu;
        }
        else if (m_menuItemSelected == 5 && m_saveGameEnabled)
        {
            // End game
            m_askForEndGame = true;
        }
        else if (m_menuItemSelected == 6)
        {
            // Skull 'n' Bones
            m_subMenuSelected = subMenuSkullNBones;
            m_menuItemSelected = 0;
            m_skullNBones.Reset();
        }
        else if (m_menuItemSelected == 7)
        {
            m_askForQuit = true;
        }
    }
    else if (m_subMenuSelected == subMenuNewGame)
    {
        if (m_saveGameEnabled)
        {
            m_askForEndGame = true;
        }
        else
        {
            command =
                (m_menuItemSelected == 0) ? MenuCommandStartNewGameEasy :
                (m_menuItemSelected == 1) ? MenuCommandStartNewGameNormal :
                MenuCommandStartNewGameHard;
            m_subMenuSelected = subMenuMain;
            m_menuItemSelected = 0;
        }
    }
    else if (m_subMenuSelected == subMenuVideo)
    {
        if (m_menuItemSelected == 0)
        {
            if (m_configurationSettings.GetScreenMode() == Windowed)
            {
                m_configurationSettings.SetScreenMode(Fullscreen);
            }
            else if (m_configurationSettings.GetScreenMode() == Fullscreen)
            {
                m_configurationSettings.SetScreenMode(BorderlessWindowed);
            }
            else
            {
                m_configurationSettings.SetScreenMode(Windowed);
            }
        }
        else if (m_menuItemSelected == 1)
        {
            if (m_configurationSettings.GetAspectRatio() == 1)
            {
                m_configurationSettings.SetAspectRatio(0);
            }
            else
            {
                m_configurationSettings.SetAspectRatio(1);
            }
        }
        else if (m_menuItemSelected == 2)
        {
            if (m_configurationSettings.GetFov() == 45)
            {
                m_configurationSettings.SetFov(25);
            }
            else
            {
                m_configurationSettings.SetFov(m_configurationSettings.GetFov() + 1);
            }
        }
        else if (m_menuItemSelected == 3)
        {
            if (m_configurationSettings.GetTextureFilter() == IRenderer::Nearest)
            {
                m_configurationSettings.SetTextureFilter(IRenderer::Linear);
            }
            else
            {
                m_configurationSettings.SetTextureFilter(IRenderer::Nearest);
            }
        }
        else if (m_menuItemSelected == 4)
        {
            m_configurationSettings.SetDepthShading(!m_configurationSettings.GetDepthShading());
        }
        else if (m_menuItemSelected == 5)
        {
            const ShowFpsMode previousShowFpsMode = m_configurationSettings.GetShowFps();
            const ShowFpsMode nextShowFpsMode =
                (previousShowFpsMode == Off) ? Minimal :
                (previousShowFpsMode == Minimal) ? Extended :
                Off;
            m_configurationSettings.SetShowFps(nextShowFpsMode);
        }
        else if (m_menuItemSelected == 6)
        {
            m_configurationSettings.SetVSync(!m_configurationSettings.GetVSync());
        }
        else if (m_menuItemSelected == 7)
        {
            const OverheadMapMode previousOverHeadMapMode = m_configurationSettings.GetOverHeadMapMode();
            const OverheadMapMode nextOverHeadMapMode =
                (previousOverHeadMapMode == Classic) ? Isometric :
                (previousOverHeadMapMode == Isometric) ? TopDown :
                Classic;
            m_configurationSettings.SetOverHeadMapMode(nextOverHeadMapMode);
        }
    }
    else if (m_subMenuSelected == subMenuControls)
    {
        if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size())
        {
            // Mouse sensitivity
            if (m_configurationSettings.GetMouseSensitivity() < 15)
            {
                m_configurationSettings.SetMouseSensitivity(m_configurationSettings.GetMouseSensitivity() + 1);
            }
            else
            {
                m_configurationSettings.SetMouseSensitivity(1);
            }
        }
        else if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 1)
        {
            // Turn speed
            if (m_configurationSettings.GetTurnSpeed() < 250)
            {
                m_configurationSettings.SetTurnSpeed(m_configurationSettings.GetTurnSpeed() + 10);
            }
            else
            {
                m_configurationSettings.SetTurnSpeed(100);
            }
        }
        else if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() - 1)
        {
            // Mouse look
            m_configurationSettings.SetMouseLook(!m_configurationSettings.GetMouseLook());
        }
        else if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 2)
        {
            // Always run
            m_configurationSettings.SetAlwaysRun(!m_configurationSettings.GetAlwaysRun());
        }
        else if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 3)
        {
            // Auto Fire
            m_configurationSettings.SetAutoFire(!m_configurationSettings.GetAutoFire());
        }
        else if (m_menuItemSelected < m_configurationSettings.GetControlsMap().GetActionLabels().size() - 1)
        {
            // Any of the control options
            if (!m_waitingForKeyToBind)
            {
                m_waitingForKeyToBind = true;
            }
        }
    }
    else if (m_subMenuSelected == subMenuSound)
    {
        m_configurationSettings.SetSoundMode(m_menuItemSelected);
    }
    else if (m_subMenuSelected == subMenuMusic)
    {
        m_configurationSettings.SetMusicOn(m_menuItemSelected != 0);
    }
    else if (m_subMenuSelected == subMenuRestoreGame)
    {
        m_newSaveGameName = m_savedGames.at(m_menuItemSelected);
        if (m_saveGameEnabled)
        {
            m_askForEndGame = true;
        }
        else
        {
            m_subMenuSelected = subMenuMain;
            m_menuItemSelected = 0;
            return MenuCommandLoadGame;
        }
    }
    else if (m_subMenuSelected == subMenuSaveGame)
    {
        if (m_menuItemSelected == 0)
        {
            if (!m_waitingForNewSaveGameName)
            {
                m_waitingForNewSaveGameName = true;
                m_newSaveGameName = "";
            }
            else 
            {
                if (IsNewSaveGameNameAlreadyInUse())
                {
                    m_askForOverwrite = true;
                }
                else if (m_newSaveGameName.size() == 0)
                {
                    // Not a valid name, do not store
                }
                else
                {
                    command = MenuCommandSaveGame;
                    m_menuItemSelected = 0;
                }
                
                m_waitingForNewSaveGameName = false;
            }
        }
        else
        {
            m_askForOverwrite = true;
            m_newSaveGameName = m_savedGames.at(m_menuItemSelected - 1);
        }
    }
    else if (m_subMenuSelected == subMenuConfigure)
    {
        if (m_menuItemSelected == 0)
        {
            // Sound
            m_subMenuSelected = subMenuSound;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == 1)
        {
            // Music
            m_subMenuSelected = subMenuMusic;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == 2)
        {
            // Video
            m_subMenuSelected = subMenuVideo;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == 3)
        {
            // Controls
            m_subMenuSelected = subMenuControls;
            m_menuItemSelected = 0;
        }
    }
    else if (m_subMenuSelected == subMenuHighScores)
    {
        m_subMenuSelected = subMenuMain;
        m_menuItemSelected = 0;
        m_highScores.FinishNameOfNewScore();
        command = MenuCommandEndGame;
    }

    return command;
}

void Catacomb3DMenu::DrawSavedGameSlot(IRenderer& renderer, const uint16_t slotPosition, const bool bright)
{
    const egaColor color = bright ? EgaBrightRed : EgaRed;
    const int16_t offsetYInPixels = 62 + (slotPosition * 11);
    renderer.Render2DBar(80, offsetYInPixels, 148, 1, color);
    renderer.Render2DBar(80, offsetYInPixels + 9, 148, 1, color);
    renderer.Render2DBar(80, offsetYInPixels + 1, 1, 8, color);
    renderer.Render2DBar(227, offsetYInPixels + 1, 1, 8, color);
}

void Catacomb3DMenu::DrawConfirmationDialog(IRenderer& renderer, EgaGraph& egaGraph, const uint16_t width, const std::string& message1, const std::string& message2, const std::string& message3)
{
    const int offsetX = 154 - (width / 2);
    RenderableText renderableText(*egaGraph.GetFont(4));
    renderer.Render2DPicture(egaGraph.GetMaskedPicture(CP_MENUMASKPICM), 74, 48);
    renderer.Render2DBar(offsetX + 1, 81, width - 2, 36, EgaBlack);
    renderer.Render2DBar(offsetX, 80, width, 1, EgaRed);
    renderer.Render2DBar(offsetX, 117, width, 1, EgaRed);
    renderer.Render2DBar(offsetX, 81, 1, 36, EgaRed);
    renderer.Render2DBar(offsetX + width - 1, 81, 1, 36, EgaRed);

    renderableText.Centered(message1, EgaBrightRed, 154, 89);

    renderer.Render2DBar(offsetX + 4, 102, width - 8, 1, EgaRed);
    renderableText.Centered(message2, EgaRed, 154, 104);
    renderableText.Centered(message3, EgaRed, 154, 111);
    renderer.RenderText(renderableText);
}

// Based on US_CenterWindow in ID_US.C of the Catacomb Abyss source code.
void Catacomb3DMenu::DrawCenteredTiledWindow(IRenderer& renderer, EgaGraph* const egaGraph, const uint16_t width, const uint16_t height)
{
    DrawTiledWindow(renderer, egaGraph, (40 - width) / 2, (25 - height) / 2, width, height);
}

void Catacomb3DMenu::DrawTiledWindow(IRenderer& renderer, EgaGraph* const egaGraph, const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height)
{
    renderer.Render2DBar(x * 8, y * 8, width * 8, height * 8, EgaBrightWhite);

    RenderableTiles tiles(*egaGraph->GetTilesSize8Masked());
    tiles.DrawWindow(x, y, width, height);
    renderer.RenderTiles(tiles);
}

void Catacomb3DMenu::Draw(IRenderer& renderer, EgaGraph* const egaGraph, const uint16_t menuCursorPic, const uint32_t timeStamp)
{
    if (m_menuActivatedTimestamp == 0)
    {
        m_menuActivatedTimestamp = timeStamp;
    }

    const uint32_t loadingDuration = 500;
    if (timeStamp < m_menuActivatedTimestamp + loadingDuration)
    {
        DrawCenteredTiledWindow(renderer, egaGraph, 20, 8);
        RenderableText renderableText(*egaGraph->GetFont(3));
        renderableText.Centered("Loading", EgaBlack, 160, 65);
        renderableText.Centered("Control Panel", EgaBrightRed, 160, 75);
        renderer.RenderText(renderableText);

        const int16_t thx = 88;
        const int16_t thy = 96;
        RenderableTiles tiles (*egaGraph->GetTilesSize8());
        tiles.DrawDialog(thx, thy, 18);
        renderer.RenderTiles(tiles);

        const uint16_t progress = ((timeStamp - m_menuActivatedTimestamp) * 136) / loadingDuration;
        renderer.Render2DBar(thx + 4, thy + 5, progress, 14, EgaBrightYellow);

        return;
    }

    if (m_subMenuSelected == subMenuHighScores)
    {
        m_highScores.Draw(renderer, *egaGraph, timeStamp, HIGHSCORESPIC);
        return;
    }

    renderer.Render2DPicture(egaGraph->GetPicture(CP_MENUSCREENPIC), 0, 0);

    if (m_askForOverwrite)
    {
        RenderableText renderableText(*egaGraph->GetFont(4));
        renderableText.Centered("Warning!", EgaRed, 160, 52);
        renderableText.Centered("Existing stored game", EgaRed, 160, 72);
        renderableText.Centered(m_newSaveGameName, EgaBrightRed, 160, 82);
        renderableText.Centered("will be overwritten!", EgaRed, 160, 92);
        renderableText.Centered("Are you sure?", EgaRed, 160, 102);
        renderableText.Centered("Y / N", EgaRed, 160, 112);
        renderer.RenderText(renderableText);
        return;
    }

    const bool flashIcon = ((timeStamp / 1000) % 2 == 0);
    if (m_subMenuSelected == subMenuMain)
    {
        RenderableTiles renderableTiles(*egaGraph->GetTilesSize8());
        RenderableText renderableText(*egaGraph->GetFont(4));
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_MAINMENUPIC), 80, 48);
        renderableTiles.DrawListBullet(112, 62, true, (m_menuItemSelected == 0) && flashIcon);
        renderableText.LeftAligned("NEW GAME", (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 120, 63);
        renderableTiles.DrawListBullet(112, 70, m_savedGames.size() > 0, (m_menuItemSelected == 1) && flashIcon);
        renderableText.LeftAligned("LOAD GAME", (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 120, 71);
        renderableTiles.DrawListBullet(112, 78, m_saveGameEnabled, (m_menuItemSelected == 2) && flashIcon);
        renderableText.LeftAligned("SAVE GAME", (m_menuItemSelected == 2) ? EgaBrightRed : EgaRed, 120, 79);
        renderableTiles.DrawListBullet(112, 86, true, (m_menuItemSelected == 3) && flashIcon);
        renderableText.LeftAligned("CONFIGURE", (m_menuItemSelected == 3) ? EgaBrightRed : EgaRed, 120, 87);
        renderableTiles.DrawListBullet(112, 94, true, (m_menuItemSelected == 4) && flashIcon);
        const char* returnOptionText = (m_saveGameEnabled) ? "RETURN TO GAME" : "RETURN TO DEMO";
        renderableText.LeftAligned(returnOptionText, (m_menuItemSelected == 4) ? EgaBrightRed : EgaRed, 120, 95);
        renderableTiles.DrawListBullet(112, 102, m_saveGameEnabled, (m_menuItemSelected == 5) && flashIcon);
        renderableText.LeftAligned("END GAME", (m_menuItemSelected == 5) ? EgaBrightRed : EgaRed, 120, 103);
        renderableTiles.DrawListBullet(112, 110, true, (m_menuItemSelected == 6) && flashIcon);
        renderableText.LeftAligned("SKULL 'N' BONES", (m_menuItemSelected == 6) ? EgaBrightRed : EgaRed, 120, 111);
        renderableTiles.DrawListBullet(112, 118, true, (m_menuItemSelected == 7) && flashIcon);
        renderableText.LeftAligned("QUIT", (m_menuItemSelected == 7) ? EgaBrightRed : EgaRed, 120, 119);

        renderableText.LeftAligned("Arrows move", EgaRed, 78, 135);
        renderableText.LeftAligned("Enter selects", EgaRed, 163, 135);
        renderableText.Centered("Esc to close", EgaRed, 154, 144);
        renderer.RenderTiles(renderableTiles);
        renderer.RenderText(renderableText);

        if (m_askForQuit)
        {
            DrawConfirmationDialog(renderer, *egaGraph, 90, "REALLY QUIT?", "PRESS Y TO QUIT", "ESC TO BACK OUT");
        }

        if (m_askForEndGame)
        {
            DrawConfirmationDialog(renderer, *egaGraph, 142, "REALLY END CURRENT GAME?", "PRESS Y TO END IT", "ESC TO BACK OUT");
        }
    }
    else if (m_subMenuSelected == subMenuNewGame)
    {
        RenderableTiles renderableTiles(*egaGraph->GetTilesSize8());
        RenderableText renderableText(*egaGraph->GetFont(4));
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_NEWGAMEMENUPIC), 80, 48);
        renderableTiles.DrawListBullet(88, 62, true, (m_menuItemSelected == 0) && flashIcon);
        renderableText.LeftAligned("BEGIN EASY GAME", (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 96, 63);
        renderableTiles.DrawListBullet(88, 70, true, (m_menuItemSelected == 1) && flashIcon);
        renderableText.LeftAligned("BEGIN NORMAL GAME", (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 96, 71);
        renderableTiles.DrawListBullet(88, 78, true, (m_menuItemSelected == 2) && flashIcon);
        renderableText.LeftAligned("BEGIN HARD GAME", (m_menuItemSelected == 2) ? EgaBrightRed : EgaRed, 96, 79);

        renderableText.LeftAligned("Arrows move", EgaRed, 78, 135);
        renderableText.LeftAligned("Enter selects", EgaRed, 163, 135);
        renderableText.Centered("Esc to back out", EgaRed, 154, 144);
        renderer.RenderTiles(renderableTiles);
        renderer.RenderText(renderableText);

        if (m_askForEndGame)
        {
            DrawConfirmationDialog(renderer, *egaGraph, 142, "YOU'RE IN A GAME", "PRESS Y FOR NEW GAME", "ESC TO BACK OUT");
        }
    }
    else if (m_subMenuSelected == subMenuConfigure)
    {
        RenderableTiles renderableTiles(*egaGraph->GetTilesSize8());
        RenderableText renderableText(*egaGraph->GetFont(4));
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_CONFIGMENUPIC), 80, 48);
        renderableTiles.DrawListBullet(88, 62, true, (m_menuItemSelected == 0) && flashIcon);
        renderableText.LeftAligned("SOUND", (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 96, 63);
        renderableTiles.DrawListBullet(88, 70, true, (m_menuItemSelected == 1) && flashIcon);
        renderableText.LeftAligned("MUSIC", (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 96, 71);
        renderableTiles.DrawListBullet(88, 78, true, (m_menuItemSelected == 2) && flashIcon);
        renderableText.LeftAligned("VIDEO", (m_menuItemSelected == 2) ? EgaBrightRed : EgaRed, 96, 79);
        renderableTiles.DrawListBullet(88, 86, true, (m_menuItemSelected == 3) && flashIcon);
        renderableText.LeftAligned("CONTROLS", (m_menuItemSelected == 3) ? EgaBrightRed : EgaRed, 96, 87);

        renderableText.LeftAligned("Arrows move", EgaRed, 78, 135);
        renderableText.LeftAligned("Enter selects", EgaRed, 163, 135);
        renderableText.Centered("Esc to back out", EgaRed, 154, 144);
        renderer.RenderTiles(renderableTiles);
        renderer.RenderText(renderableText);
    }
    else if (m_subMenuSelected == subMenuVideo)
    {
        RenderableTiles renderableTiles(*egaGraph->GetTilesSize8());

        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        
        // The header for the Video Menu is composed out of letters from other menu headers, as it did not exist
        // in the original game.
        renderer.Render2DPictureSegment(egaGraph->GetPicture(CP_SAVEMENUPIC), 80, 48, 16, 0, 8, 12); // V
        renderer.Render2DPictureSegment(egaGraph->GetPicture(CP_MAINMENUPIC), 88, 48, 19, 0, 3, 12); // I
        renderer.Render2DPictureSegment(egaGraph->GetPicture(CP_LOADMENUPIC), 91, 48, 24, 0, 7, 12); // D
        renderer.Render2DPictureSegment(egaGraph->GetPicture(CP_SAVEMENUPIC), 98, 48, 24, 0, 7, 12); // E
        renderer.Render2DPictureSegment(egaGraph->GetPicture(CP_LOADMENUPIC), 105, 48, 10, 0, 7, 12); // O
        renderer.Render2DPictureSegment(egaGraph->GetPicture(CP_MAINMENUPIC), 112, 48, 30, 0, 34, 12); // MENU

        RenderableText renderableText(*egaGraph->GetFont(4));
        renderableTiles.DrawListBullet(76, 62, true, (m_menuItemSelected == 0) && flashIcon);
        renderableText.LeftAligned("Screen Mode", (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 84, 63);
        const char* screenModeStr =
            (m_configurationSettings.GetScreenMode() == Windowed) ? "Windowed" :
            (m_configurationSettings.GetScreenMode() == Fullscreen) ? "Fullscreen" :
            "Borderless";
        renderableText.LeftAligned(screenModeStr, (m_menuItemSelected == 0) ? EgaLightGray : EgaDarkGray, 180, 63);

        renderableTiles.DrawListBullet(76, 70, true, (m_menuItemSelected == 1) && flashIcon);
        renderableText.LeftAligned("Aspect ratio", (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 84, 70);
        const std::string& aspectRatioStr = aspectRatios[m_configurationSettings.GetAspectRatio()].description;
        renderableText.LeftAligned(aspectRatioStr, (m_menuItemSelected == 1) ? EgaLightGray : EgaDarkGray, 160, 71);

        renderableTiles.DrawListBullet(76, 78, true, (m_menuItemSelected == 2) && flashIcon);
        renderableText.LeftAligned("Field Of View (Y)", (m_menuItemSelected == 2) ? EgaBrightRed : EgaRed, 84, 79);
        char fovStr[40];
        sprintf_s(fovStr, 40, "%d", m_configurationSettings.GetFov());
        renderableText.LeftAligned(fovStr, (m_menuItemSelected == 2) ? EgaLightGray : EgaDarkGray, 180, 79);

        renderableTiles.DrawListBullet(76, 86, true, (m_menuItemSelected == 3) && flashIcon);
        renderableText.LeftAligned("Texture filtering", (m_menuItemSelected == 3) ? EgaBrightRed : EgaRed, 84, 87);
        const char* textureFilterStr = (m_configurationSettings.GetTextureFilter() == IRenderer::Nearest) ? "Nearest" : "Linear";
        renderableText.LeftAligned(textureFilterStr, (m_menuItemSelected == 3) ? EgaLightGray : EgaDarkGray, 180, 87);

        renderableTiles.DrawListBullet(76, 94, true, (m_menuItemSelected == 4) && flashIcon);
        renderableText.LeftAligned("Depth shading", (m_menuItemSelected == 4) ? EgaBrightRed : EgaRed, 84, 95);
        const char* depthShadingStr = (m_configurationSettings.GetDepthShading()) ? "Enabled" : "Disabled";
        renderableText.LeftAligned(depthShadingStr, (m_menuItemSelected == 4) ? EgaLightGray : EgaDarkGray, 180, 95);

        renderableTiles.DrawListBullet(76, 102, true, (m_menuItemSelected == 5) && flashIcon);
        renderableText.LeftAligned("Show frame rate", (m_menuItemSelected == 5) ? EgaBrightRed : EgaRed, 84, 103);
        const ShowFpsMode showFpsMode = m_configurationSettings.GetShowFps();
        const char* showFpsStr =
            (showFpsMode == Minimal) ? "Minimal" :
            (showFpsMode == Extended) ? "Extended" :
            "Off";
        renderableText.LeftAligned(showFpsStr, (m_menuItemSelected == 5) ? EgaLightGray : EgaDarkGray, 180, 103);

        const bool vsyncSupported = renderer.IsVSyncSupported();
        renderableTiles.DrawListBullet(76, 110, vsyncSupported, (m_menuItemSelected == 6) && flashIcon);
        renderableText.LeftAligned("VSync", (m_menuItemSelected == 6) ? EgaBrightRed : EgaRed, 84, 111);
        const char* vsyncStr = (!vsyncSupported) ? "Not supported" : (m_configurationSettings.GetVSync()) ? "Enabled" : "Disabled";
        renderableText.LeftAligned(vsyncStr, (m_menuItemSelected == 6) ? EgaLightGray : EgaDarkGray, !vsyncSupported ? 160 : 180, 111);

        renderableTiles.DrawListBullet(76, 118, true, (m_menuItemSelected == 7) && flashIcon);
        renderableText.LeftAligned("Overhead Map", (m_menuItemSelected == 7) ? EgaBrightRed : EgaRed, 84, 119);
        const char* overheadMapModeStr =
            (m_configurationSettings.GetOverHeadMapMode() == Classic) ? "Classic" :
            (m_configurationSettings.GetOverHeadMapMode() == Isometric) ? "Isometric" :
            "Top down";
        renderableText.LeftAligned(overheadMapModeStr, (m_menuItemSelected == 7) ? EgaLightGray : EgaDarkGray, 180, 119);

        renderableText.LeftAligned("Arrows move", EgaRed, 78, 135);
        renderableText.LeftAligned("Enter selects", EgaRed, 163, 135);
        renderableText.Centered("Esc to back out", EgaRed, 154, 144);
        renderer.RenderTiles(renderableTiles);
        renderer.RenderText(renderableText);
    }
    else if (m_subMenuSelected == subMenuControls)
    {
        RenderableTiles renderableTiles(*egaGraph->GetTilesSize8());

        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_KEYBUTTONPIC), 80, 48);
        RenderableText renderableText(*egaGraph->GetFont(4));
        RenderableText renderableTextDefaultFont(*DefaultFont::Get(renderer, 7));

        uint16_t index = 1;
        while (index < 9)
        {
            if (index + m_menuItemOffset < (uint16_t)m_configurationSettings.GetControlsMap().GetActionLabels().size())
            {
                renderableTiles.DrawListBullet(76, 62 + ((index - 1) * 8), true, (m_menuItemSelected == index + m_menuItemOffset - 1) && flashIcon);
                const std::string& actionLabel = m_configurationSettings.GetControlsMap().GetActionLabels().at((ControlAction)(index + m_menuItemOffset));
                const uint16_t yOffset = 63 + ((index - 1) * 8);
                renderableText.LeftAligned(actionLabel, (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaBrightRed : EgaRed, 84, yOffset);
                if (m_waitingForKeyToBind && m_menuItemSelected == index + m_menuItemOffset - 1)
                {
                    renderableText.LeftAligned("< Press key to bind >", EgaLightGray, 160, yOffset);
                }
                else
                {
                    // The name of the keys is shown with the default font, as the original font from the game lacks some required characters.
                    renderableTextDefaultFont.LeftAligned(m_configurationSettings.GetControlsMap().GetKeyStringFromAction((ControlAction)(index + m_menuItemOffset)), (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaLightGray : EgaDarkGray, 160, yOffset);
                }
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size())
            {
                renderableTiles.DrawListBullet(76, 62 + ((index - 1) * 8), true, (m_menuItemSelected == index + m_menuItemOffset - 1) && flashIcon);
                renderableText.LeftAligned("Mouse Look", (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaBrightRed : EgaRed, 84, 55 + (index * 8));
                const char* mouseLookStr = (m_configurationSettings.GetMouseLook()) ? "Enabled" : "Disabled";
                renderableText.LeftAligned(mouseLookStr, (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaLightGray : EgaDarkGray, 160, 55 + (index * 8));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 1)
            {
                renderableTiles.DrawListBullet(76, 62 + ((index - 1) * 8), true, (m_menuItemSelected == index + m_menuItemOffset - 1) && flashIcon);
                renderableText.LeftAligned("Mouse Sens.", (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaBrightRed : EgaRed, 84, 55 + (index * 8));
                char mouseSensitivityStr[5];
                sprintf_s(mouseSensitivityStr, 5, "%d", m_configurationSettings.GetMouseSensitivity());
                renderableText.LeftAligned(mouseSensitivityStr, (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaLightGray : EgaDarkGray, 160, 55 + (index * 8));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 2)
            {
                renderableTiles.DrawListBullet(76, 62 + ((index - 1) * 8), true, (m_menuItemSelected == index + m_menuItemOffset - 1) && flashIcon);
                renderableText.LeftAligned("Turn Speed", (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaBrightRed : EgaRed, 84, 55 + (index * 8));
                char turnSpeedStr[5];
                sprintf_s(turnSpeedStr, 5, "%d", m_configurationSettings.GetTurnSpeed());
                renderableText.LeftAligned(turnSpeedStr, (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaLightGray : EgaDarkGray, 160, 55 + (index * 8));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 3)
            {
                renderableTiles.DrawListBullet(76, 62 + ((index - 1) * 8), true, (m_menuItemSelected == index + m_menuItemOffset - 1) && flashIcon);
                renderableText.LeftAligned("Always Run", (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaBrightRed : EgaRed, 84, 55 + (index * 8));
                const char* alwaysRunStr = (m_configurationSettings.GetAlwaysRun()) ? "Enabled" : "Disabled";
                renderableText.LeftAligned(alwaysRunStr, (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaLightGray : EgaDarkGray, 160, 55 + (index * 8));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 4)
            {
                renderableTiles.DrawListBullet(76, 62 + ((index - 1) * 8), true, (m_menuItemSelected == index + m_menuItemOffset - 1) && flashIcon);
                renderableText.LeftAligned("Auto Fire", (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaBrightRed : EgaRed, 84, 55 + (index * 8));
                const char* autoFireStr = (m_configurationSettings.GetAutoFire()) ? "Enabled" : "Disabled";
                renderableText.LeftAligned(autoFireStr, (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaLightGray : EgaDarkGray, 160, 55 + (index * 8));
            }
            index++;
        }


        renderableText.LeftAligned("Arrows move", EgaRed, 78, 135);
        renderableText.LeftAligned("Enter selects", EgaRed, 163, 135);
        renderableText.Centered("Esc to back out", EgaRed, 154, 144);
        renderer.RenderTiles(renderableTiles);
        renderer.RenderText(renderableText);
        renderer.RenderText(renderableTextDefaultFont);
    }
    else if (m_subMenuSelected == subMenuSound)
    {
        RenderableTiles renderableTiles(*egaGraph->GetTilesSize8());
        RenderableText renderableText(*egaGraph->GetFont(4));
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_SOUNDMENUPIC), 80, 48);

        const uint8_t soundMode = m_configurationSettings.GetSoundMode();
        renderableTiles.DrawRadioButton(88, 62, (soundMode == 0), (m_menuItemSelected == 0) && flashIcon);
        renderableText.LeftAligned("NO SOUND EFFECTS", (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 96, 63);
        renderableTiles.DrawRadioButton(88, 70, (soundMode == 1), (m_menuItemSelected == 1) && flashIcon);
        renderableText.LeftAligned("PC SPEAKER", (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 96, 71);
        renderableTiles.DrawRadioButton(88, 78, (soundMode == 2), (m_menuItemSelected == 2) && flashIcon);
        renderableText.LeftAligned("ADLIB/SOUNDBLASTER", (m_menuItemSelected == 2) ? EgaBrightRed : EgaRed, 96, 79);

        renderableText.LeftAligned("Arrows move", EgaRed, 78, 135);
        renderableText.LeftAligned("Enter selects", EgaRed, 163, 135);
        renderableText.Centered("Esc to back out", EgaRed, 154, 144);
        renderer.RenderTiles(renderableTiles);
        renderer.RenderText(renderableText);
    }
    else if (m_subMenuSelected == subMenuMusic)
    {
        RenderableTiles renderableTiles(*egaGraph->GetTilesSize8());
        RenderableText renderableText(*egaGraph->GetFont(4));
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_MUSICMENUPIC), 80, 48);

        const bool musicOn = m_configurationSettings.GetMusicOn();
        renderableTiles.DrawRadioButton(88, 62, !musicOn, (m_menuItemSelected == 0) && flashIcon);
        renderableText.LeftAligned("NO MUSIC", (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 96, 63);
        renderableTiles.DrawRadioButton(88, 70, musicOn, (m_menuItemSelected == 1) && flashIcon);
        renderableText.LeftAligned("ADLIB/SOUNDBLASTER", (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 96, 71);

        renderableText.LeftAligned("Arrows move", EgaRed, 78, 135);
        renderableText.LeftAligned("Enter selects", EgaRed, 163, 135);
        renderableText.Centered("Esc to back out", EgaRed, 154, 144);
        renderer.RenderTiles(renderableTiles);
        renderer.RenderText(renderableText);
    }
    else if (m_subMenuSelected == subMenuRestoreGame)
    {
        const uint16_t maxSlotsVisible = 6;
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_LOADMENUPIC), 80, 48);
        RenderableText renderableText(*egaGraph->GetFont(4));
        uint8_t index = 0;
        for (auto savedGameName : m_savedGames)
        {
            if (index >= m_menuItemOffset && index < m_menuItemOffset + maxSlotsVisible)
            {
                renderableText.LeftAligned(savedGameName, (m_menuItemSelected == index) ? EgaBrightRed : EgaRed, 82, 64 + ((index - m_menuItemOffset) * 11));
                const bool bright = (index == m_menuItemSelected && flashIcon);
                DrawSavedGameSlot(renderer, index - m_menuItemOffset, bright);
            }
            index++;
        }

        renderableText.LeftAligned("Arrows move", EgaRed, 78, 135);
        renderableText.LeftAligned("Enter accepts", EgaRed, 163, 135);
        renderableText.Centered("Esc to back out", EgaRed, 154, 144);
        renderer.RenderText(renderableText);

        if (m_askForEndGame)
        {
            DrawConfirmationDialog(renderer, *egaGraph, 142, "YOU'RE IN A GAME", "PRESS Y TO LOAD GAME", "ESC TO BACK OUT");
        }
    }
    else if (m_subMenuSelected == subMenuSaveGame)
    {
        const uint16_t maxSlotsVisible = 6;
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_SAVEMENUPIC), 80, 48);
        RenderableText renderableText(*egaGraph->GetFont(4));
        if (m_menuItemOffset == 0)
        {
            if (!m_waitingForNewSaveGameName)
            {
                renderableText.Centered("Empty", (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 154, 64);
            }
            else
            {
                const std::string saveGameName = m_newSaveGameName + "_";
                renderableText.LeftAligned(saveGameName, EgaBrightRed, 82, 64);
            }
        }

        uint8_t index = 1;
        for (auto savedGameName : m_savedGames)
        {
            if (index >= m_menuItemOffset && index < m_menuItemOffset + maxSlotsVisible)
            {
                renderableText.LeftAligned(savedGameName, (m_menuItemSelected == index) ? EgaBrightRed : EgaRed, 82, 64 + ((index - m_menuItemOffset) * 11));
            }
            index++;
        }

        const uint16_t totalNumberOfSlots = (uint16_t)m_savedGames.size() + 1;
        const uint16_t numberOfSlotsVisible = (totalNumberOfSlots < maxSlotsVisible) ? totalNumberOfSlots : maxSlotsVisible;
        for (uint16_t slotPosition = 0; slotPosition < numberOfSlotsVisible; slotPosition++)
        {
            const bool bright = ((slotPosition + m_menuItemOffset) == m_menuItemSelected && (flashIcon || m_waitingForNewSaveGameName));
            DrawSavedGameSlot(renderer, slotPosition, bright);
        }

        const char* instructionText = m_waitingForNewSaveGameName ? "Type name" : "Arrows move";
        renderableText.LeftAligned(instructionText, EgaRed, 78, 135);
        renderableText.LeftAligned("Enter accepts", EgaRed, 163, 135);
        renderableText.Centered("Esc to back out", EgaRed, 154, 144);
        renderer.RenderText(renderableText);
    }
    else if (m_subMenuSelected == subMenuSkullNBones)
    {
        m_skullNBones.Draw(renderer, *egaGraph, timeStamp);
    }
}

void Catacomb3DMenu::SetSaveGameEnabled(const bool enabled)
{
    m_saveGameEnabled = enabled;
}

const std::string& Catacomb3DMenu::GetNewSaveGameName() const
{
    return m_newSaveGameName;
}

bool Catacomb3DMenu::KeyIsSuitableForSaveGameName(const SDL_Keycode keyCode)
{
    return ((keyCode >= SDLK_a && keyCode <= SDLK_z) ||
            (keyCode >= SDLK_0 && keyCode <= SDLK_9));
}

void Catacomb3DMenu::OpenRestoreGameMenu()
{
    m_menuActive = true;
    m_menuItemSelected = 0;
    m_subMenuSelected = subMenuRestoreGame;
    m_menuItemOffset = 0;
    m_waitingForKeyToBind = false;
    m_waitingForNewSaveGameName = false;
}

void Catacomb3DMenu::OpenSaveGameMenu()
{
    m_menuActive = true;
    m_menuItemSelected = 0;
    m_subMenuSelected = subMenuSaveGame;
    m_menuItemOffset = 0;
    m_waitingForKeyToBind = false;
    m_waitingForNewSaveGameName = false;
}

void Catacomb3DMenu::OpenSoundMenu()
{
    m_menuActive = true;
    m_menuItemSelected = 0;
    m_subMenuSelected = subMenuSound;
    m_menuItemOffset = 0;
    m_waitingForKeyToBind = false;
    m_waitingForNewSaveGameName = false;
}

bool Catacomb3DMenu::IsNewSaveGameNameAlreadyInUse() const
{
    bool isInUse = false;
    for (uint16_t i = 0; i < m_savedGames.size(); i++)
    {
        isInUse |= (m_savedGames.at(i) == m_newSaveGameName);
    }

    return isInUse;
}

void Catacomb3DMenu::CheckHighScore(const uint16_t level, const uint32_t score)
{
    m_subMenuSelected = subMenuHighScores;
    m_highScores.TryToAddNewScore(score, level + 1);
}