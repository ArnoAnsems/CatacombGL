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

#include "ExtraMenu.h"
#include "..\..\ThirdParty\SDL\include\SDL_keyboard.h"

const uint8_t subMenuMain = 0;
const uint8_t subMenuVideo = 1;
const uint8_t subMenuControls = 2;
const uint8_t subMenuSound = 3;
const uint8_t subMenuRestoreGame = 4;
const uint8_t subMenuSaveGame = 5;

const uint16_t browseMenuSound = 0;

ExtraMenu::ExtraMenu(ConfigurationSettings& configurationSettings, AudioPlayer& audioPlayer, std::vector<std::string>& savedGames) :
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

bool ExtraMenu::IsActive() const
{
    return m_menuActive;
}

void ExtraMenu::SetActive(bool active)
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

MenuCommand ExtraMenu::ProcessInput(const PlayerInput& playerInput)
{
    MenuCommand command = MenuCommandNone;

    const SDL_Keycode keyCode = playerInput.GetFirstKeyPressed();
    if (m_askForOverwrite)
    {
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
        if (keyCode != SDLK_UNKNOWN && m_configurationSettings.GetControlsMap().AssignActionToKey((ControlAction)(m_menuItemSelected), keyCode))
        {
            m_waitingForKeyToBind = false;
        }
        else
        {
            const uint8_t buttonCode = playerInput.GetFirstMouseButtonPressed();
            if (buttonCode != 0 && m_configurationSettings.GetControlsMap().AssignActionToMouseButton((ControlAction)(m_menuItemSelected), buttonCode))
            {
                m_waitingForKeyToBind = false;
            }
        }
    }
    else if (m_waitingForNewSaveGameName)
    {
        const uint16_t maxSaveGameNameLength = 20;
        // Check which key is pressed
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
        command = MenuCommandCloseMenu;
    }

    return command;
}

