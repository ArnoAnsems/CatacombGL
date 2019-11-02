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
#include "..\..\ThirdParty\SDL\include\SDL_keyboard.h"

const uint8_t subMenuMain = 0;
const uint8_t subMenuVideo = 1;
const uint8_t subMenuControls = 2;
const uint8_t subMenuSound = 3;
const uint8_t subMenuRestoreGame = 4;
const uint8_t subMenuSaveGame = 5;
const uint8_t subMenuNewGame = 6;
const uint8_t subMenuConfigure = 7;
const uint8_t subMenuMusic = 8;

const uint16_t browseMenuSound = 0;

Catacomb3DMenu::Catacomb3DMenu(ConfigurationSettings& configurationSettings, AudioPlayer& audioPlayer, std::vector<std::string>& savedGames) :
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
    m_askForOverwrite (false)
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

    if (playerInput.IsKeyJustPressed(SDLK_RETURN))
    {
        command = EnterKeyPressed();
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
    else if (playerInput.IsKeyJustPressed(SDLK_ESCAPE))
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

    return command;
}

void Catacomb3DMenu::MenuDown()
{
    if (m_menuActive)
    {
        m_audioPlayer.Play(browseMenuSound);
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
            if (m_menuItemSelected == 6)
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
            if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 2)
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
        m_audioPlayer.Play(browseMenuSound);
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
                m_menuItemSelected = 6;
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
                m_menuItemSelected = (uint8_t)m_configurationSettings.GetControlsMap().GetActionLabels().size() + 2;
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
        m_audioPlayer.Play(browseMenuSound);
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
        m_audioPlayer.Play(browseMenuSound);
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
                if (m_configurationSettings.GetMouseSensitivity() < 15)
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
            // Return to demo
        }
        else if (m_menuItemSelected == 5)
        {
            // End game
        }
        else if (m_menuItemSelected == 6)
        {
            // Skull 'n' Bones
        }
        else if (m_menuItemSelected == 7)
        {
            command = MenuCommandExitGame;
        }
    }
    else if (m_subMenuSelected == subMenuNewGame)
    {
        command =
            (m_menuItemSelected == 0) ? MenuCommandStartNewGameEasy :
            (m_menuItemSelected == 1) ? MenuCommandStartNewGameNormal :
            MenuCommandStartNewGameHard;
        m_menuActive = false;
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
            m_configurationSettings.SetShowFps(!m_configurationSettings.GetShowFps());
        }
        else if (m_menuItemSelected == 6)
        {
            m_configurationSettings.SetVSync(!m_configurationSettings.GetVSync());
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
        command = MenuCommandLoadGame;
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

void Catacomb3DMenu::Draw(IRenderer& renderer, EgaGraph* const egaGraph, const uint16_t menuCursorPic, const uint32_t timeStamp)
{
    renderer.Render2DPicture(egaGraph->GetPicture(CP_MENUSCREENPIC), 0, 0);

    if (m_askForOverwrite)
    {  
        renderer.RenderTextCentered("Warning!", egaGraph->GetFont(4), EgaRed, 160, 52);
        renderer.RenderTextCentered("Existing stored game", egaGraph->GetFont(4), EgaRed, 160, 72);
        renderer.RenderTextCentered(m_newSaveGameName.c_str(), egaGraph->GetFont(4), EgaBrightRed, 160, 82);
        renderer.RenderTextCentered("will be overwritten!", egaGraph->GetFont(4), EgaRed, 160, 92);
        renderer.RenderTextCentered("Are you sure?", egaGraph->GetFont(4), EgaRed, 160, 102);
        renderer.RenderTextCentered("Y / N", egaGraph->GetFont(4), EgaRed, 160, 112);
        return;
    }

    const bool flashIcon = ((timeStamp / 1000) % 2 == 0);
    if (m_subMenuSelected == subMenuMain)
    {
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_MAINMENUPIC), 80, 48);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 0) && flashIcon) ? 93 : 92), 112, 62);
        renderer.RenderTextLeftAligned("NEW GAME", egaGraph->GetFont(4), (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 120, 63);
        if (m_savedGames.size() > 0)
        {
            renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 1) && flashIcon) ? 93 : 92), 112, 70);
        }
        else
        {
            renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 1) && flashIcon) ? 97 : 96), 112, 70);
        }
        renderer.RenderTextLeftAligned("LOAD GAME", egaGraph->GetFont(4), (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 120, 71);
        if (m_saveGameEnabled)
        {
            renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 2) && flashIcon) ? 93 : 92), 112, 78);
        }
        else
        {
            renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 2) && flashIcon) ? 97 : 96), 112, 78);
        }
        
        renderer.RenderTextLeftAligned("SAVE GAME", egaGraph->GetFont(4), (m_menuItemSelected == 2) ? EgaBrightRed : EgaRed, 120, 79);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 3) && flashIcon) ? 93 : 92), 112, 86);
        renderer.RenderTextLeftAligned("CONFIGURE", egaGraph->GetFont(4), (m_menuItemSelected == 3) ? EgaBrightRed : EgaRed, 120, 87);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 4) && flashIcon) ? 93 : 92), 112, 94);
        renderer.RenderTextLeftAligned("RETURN TO DEMO", egaGraph->GetFont(4), (m_menuItemSelected == 4) ? EgaBrightRed : EgaRed, 120, 95);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 5) && flashIcon) ? 93 : 92), 112, 102);
        renderer.RenderTextLeftAligned("END GAME", egaGraph->GetFont(4), (m_menuItemSelected == 5) ? EgaBrightRed : EgaRed, 120, 103);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 6) && flashIcon) ? 93 : 92), 112, 110);
        renderer.RenderTextLeftAligned("SKULL 'N' BONES", egaGraph->GetFont(4), (m_menuItemSelected == 6) ? EgaBrightRed : EgaRed, 120, 111);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 7) && flashIcon) ? 93 : 92), 112, 118);
        renderer.RenderTextLeftAligned("QUIT", egaGraph->GetFont(4), (m_menuItemSelected == 7) ? EgaBrightRed : EgaRed, 120, 119);

        renderer.RenderTextLeftAligned("Arrows move", egaGraph->GetFont(4), EgaRed, 78, 135);
        renderer.RenderTextLeftAligned("Enter selects", egaGraph->GetFont(4), EgaRed, 163, 135);
        renderer.RenderTextCentered("Esc to quit", egaGraph->GetFont(4), EgaRed, 154, 144);
    }
    else if (m_subMenuSelected == subMenuNewGame)
    {
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_NEWGAMEMENUPIC), 80, 48);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 0) && flashIcon) ? 93 : 92), 88, 62);
        renderer.RenderTextLeftAligned("BEGIN EASY GAME", egaGraph->GetFont(4), (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 96, 63);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 1) && flashIcon) ? 93 : 92), 88, 70);
        renderer.RenderTextLeftAligned("BEGIN NORMAL GAME", egaGraph->GetFont(4), (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 96, 71);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 2) && flashIcon) ? 93 : 92), 88, 78);
        renderer.RenderTextLeftAligned("BEGIN HARD GAME", egaGraph->GetFont(4), (m_menuItemSelected == 2) ? EgaBrightRed : EgaRed, 96, 79);

        renderer.RenderTextLeftAligned("Arrows move", egaGraph->GetFont(4), EgaRed, 78, 135);
        renderer.RenderTextLeftAligned("Enter selects", egaGraph->GetFont(4), EgaRed, 163, 135);
        renderer.RenderTextCentered("Esc to back out", egaGraph->GetFont(4), EgaRed, 154, 144);
    }
    else if (m_subMenuSelected == subMenuConfigure)
    {
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_CONFIGMENUPIC), 80, 48);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 0) && flashIcon) ? 93 : 92), 88, 62);
        renderer.RenderTextLeftAligned("SOUND", egaGraph->GetFont(4), (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 96, 63);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 1) && flashIcon) ? 93 : 92), 88, 70);
        renderer.RenderTextLeftAligned("MUSIC", egaGraph->GetFont(4), (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 96, 71);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 2) && flashIcon) ? 93 : 92), 88, 78);
        renderer.RenderTextLeftAligned("VIDEO", egaGraph->GetFont(4), (m_menuItemSelected == 2) ? EgaBrightRed : EgaRed, 96, 79);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 3) && flashIcon) ? 93 : 92), 88, 86);
        renderer.RenderTextLeftAligned("CONTROLS", egaGraph->GetFont(4), (m_menuItemSelected == 3) ? EgaBrightRed : EgaRed, 96, 87);

        renderer.RenderTextLeftAligned("Arrows move", egaGraph->GetFont(4), EgaRed, 78, 135);
        renderer.RenderTextLeftAligned("Enter selects", egaGraph->GetFont(4), EgaRed, 163, 135);
        renderer.RenderTextCentered("Esc to back out", egaGraph->GetFont(4), EgaRed, 154, 144);
    }
    else if (m_subMenuSelected == subMenuVideo)
    {
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

        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 0) && flashIcon) ? 93 : 92), 76, 62);
        renderer.RenderTextLeftAligned("Screen Mode", egaGraph->GetFont(4), (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 84, 63);
        const char* screenModeStr =
            (m_configurationSettings.GetScreenMode() == Windowed) ? "Windowed" :
            (m_configurationSettings.GetScreenMode() == Fullscreen) ? "Fullscreen" :
            "Borderless";
        renderer.RenderTextLeftAligned(screenModeStr, egaGraph->GetFont(4), (m_menuItemSelected == 0) ? EgaLightGray : EgaDarkGray, 180, 63);

        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 1) && flashIcon) ? 93 : 92), 76, 70);
        renderer.RenderTextLeftAligned("Aspect ratio", egaGraph->GetFont(4), (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 84, 70);
        const char* aspectRatioStr = aspectRatios[m_configurationSettings.GetAspectRatio()].description.c_str();
        renderer.RenderTextLeftAligned(aspectRatioStr, egaGraph->GetFont(4), (m_menuItemSelected == 1) ? EgaLightGray : EgaDarkGray, 160, 71);

        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 2) && flashIcon) ? 93 : 92), 76, 78);
        renderer.RenderTextLeftAligned("Field Of View (Y)", egaGraph->GetFont(4), (m_menuItemSelected == 2) ? EgaBrightRed : EgaRed, 84, 79);
        char fovStr[40];
        sprintf_s(fovStr, 40, "%d", m_configurationSettings.GetFov());
        renderer.RenderTextLeftAligned(fovStr, egaGraph->GetFont(4), (m_menuItemSelected == 2) ? EgaLightGray : EgaDarkGray, 180, 79);

        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 3) && flashIcon) ? 93 : 92), 76, 86);
        renderer.RenderTextLeftAligned("Texture filtering", egaGraph->GetFont(4), (m_menuItemSelected == 3) ? EgaBrightRed : EgaRed, 84, 87);
        const char* textureFilterStr = (m_configurationSettings.GetTextureFilter() == IRenderer::Nearest) ? "Nearest" : "Linear";
        renderer.RenderTextLeftAligned(textureFilterStr, egaGraph->GetFont(4), (m_menuItemSelected == 3) ? EgaLightGray : EgaDarkGray, 180, 87);

        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 4) && flashIcon) ? 93 : 92), 76, 94);
        renderer.RenderTextLeftAligned("Depth shading", egaGraph->GetFont(4), (m_menuItemSelected == 4) ? EgaBrightRed : EgaRed, 84, 95);
        const char* depthShadingStr = (m_configurationSettings.GetDepthShading()) ? "Enabled" : "Disabled";
        renderer.RenderTextLeftAligned(depthShadingStr, egaGraph->GetFont(4), (m_menuItemSelected == 4) ? EgaLightGray : EgaDarkGray, 180, 95);

        renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 5) && flashIcon) ? 93 : 92), 76, 102);
        renderer.RenderTextLeftAligned("Show frame rate", egaGraph->GetFont(4), (m_menuItemSelected == 5) ? EgaBrightRed : EgaRed, 84, 103);
        const char* showFpsStr = (m_configurationSettings.GetShowFps()) ? "Enabled" : "Disabled";
        renderer.RenderTextLeftAligned(showFpsStr, egaGraph->GetFont(4), (m_menuItemSelected == 5) ? EgaLightGray : EgaDarkGray, 180, 103);

        const bool vsyncNotSupported = !renderer.IsVSyncSupported();
        if (vsyncNotSupported)
        {
            renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 6) && flashIcon) ? 97 : 96), 76, 110);
        }
        else
        {
            renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == 6) && flashIcon) ? 93 : 92), 76, 110);
        }
        renderer.RenderTextLeftAligned("VSync", egaGraph->GetFont(4), (m_menuItemSelected == 6) ? EgaBrightRed : EgaRed, 84, 111);
        const char* vsyncStr = (vsyncNotSupported) ? "Not supported" : (m_configurationSettings.GetVSync()) ? "Enabled" : "Disabled";
        renderer.RenderTextLeftAligned(vsyncStr, egaGraph->GetFont(4), (m_menuItemSelected == 6) ? EgaLightGray : EgaDarkGray, vsyncNotSupported ? 160 : 180, 111);

        renderer.RenderTextLeftAligned("Arrows move", egaGraph->GetFont(4), EgaRed, 78, 135);
        renderer.RenderTextLeftAligned("Enter selects", egaGraph->GetFont(4), EgaRed, 163, 135);
        renderer.RenderTextCentered("Esc to back out", egaGraph->GetFont(4), EgaRed, 154, 144);
    }
    else if (m_subMenuSelected == subMenuControls)
    {
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_KEYBUTTONPIC), 80, 48);

        uint16_t index = 1;
        while (index < 9)
        {
            if (index + m_menuItemOffset < (uint16_t)m_configurationSettings.GetControlsMap().GetActionLabels().size())
            {
                renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == index + m_menuItemOffset - 1) && flashIcon) ? 93 : 92), 76, 62 + ((index - 1) * 8));
                const std::string& actionLabel = m_configurationSettings.GetControlsMap().GetActionLabels().at((ControlAction)(index + m_menuItemOffset));
                const uint16_t yOffset = 63 + ((index - 1) * 8);
                renderer.RenderTextLeftAligned(actionLabel.c_str(), egaGraph->GetFont(4), (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaBrightRed : EgaRed, 84, yOffset);
                if (m_waitingForKeyToBind && m_menuItemSelected == index + m_menuItemOffset - 1)
                {
                    renderer.RenderTextLeftAligned("< Press key to bind >", egaGraph->GetFont(4), EgaLightGray, 160, yOffset);
                }
                else
                {
                    renderer.RenderTextLeftAligned(m_configurationSettings.GetControlsMap().GetKeyStringFromAction((ControlAction)(index + m_menuItemOffset)).c_str(), egaGraph->GetFont(4), (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaLightGray : EgaDarkGray, 160, yOffset);
                }
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size())
            {
                renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == index + m_menuItemOffset - 1) && flashIcon) ? 93 : 92), 76, 62 + ((index - 1) * 8));
                renderer.RenderTextLeftAligned("Mouse Look", egaGraph->GetFont(4), (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaBrightRed : EgaRed, 84, 55 + (index * 8));
                const char* mouseLookStr = (m_configurationSettings.GetMouseLook()) ? "Enabled" : "Disabled";
                renderer.RenderTextLeftAligned(mouseLookStr, egaGraph->GetFont(4), (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaLightGray : EgaDarkGray, 160, 55 + (index * 8));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 1)
            {
                renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == index + m_menuItemOffset - 1) && flashIcon) ? 93 : 92), 76, 62 + ((index - 1) * 8));
                renderer.RenderTextLeftAligned("Mouse Sens.", egaGraph->GetFont(4), (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaBrightRed : EgaRed, 84, 55 + (index * 8));
                char mouseSensitivityStr[5];
                sprintf_s(mouseSensitivityStr, 5, "%d", m_configurationSettings.GetMouseSensitivity());
                renderer.RenderTextLeftAligned(mouseSensitivityStr, egaGraph->GetFont(4), (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaLightGray : EgaDarkGray, 160, 55 + (index * 8));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 2)
            {
                renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == index + m_menuItemOffset - 1) && flashIcon) ? 93 : 92), 76, 62 + ((index - 1) * 8));
                renderer.RenderTextLeftAligned("Turn Speed", egaGraph->GetFont(4), (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaBrightRed : EgaRed, 84, 55 + (index * 8));
                char turnSpeedStr[5];
                sprintf_s(turnSpeedStr, 5, "%d", m_configurationSettings.GetTurnSpeed());
                renderer.RenderTextLeftAligned(turnSpeedStr, egaGraph->GetFont(4), (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaLightGray : EgaDarkGray, 160, 55 + (index * 8));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 3)
            {
                renderer.Render2DPicture(egaGraph->GetTilesSize8(((m_menuItemSelected == index + m_menuItemOffset - 1) && flashIcon) ? 93 : 92), 76, 62 + ((index - 1) * 8));
                renderer.RenderTextLeftAligned("Always Run", egaGraph->GetFont(4), (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaBrightRed : EgaRed, 84, 55 + (index * 8));
                const char* alwaysRunStr = (m_configurationSettings.GetAlwaysRun()) ? "Enabled" : "Disabled";
                renderer.RenderTextLeftAligned(alwaysRunStr, egaGraph->GetFont(4), (m_menuItemSelected == index + m_menuItemOffset - 1) ? EgaLightGray : EgaDarkGray, 160, 55 + (index * 8));
            }
            index++;
        }


        renderer.RenderTextLeftAligned("Arrows move", egaGraph->GetFont(4), EgaRed, 78, 135);
        renderer.RenderTextLeftAligned("Enter selects", egaGraph->GetFont(4), EgaRed, 163, 135);
        renderer.RenderTextCentered("Esc to back out", egaGraph->GetFont(4), EgaRed, 154, 144);
    }
    else if (m_subMenuSelected == subMenuSound)
    {
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_SOUNDMENUPIC), 80, 48);

        const uint8_t soundMode = m_configurationSettings.GetSoundMode();
        renderer.Render2DPicture(egaGraph->GetTilesSize8(
            ((m_menuItemSelected == 0) && flashIcon && soundMode == 0) ? 93 :
            (soundMode == 0) ? 92 :
            (m_menuItemSelected == 0 && flashIcon)  ? 95 : 94), 88, 62);
        renderer.RenderTextLeftAligned("NO SOUND EFFECTS", egaGraph->GetFont(4), (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 96, 63);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(
            ((m_menuItemSelected == 1) && flashIcon && soundMode == 1) ? 93 :
            (soundMode == 1) ? 92 :
            (m_menuItemSelected == 1 && flashIcon) ? 95 : 94), 88, 70);
        renderer.RenderTextLeftAligned("PC SPEAKER", egaGraph->GetFont(4), (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 96, 71);
        renderer.Render2DPicture(egaGraph->GetTilesSize8(
            ((m_menuItemSelected == 2) && flashIcon && soundMode == 2) ? 93 :
            (soundMode == 2) ? 92 :
            (m_menuItemSelected == 2 && flashIcon) ? 95 : 94), 88, 78);
        renderer.RenderTextLeftAligned("ADLIB/SOUNDBLASTER", egaGraph->GetFont(4), (m_menuItemSelected == 2) ? EgaBrightRed : EgaRed, 96, 79);

        renderer.RenderTextLeftAligned("Arrows move", egaGraph->GetFont(4), EgaRed, 78, 135);
        renderer.RenderTextLeftAligned("Enter selects", egaGraph->GetFont(4), EgaRed, 163, 135);
        renderer.RenderTextCentered("Esc to back out", egaGraph->GetFont(4), EgaRed, 154, 144);
    }
    else if (m_subMenuSelected == subMenuMusic)
    {
    renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
    renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
    renderer.Render2DPicture(egaGraph->GetPicture(CP_MUSICMENUPIC), 80, 48);

    const bool musicOn = m_configurationSettings.GetMusicOn();
    renderer.Render2DPicture(egaGraph->GetTilesSize8(
        ((m_menuItemSelected == 0) && flashIcon && !musicOn) ? 93 :
        (!musicOn) ? 92 :
        (m_menuItemSelected == 0 && flashIcon) ? 95 : 94), 88, 62);
    renderer.RenderTextLeftAligned("NO MUSIC", egaGraph->GetFont(4), (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 96, 63);
    renderer.Render2DPicture(egaGraph->GetTilesSize8(
        ((m_menuItemSelected == 1) && flashIcon && musicOn) ? 93 :
        (musicOn) ? 92 :
        (m_menuItemSelected == 1 && flashIcon) ? 95 : 94), 88, 70);
    renderer.RenderTextLeftAligned("ADLIB/SOUNDBLASTER", egaGraph->GetFont(4), (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 96, 71);

    renderer.RenderTextLeftAligned("Arrows move", egaGraph->GetFont(4), EgaRed, 78, 135);
    renderer.RenderTextLeftAligned("Enter selects", egaGraph->GetFont(4), EgaRed, 163, 135);
    renderer.RenderTextCentered("Esc to back out", egaGraph->GetFont(4), EgaRed, 154, 144);
    }
    else if (m_subMenuSelected == subMenuRestoreGame)
    {
        const uint16_t maxSlotsVisible = 6;
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_LOADMENUPIC), 80, 48);

        uint8_t index = 0;
        for (auto savedGameName : m_savedGames)
        {
            if (index >= m_menuItemOffset && index < m_menuItemOffset + maxSlotsVisible)
            {
                renderer.RenderTextLeftAligned(savedGameName.c_str(), egaGraph->GetFont(4), (m_menuItemSelected == index) ? EgaBrightRed : EgaRed, 82, 64 + ((index - m_menuItemOffset) * 11));
                const bool bright = (index == m_menuItemSelected && flashIcon);
                DrawSavedGameSlot(renderer, index - m_menuItemOffset, bright);
            }
            index++;
        }

        renderer.RenderTextLeftAligned("Arrows move", egaGraph->GetFont(4), EgaRed, 78, 135);
        renderer.RenderTextLeftAligned("Enter accepts", egaGraph->GetFont(4), EgaRed, 163, 135);
        renderer.RenderTextCentered("Esc to back out", egaGraph->GetFont(4), EgaRed, 154, 144);
    }
    else if (m_subMenuSelected == subMenuSaveGame)
    {
        const uint16_t maxSlotsVisible = 6;
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_SAVEMENUPIC), 80, 48);

        if (m_menuItemOffset == 0)
        {
            if (!m_waitingForNewSaveGameName)
            {
                renderer.RenderTextCentered("Empty", egaGraph->GetFont(4), (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 154, 64);
            }
            else
            {
                const std::string saveGameName = m_newSaveGameName + "_";
                renderer.RenderTextLeftAligned(saveGameName.c_str(), egaGraph->GetFont(4), EgaBrightRed, 82, 64);
            }
        }

        uint8_t index = 1;
        for (auto savedGameName : m_savedGames)
        {
            if (index >= m_menuItemOffset && index < m_menuItemOffset + maxSlotsVisible)
            {
                renderer.RenderTextLeftAligned(savedGameName.c_str(), egaGraph->GetFont(4), (m_menuItemSelected == index) ? EgaBrightRed : EgaRed, 82, 64 + ((index - m_menuItemOffset) * 11));
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
        renderer.RenderTextLeftAligned(instructionText, egaGraph->GetFont(4), EgaRed, 78, 135);
        renderer.RenderTextLeftAligned("Enter accepts", egaGraph->GetFont(4), EgaRed, 163, 135);
        renderer.RenderTextCentered("Esc to back out", egaGraph->GetFont(4), EgaRed, 154, 144);
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