void ExtraMenu::MenuDown()
{
    if (m_menuActive)
    {
        m_audioPlayer.Play(browseMenuSound);
        if (m_subMenuSelected == subMenuMain)
        {
            if (m_menuItemSelected == 6)
            {
                m_menuItemSelected = 0;
            }
            else
            {
                m_menuItemSelected++;
            }
            if (m_menuItemSelected == 2 && !m_saveGameEnabled)
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
            if (m_menuItemSelected == 0)
            {
                m_menuItemSelected = 1;
            }
            else
            {
                m_menuItemSelected = 0;
            }
        }
        else if (m_subMenuSelected == subMenuRestoreGame)
        {
            if (m_menuItemSelected == m_savedGames.size())
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
        else if (m_subMenuSelected == subMenuSaveGame)
        {
            if (!m_askForOverwrite && !m_waitingForNewSaveGameName)  // Do not change the save game slot while typing
            {
                if (m_menuItemSelected == m_savedGames.size() + 1)
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
        }
    }
}

void ExtraMenu::MenuUp()
{
    if (m_menuActive)
    {
        m_audioPlayer.Play(browseMenuSound);
        if (m_subMenuSelected == subMenuMain)
        {
            if (m_menuItemSelected == 0)
            {
                m_menuItemSelected = 6;
            }
            else
            {
                m_menuItemSelected--;
            }
            if (m_menuItemSelected == 2 && !m_saveGameEnabled)
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
            if (m_menuItemSelected == 0)
            {
                m_menuItemSelected = 1;
            }
            else
            {
                m_menuItemSelected = 0;
            }
        }
        else if (m_subMenuSelected == subMenuRestoreGame)
        {
            if (m_menuItemSelected == 0)
            {
                m_menuItemSelected = (uint8_t)m_savedGames.size();
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
        else if (m_subMenuSelected == subMenuSaveGame)
        {
            if (!m_askForOverwrite && !m_waitingForNewSaveGameName)  // Do not change the save game slot while typing
            {
                if (m_menuItemSelected == 0)
                {
                    m_menuItemSelected = (uint8_t)m_savedGames.size() + 1;
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
        }
    }
}

void ExtraMenu::MenuLeft()
{
    if (m_menuActive)
    {
        m_audioPlayer.Play(browseMenuSound);
        if (m_subMenuSelected == subMenuVideo)
        {
            if (m_menuItemSelected == 3)
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
            if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 1)
            {
                if (m_configurationSettings.GetMouseSensitivity() > 1)
                {
                    m_configurationSettings.SetMouseSensitivity(m_configurationSettings.GetMouseSensitivity() - 1);
                }
            }
            else if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 2)
            {
                if (m_configurationSettings.GetTurnSpeed() > 100)
                {
                    m_configurationSettings.SetTurnSpeed(m_configurationSettings.GetTurnSpeed() - 10);
                }
            }
        }
    }
}

void ExtraMenu::MenuRight()
{
    if (m_menuActive)
    {
        m_audioPlayer.Play(browseMenuSound);
        if (m_subMenuSelected == subMenuVideo)
        {
            if (m_menuItemSelected == 3)
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
            if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 1)
            {
                if (m_configurationSettings.GetMouseSensitivity() < 15)
                {
                    m_configurationSettings.SetMouseSensitivity(m_configurationSettings.GetMouseSensitivity() + 1);
                }
            }
            if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 2)
            {
                if (m_configurationSettings.GetTurnSpeed() < 250)
                {
                    m_configurationSettings.SetTurnSpeed(m_configurationSettings.GetTurnSpeed() + 10);
                }
            }
        }
    }
}

MenuCommand ExtraMenu::EnterKeyPressed()
{
    MenuCommand command = MenuCommandNone;
    if (m_subMenuSelected == subMenuMain) // Main menu
    {
        if (m_menuItemSelected == 0)
        {
            // New game
            command = MenuCommandStartNewGame;
            m_menuActive = false;
        }
        else if (m_menuItemSelected == 1)
        {
            m_subMenuSelected = subMenuRestoreGame;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == 2)
        {
            m_subMenuSelected = subMenuSaveGame;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == 3)
        {
            // Video
            m_subMenuSelected = subMenuVideo;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == 4)
        {
            // Sound
            m_subMenuSelected = subMenuSound;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == 5)
        {
            // Controls
            m_subMenuSelected = subMenuControls;
            m_menuItemSelected = 0;
            m_menuItemOffset = 0;
        }
        else if (m_menuItemSelected == 6)
        {
            command = MenuCommandExitGame;
        }
    }
    else if (m_subMenuSelected == subMenuVideo)
    {
        if (m_menuItemSelected == 0)
        {
            m_subMenuSelected = 0;
            m_menuItemSelected = 3;
        }
        else if (m_menuItemSelected == 1)
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
        else if (m_menuItemSelected == 2)
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
        else if (m_menuItemSelected == 4)
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
        else if (m_menuItemSelected == 5)
        {
            m_configurationSettings.SetDepthShading(!m_configurationSettings.GetDepthShading());
        }
        else if (m_menuItemSelected == 6)
        {
            m_configurationSettings.SetShowFps(!m_configurationSettings.GetShowFps());
        }
        else if (m_menuItemSelected == 7)
        {
            m_configurationSettings.SetVSync(!m_configurationSettings.GetVSync());
        }
    }
    else if (m_subMenuSelected == subMenuControls)
    {
        if (m_menuItemSelected == 0)
        {
            // Go back to main menu
            m_subMenuSelected = 0;
            m_menuItemSelected = 5;
        }
        else if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size())
        {
            // Mouse look
            m_configurationSettings.SetMouseLook(!m_configurationSettings.GetMouseLook());
        }
        else if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 3)
        {
            // Always run
            m_configurationSettings.SetAlwaysRun(!m_configurationSettings.GetAlwaysRun());
        }
        else if (m_menuItemSelected < m_configurationSettings.GetControlsMap().GetActionLabels().size() + 1)
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
        if (m_menuItemSelected == 0)
        {
            m_subMenuSelected = 0;
            m_menuItemSelected = 4;
        }
        else if (m_menuItemSelected == 1)
        {
            if (m_configurationSettings.GetSoundMode() == 0)
            {
                m_configurationSettings.SetSoundMode(1);
            }
            else
            {
                m_configurationSettings.SetSoundMode(0);
            }
        }
    }
    else if (m_subMenuSelected == subMenuRestoreGame)
    {
        if (m_menuItemSelected == 0)
        {
            m_subMenuSelected = 0;
            m_menuItemSelected = 1;
        }
        else
        {
            m_newSaveGameName = m_savedGames.at(m_menuItemSelected - 1);
            command = MenuCommandLoadGame;
        }
    }
    else if (m_subMenuSelected == subMenuSaveGame)
    {
        if (m_menuItemSelected == 0)
        {
            m_subMenuSelected = 0;
            m_menuItemSelected = 2;
        }
        else if (m_menuItemSelected == 1)
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
            m_newSaveGameName = m_savedGames.at(m_menuItemSelected - 2);
        }
    }

    return command;
}

void ExtraMenu::Draw(IRenderer& renderer, EgaGraph* const egaGraph, const uint16_t menuCursorPic, const uint32_t timeStamp)
{
    if (m_askForOverwrite)
    {  
        renderer.RenderTextCentered("Warning!", egaGraph->GetFont(3), EgaBrightYellow, 160, 12);
        renderer.RenderTextCentered("Existing stored game", egaGraph->GetFont(3), EgaBrightYellow, 160, 32);
        renderer.RenderTextCentered(m_newSaveGameName.c_str(), egaGraph->GetFont(3), EgaBrightYellow, 160, 42);
        renderer.RenderTextCentered("will be overwritten!", egaGraph->GetFont(3), EgaBrightYellow, 160, 52);
        renderer.RenderTextCentered("Are you sure?", egaGraph->GetFont(3), EgaBrightYellow, 160, 72);
        renderer.RenderTextCentered("Y / N", egaGraph->GetFont(3), EgaBrightYellow, 160, 82);
        return;
    }
    if (m_subMenuSelected == subMenuMain)
    {
        const uint16_t xOffset = 140;
        renderer.RenderTextCentered("Main menu", egaGraph->GetFont(3), EgaBrightYellow,160,12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic),110,4+(m_menuItemSelected * 10));
        renderer.RenderTextLeftAligned("New game", egaGraph->GetFont(3), (m_menuItemSelected == 0) ? EgaBrightCyan : EgaBrightWhite,xOffset,30);
        renderer.RenderTextLeftAligned("Restore game", egaGraph->GetFont(3), (m_menuItemSelected == 1) ? EgaBrightCyan : EgaBrightWhite,xOffset,40);
        const egaColor saveGameColor = (!m_saveGameEnabled) ? EgaDarkGray : (m_menuItemSelected == 2) ? EgaBrightCyan : EgaBrightWhite;
        renderer.RenderTextLeftAligned("Save game", egaGraph->GetFont(3), saveGameColor,xOffset,50);
        renderer.RenderTextLeftAligned("Video", egaGraph->GetFont(3), (m_menuItemSelected == 3) ? EgaBrightCyan : EgaBrightWhite,xOffset,60);
        renderer.RenderTextLeftAligned("Sound", egaGraph->GetFont(3), (m_menuItemSelected == 4) ? EgaBrightCyan : EgaBrightWhite,xOffset,70);
        renderer.RenderTextLeftAligned("Controls", egaGraph->GetFont(3), (m_menuItemSelected == 5) ? EgaBrightCyan : EgaBrightWhite,xOffset,80);
        renderer.RenderTextLeftAligned("Quit", egaGraph->GetFont(3), (m_menuItemSelected == 6) ? EgaBrightCyan : EgaBrightWhite,xOffset,90);          
    }
    else if (m_subMenuSelected == subMenuVideo)
    {
        const uint16_t xOffset = 60;
        const uint16_t xOffset2 = 200;
        renderer.RenderTextCentered("Video", egaGraph->GetFont(3), EgaBrightYellow,160,12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic),30,4+(m_menuItemSelected * 10));
        renderer.RenderTextLeftAligned("Back to main menu", egaGraph->GetFont(3), (m_menuItemSelected == 0) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30);
        renderer.RenderTextLeftAligned("Screen Mode", egaGraph->GetFont(3), (m_menuItemSelected == 1) ? EgaBrightCyan : EgaBrightWhite, xOffset, 40);
        const char* screenModeStr =
            (m_configurationSettings.GetScreenMode() == Windowed) ? "Windowed" :
            (m_configurationSettings.GetScreenMode() == Fullscreen) ? "Fullscreen" :
            "Borderless";
        renderer.RenderTextLeftAligned(screenModeStr, egaGraph->GetFont(3), (m_menuItemSelected == 1) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 40);
        renderer.RenderTextLeftAligned("Aspect ratio", egaGraph->GetFont(3), (m_menuItemSelected == 2) ? EgaBrightCyan : EgaBrightWhite,xOffset,50);
        const char* aspectRatioStr = aspectRatios[m_configurationSettings.GetAspectRatio()].description.c_str();
        renderer.RenderTextLeftAligned(aspectRatioStr, egaGraph->GetFont(3), (m_menuItemSelected == 2) ? EgaBrightCyan : EgaBrightWhite,xOffset2,50);
        renderer.RenderTextLeftAligned("Field Of View (Y)", egaGraph->GetFont(3), (m_menuItemSelected == 3) ? EgaBrightCyan : EgaBrightWhite,xOffset,60);
        char fovStr[40];
        sprintf_s(fovStr, 40, "%d", m_configurationSettings.GetFov());
        renderer.RenderTextLeftAligned(fovStr, egaGraph->GetFont(3), (m_menuItemSelected == 3) ? EgaBrightCyan : EgaBrightWhite,xOffset2,60);
        renderer.RenderTextLeftAligned("Texture filtering", egaGraph->GetFont(3), (m_menuItemSelected == 4) ? EgaBrightCyan : EgaBrightWhite,xOffset,70);
        const char* textureFilterStr = (m_configurationSettings.GetTextureFilter() == IRenderer::Nearest) ? "Nearest" : "Linear";
        renderer.RenderTextLeftAligned(textureFilterStr, egaGraph->GetFont(3), (m_menuItemSelected == 4) ? EgaBrightCyan : EgaBrightWhite,xOffset2,70);
        renderer.RenderTextLeftAligned("Depth shading", egaGraph->GetFont(3), (m_menuItemSelected == 5) ? EgaBrightCyan : EgaBrightWhite,xOffset,80);
        const char* depthShadingStr = (m_configurationSettings.GetDepthShading()) ? "Enabled" : "Disabled";
        renderer.RenderTextLeftAligned(depthShadingStr, egaGraph->GetFont(3), (m_menuItemSelected == 5) ? EgaBrightCyan : EgaBrightWhite,xOffset2,80);
        renderer.RenderTextLeftAligned("Show frame rate", egaGraph->GetFont(3), (m_menuItemSelected == 6) ? EgaBrightCyan : EgaBrightWhite,xOffset,90);
        const char* showFpsStr = (m_configurationSettings.GetShowFps()) ? "Enabled" : "Disabled";
        renderer.RenderTextLeftAligned(showFpsStr, egaGraph->GetFont(3), (m_menuItemSelected == 6) ? EgaBrightCyan : EgaBrightWhite,xOffset2,90);
        const bool vsyncNotSupported = !renderer.IsVSyncSupported();
        renderer.RenderTextLeftAligned("VSync", egaGraph->GetFont(3), (vsyncNotSupported) ? EgaDarkGray : (m_menuItemSelected == 7) ? EgaBrightCyan : EgaBrightWhite, xOffset, 100);
        const char* vsyncStr = (vsyncNotSupported) ? "Not supported" : (m_configurationSettings.GetVSync()) ? "Enabled" : "Disabled";
        renderer.RenderTextLeftAligned(vsyncStr, egaGraph->GetFont(3), (vsyncNotSupported) ? EgaDarkGray : (m_menuItemSelected == 7) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 100);
    }
    else if (m_subMenuSelected == subMenuControls)
    {
        const uint16_t xOffset = 50;
        const uint16_t xOffset2 = 145;
        const uint8_t maxRowsVisible = 8;
            
        renderer.RenderTextCentered("Controls", egaGraph->GetFont(3), EgaBrightYellow,160,12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic),20,4+((m_menuItemSelected - m_menuItemOffset) * 10));
        uint16_t index = 0;
        while (index < 8)
        {
            if (index + m_menuItemOffset == 0)
            {
                renderer.RenderTextLeftAligned("Back to main menu", egaGraph->GetFont(3), (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30);
            }
            else if (index + m_menuItemOffset < (uint16_t)m_configurationSettings.GetControlsMap().GetActionLabels().size())
            {
                const std::string& actionLabel = m_configurationSettings.GetControlsMap().GetActionLabels().at((ControlAction)(index + m_menuItemOffset));
                const uint16_t yOffset = 30 + (index * 10);
                renderer.RenderTextLeftAligned(actionLabel.c_str(), egaGraph->GetFont(3), (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite,xOffset,yOffset);
                if (m_waitingForKeyToBind && m_menuItemSelected == index + m_menuItemOffset)
                {
                    renderer.RenderTextLeftAligned("< Press key to bind >", egaGraph->GetFont(3), EgaBrightCyan,xOffset2,yOffset);
                }
                else
                {
                    renderer.RenderTextLeftAligned(m_configurationSettings.GetControlsMap().GetKeyStringFromAction((ControlAction)(index + m_menuItemOffset)).c_str(), egaGraph->GetFont(3), (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite,xOffset2,yOffset);
                }
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size())
            {
                renderer.RenderTextLeftAligned("Mouse Look", egaGraph->GetFont(3), (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite,xOffset,30 + (index * 10));
                const char* mouseLookStr = (m_configurationSettings.GetMouseLook()) ? "Enabled" : "Disabled";
                renderer.RenderTextLeftAligned(mouseLookStr, egaGraph->GetFont(3), (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite,xOffset2,30 + (index * 10));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 1)
            {
                renderer.RenderTextLeftAligned("Mouse Sensitiv.", egaGraph->GetFont(3), (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite,xOffset,30 + (index * 10));
                char mouseSensitivityStr[5];
                sprintf_s(mouseSensitivityStr, 5, "%d", m_configurationSettings.GetMouseSensitivity());
                renderer.RenderTextLeftAligned(mouseSensitivityStr, egaGraph->GetFont(3), (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite,xOffset2,30 + (index * 10));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 2)
            {
                renderer.RenderTextLeftAligned("Turn Speed", egaGraph->GetFont(3), (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                char turnSpeedStr[5];
                sprintf_s(turnSpeedStr, 5, "%d", m_configurationSettings.GetTurnSpeed());
                renderer.RenderTextLeftAligned(turnSpeedStr, egaGraph->GetFont(3), (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 3)
            {
                renderer.RenderTextLeftAligned("Always Run", egaGraph->GetFont(3), (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                const char* alwaysRunStr = (m_configurationSettings.GetAlwaysRun()) ? "Enabled" : "Disabled";
                renderer.RenderTextLeftAligned(alwaysRunStr, egaGraph->GetFont(3), (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            index++;
        }
    }
    else if (m_subMenuSelected == subMenuSound)
    {
        const uint16_t xOffset = 60;
        const uint16_t xOffset2 = 200;
        renderer.RenderTextCentered("Sound", egaGraph->GetFont(3), EgaBrightYellow,160,12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic),30,4+(m_menuItemSelected * 10));
        renderer.RenderTextLeftAligned("Back to main menu", egaGraph->GetFont(3), (m_menuItemSelected == 0) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30);
        renderer.RenderTextLeftAligned("Sound Mode", egaGraph->GetFont(3), (m_menuItemSelected == 1) ? EgaBrightCyan : EgaBrightWhite,xOffset,40);
        const char* soundModeStr = (m_configurationSettings.GetSoundMode() == 0) ? "Off" : "Adlib";
        renderer.RenderTextLeftAligned(soundModeStr, egaGraph->GetFont(3), (m_menuItemSelected == 1) ? EgaBrightCyan : EgaBrightWhite,xOffset2,40);
    }
    else if (m_subMenuSelected == subMenuRestoreGame)
    {
        const uint16_t xOffset = 60;
        const uint16_t xOffset2 = 150;
        renderer.RenderTextCentered("Restore game", egaGraph->GetFont(3), EgaBrightYellow,160,12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic),30, 4 + ((m_menuItemSelected - m_menuItemOffset) * 10));

        if (m_menuItemOffset == 0)
        {
            renderer.RenderTextLeftAligned("Back to main menu", egaGraph->GetFont(3), (m_menuItemSelected == 0) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30);
        }
        uint8_t index = 1;
        for (auto savedGameName: m_savedGames)
        {
            if (index >= m_menuItemOffset && index <= m_menuItemOffset + 7)
            {
                renderer.RenderTextLeftAligned(savedGameName.c_str(), egaGraph->GetFont(3), (m_menuItemSelected == index) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + ((index - m_menuItemOffset) * 10));
            }
            index++;
        }

    }
    else if (m_subMenuSelected == subMenuSaveGame)
    {
        const uint16_t xOffset = 60;
        const uint16_t xOffset2 = 150;
        renderer.RenderTextCentered("Save game", egaGraph->GetFont(3), EgaBrightYellow, 160, 12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic), 30, 4 + ((m_menuItemSelected - m_menuItemOffset) * 10));

        if (m_menuItemOffset == 0)
        {
            renderer.RenderTextLeftAligned("Back to main menu", egaGraph->GetFont(3), (m_menuItemSelected == 0) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30);
        }

        if (m_menuItemOffset < 2)
        {
            if (!m_waitingForNewSaveGameName)
            {
                renderer.RenderTextLeftAligned("<< new saved game >>", egaGraph->GetFont(3), (m_menuItemSelected == 1) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + ((1 - m_menuItemOffset) * 10));
            }
            else
            {
                const std::string saveGameName = m_newSaveGameName + "_";
                renderer.RenderTextLeftAligned(saveGameName.c_str(), egaGraph->GetFont(3), EgaBrightCyan, xOffset, 30 + ((1 - m_menuItemOffset) * 10));
            }
        }

        uint8_t index = 2;
        for (auto savedGameName: m_savedGames)
        {
            if (index >= m_menuItemOffset && index <= m_menuItemOffset + 7)
            {
                renderer.RenderTextLeftAligned(savedGameName.c_str(), egaGraph->GetFont(3), (m_menuItemSelected == index) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + ((index - m_menuItemOffset) * 10));
            }
            index++;
        }
    }
}

void ExtraMenu::SetSaveGameEnabled(const bool enabled)
{
    m_saveGameEnabled = enabled;
}

const std::string& ExtraMenu::GetNewSaveGameName() const
{
    return m_newSaveGameName;
}

bool ExtraMenu::KeyIsSuitableForSaveGameName(const SDL_Keycode keyCode)
{
    return ((keyCode >= SDLK_a && keyCode <= SDLK_z) ||
            (keyCode >= SDLK_0 && keyCode <= SDLK_9));
}

void ExtraMenu::OpenRestoreGameMenu()
{
    m_menuActive = true;
    m_menuItemSelected = 0;
    m_subMenuSelected = subMenuRestoreGame;
    m_menuItemOffset = 0;
    m_waitingForKeyToBind = false;
    m_waitingForNewSaveGameName = false;
}

void ExtraMenu::OpenSaveGameMenu()
{
    m_menuActive = true;
    m_menuItemSelected = 0;
    m_subMenuSelected = subMenuSaveGame;
    m_menuItemOffset = 0;
    m_waitingForKeyToBind = false;
    m_waitingForNewSaveGameName = false;
}

void ExtraMenu::OpenSoundMenu()
{
    m_menuActive = true;
    m_menuItemSelected = 0;
    m_subMenuSelected = subMenuSound;
    m_menuItemOffset = 0;
    m_waitingForKeyToBind = false;
    m_waitingForNewSaveGameName = false;
}

bool ExtraMenu::IsNewSaveGameNameAlreadyInUse() const
{
    bool isInUse = false;
    for (uint16_t i = 0; i < m_savedGames.size(); i++)
    {
        isInUse |= (m_savedGames.at(i) == m_newSaveGameName);
    }

    return isInUse;
}