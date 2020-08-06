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
#include "DefaultFont.h"

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
        command = MenuCommandCloseMenu;
    }
    
    if (playerInput.IsKeyJustPressed(SDLK_RETURN))
    {
        command = EnterKeyPressed();
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
            if (m_menuItemSelected == 8)
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
        else if (m_subMenuSelected == subMenuControls)
        {
            if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 4)
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
                m_menuItemSelected = 8;
                m_menuItemOffset = m_menuItemSelected - 7;
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
        else if (m_subMenuSelected == subMenuControls)
        {
            if (m_menuItemSelected == 0)
            {
                m_menuItemSelected = (uint8_t)m_configurationSettings.GetControlsMap().GetActionLabels().size() + 4;
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
                if (m_configurationSettings.GetMouseSensitivity() < 20)
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
            const ShowFpsMode previousShowFpsMode = m_configurationSettings.GetShowFps();
            const ShowFpsMode nextShowFpsMode =
                (previousShowFpsMode == Off) ? Minimal :
                (previousShowFpsMode == Minimal) ? Extended :
                Off;
            m_configurationSettings.SetShowFps(nextShowFpsMode);
        }
        else if (m_menuItemSelected == 7)
        {
            m_configurationSettings.SetVSync(!m_configurationSettings.GetVSync());
        }
        else if (m_menuItemSelected == 8)
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
        else if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 4)
        {
            // Auto fire
            m_configurationSettings.SetAutoFire(!m_configurationSettings.GetAutoFire());
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
            else if (m_configurationSettings.GetSoundMode() == 1)
            {
                m_configurationSettings.SetSoundMode(2);
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
        RenderableText renderableText(*egaGraph->GetFont(3));
        renderableText.Centered("Warning!", EgaBrightYellow, 160, 12);
        renderableText.Centered("Existing stored game", EgaBrightYellow, 160, 32);
        renderableText.Centered(m_newSaveGameName, EgaBrightYellow, 160, 42);
        renderableText.Centered("will be overwritten!", EgaBrightYellow, 160, 52);
        renderableText.Centered("Are you sure?", EgaBrightYellow, 160, 72);
        renderableText.Centered("Y / N", EgaBrightYellow, 160, 82);
        renderer.RenderText(renderableText);
        return;
    }
    if (m_subMenuSelected == subMenuMain)
    {
        const uint16_t xOffset = 140;
        RenderableText renderableText(*egaGraph->GetFont(3));
        renderableText.Centered("Main menu", EgaBrightYellow,160,12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic),110,4+(m_menuItemSelected * 10));
        renderableText.LeftAligned("New game", (m_menuItemSelected == 0) ? EgaBrightCyan : EgaBrightWhite,xOffset,30);
        renderableText.LeftAligned("Restore game", (m_menuItemSelected == 1) ? EgaBrightCyan : EgaBrightWhite,xOffset,40);
        const egaColor saveGameColor = (!m_saveGameEnabled) ? EgaDarkGray : (m_menuItemSelected == 2) ? EgaBrightCyan : EgaBrightWhite;
        renderableText.LeftAligned("Save game", saveGameColor,xOffset,50);
        renderableText.LeftAligned("Video", (m_menuItemSelected == 3) ? EgaBrightCyan : EgaBrightWhite,xOffset,60);
        renderableText.LeftAligned("Sound", (m_menuItemSelected == 4) ? EgaBrightCyan : EgaBrightWhite,xOffset,70);
        renderableText.LeftAligned("Controls", (m_menuItemSelected == 5) ? EgaBrightCyan : EgaBrightWhite,xOffset,80);
        renderableText.LeftAligned("Quit", (m_menuItemSelected == 6) ? EgaBrightCyan : EgaBrightWhite,xOffset,90);
        renderer.RenderText(renderableText);
    }
    else if (m_subMenuSelected == subMenuVideo)
    {
        const uint16_t xOffset = 60;
        const uint16_t xOffset2 = 200;
        RenderableText renderableText(*egaGraph->GetFont(3));
        renderableText.Centered("Video", EgaBrightYellow,160,12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic), 20, 4 + ((m_menuItemSelected - m_menuItemOffset) * 10));
        uint16_t index = 0;
        while (index < 8)
        {
            if (index + m_menuItemOffset == 0)
            {
                renderableText.LeftAligned("Back to main menu", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
            }
            else if (index + m_menuItemOffset == 1)
            {
                renderableText.LeftAligned("Screen Mode", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                const char* screenModeStr =
                    (m_configurationSettings.GetScreenMode() == Windowed) ? "Windowed" :
                    (m_configurationSettings.GetScreenMode() == Fullscreen) ? "Fullscreen" :
                    "Borderless";
                renderableText.LeftAligned(screenModeStr, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            else if (index + m_menuItemOffset == 2)
            {
                renderableText.LeftAligned("Aspect ratio", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                const std::string& aspectRatioStr = aspectRatios[m_configurationSettings.GetAspectRatio()].description;
                renderableText.LeftAligned(aspectRatioStr, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            else if (index + m_menuItemOffset == 3)
            {
                renderableText.LeftAligned("Field Of View (Y)", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                char fovStr[40];
                sprintf_s(fovStr, 40, "%d", m_configurationSettings.GetFov());
                renderableText.LeftAligned(fovStr, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            else if (index + m_menuItemOffset == 4)
            {
                renderableText.LeftAligned("Texture filtering", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                const char* textureFilterStr = (m_configurationSettings.GetTextureFilter() == IRenderer::Nearest) ? "Nearest" : "Linear";
                renderableText.LeftAligned(textureFilterStr, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            else if (index + m_menuItemOffset == 5)
            {
                renderableText.LeftAligned("Depth shading", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                const char* depthShadingStr = (m_configurationSettings.GetDepthShading()) ? "Enabled" : "Disabled";
                renderableText.LeftAligned(depthShadingStr, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            else if (index + m_menuItemOffset == 6)
            {
                renderableText.LeftAligned("Show frame rate", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                const ShowFpsMode showFpsMode = m_configurationSettings.GetShowFps();
                const char* showFpsStr =
                    (showFpsMode == Minimal) ? "Minimal" :
                    (showFpsMode == Extended) ? "Extended" :
                    "Off";
                renderableText.LeftAligned(showFpsStr, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            else if (index + m_menuItemOffset == 7)
            {
                const bool vsyncNotSupported = !renderer.IsVSyncSupported();
                renderableText.LeftAligned("VSync", (vsyncNotSupported) ? EgaDarkGray : (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                const char* vsyncStr = (vsyncNotSupported) ? "Not supported" : (m_configurationSettings.GetVSync()) ? "Enabled" : "Disabled";
                renderableText.LeftAligned(vsyncStr, (vsyncNotSupported) ? EgaDarkGray : (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            else if (index + m_menuItemOffset == 8)
            {
                renderableText.LeftAligned("Overhead Map", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                const OverheadMapMode overheadMapMode = m_configurationSettings.GetOverHeadMapMode();
                const char* overheadMapModeStr =
                    (overheadMapMode == Classic) ? "Classic" :
                    (overheadMapMode == Isometric) ? "Isometric" :
                    "Top down";
                renderableText.LeftAligned(overheadMapModeStr, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            index++;
        }
        renderer.RenderText(renderableText);
    }
    else if (m_subMenuSelected == subMenuControls)
    {
        const uint16_t xOffset = 50;
        const uint16_t xOffset2 = 145;
        const uint8_t maxRowsVisible = 8;
        RenderableText renderableText(*egaGraph->GetFont(3));
        RenderableText renderableTextDefaultFont(*DefaultFont::Get(renderer, 10));
        renderableText.Centered("Controls", EgaBrightYellow,160,12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic),20,4+((m_menuItemSelected - m_menuItemOffset) * 10));
        uint16_t index = 0;
        while (index < 8)
        {
            if (index + m_menuItemOffset == 0)
            {
                renderableText.LeftAligned("Back to main menu", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30);
            }
            else if (index + m_menuItemOffset < (uint16_t)m_configurationSettings.GetControlsMap().GetActionLabels().size())
            {
                const std::string& actionLabel = m_configurationSettings.GetControlsMap().GetActionLabels().at((ControlAction)(index + m_menuItemOffset));
                const uint16_t yOffset = 30 + (index * 10);
                renderableText.LeftAligned(actionLabel, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite,xOffset,yOffset);
                if (m_waitingForKeyToBind && m_menuItemSelected == index + m_menuItemOffset)
                {
                    renderableText.LeftAligned("< Press key to bind >", EgaBrightCyan,xOffset2,yOffset);
                }
                else
                {
                    // The name of the keys is shown with the default font, as the original font from the game lacks some required characters.
                    renderableTextDefaultFont.LeftAligned(m_configurationSettings.GetControlsMap().GetKeyStringFromAction((ControlAction)(index + m_menuItemOffset)), (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite,xOffset2,yOffset);
                }
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size())
            {
                renderableText.LeftAligned("Mouse Look", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite,xOffset,30 + (index * 10));
                const char* mouseLookStr = (m_configurationSettings.GetMouseLook()) ? "Enabled" : "Disabled";
                renderableText.LeftAligned(mouseLookStr, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite,xOffset2,30 + (index * 10));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 1)
            {
                renderableText.LeftAligned("Mouse Sensitiv.", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite,xOffset,30 + (index * 10));
                char mouseSensitivityStr[5];
                sprintf_s(mouseSensitivityStr, 5, "%d", m_configurationSettings.GetMouseSensitivity());
                renderableText.LeftAligned(mouseSensitivityStr, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite,xOffset2,30 + (index * 10));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 2)
            {
                renderableText.LeftAligned("Turn Speed", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                char turnSpeedStr[5];
                sprintf_s(turnSpeedStr, 5, "%d", m_configurationSettings.GetTurnSpeed());
                renderableText.LeftAligned(turnSpeedStr, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 3)
            {
                renderableText.LeftAligned("Always Run", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                const char* alwaysRunStr = (m_configurationSettings.GetAlwaysRun()) ? "Enabled" : "Disabled";
                renderableText.LeftAligned(alwaysRunStr, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            else if (index + m_menuItemOffset == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 4)
            {
                renderableText.LeftAligned("Auto Fire", (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + (index * 10));
                const char* autoFireStr = (m_configurationSettings.GetAutoFire()) ? "Enabled" : "Disabled";
                renderableText.LeftAligned(autoFireStr, (m_menuItemSelected == index + m_menuItemOffset) ? EgaBrightCyan : EgaBrightWhite, xOffset2, 30 + (index * 10));
            }
            index++;
        }
        renderer.RenderText(renderableText);
        renderer.RenderText(renderableTextDefaultFont);
    }
    else if (m_subMenuSelected == subMenuSound)
    {
        const uint16_t xOffset = 60;
        const uint16_t xOffset2 = 200;
        RenderableText renderableText(*egaGraph->GetFont(3));
        renderableText.Centered("Sound", EgaBrightYellow,160,12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic),30,4+(m_menuItemSelected * 10));
        renderableText.LeftAligned("Back to main menu", (m_menuItemSelected == 0) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30);
        renderableText.LeftAligned("Sound Mode", (m_menuItemSelected == 1) ? EgaBrightCyan : EgaBrightWhite,xOffset,40);
        const char* soundModeStr = (m_configurationSettings.GetSoundMode() == 0) ? "Off" : (m_configurationSettings.GetSoundMode() == 1) ? "PC Speaker" : "Adlib";
        renderableText.LeftAligned(soundModeStr, (m_menuItemSelected == 1) ? EgaBrightCyan : EgaBrightWhite,xOffset2,40);
        renderer.RenderText(renderableText);
    }
    else if (m_subMenuSelected == subMenuRestoreGame)
    {
        const uint16_t xOffset = 60;
        const uint16_t xOffset2 = 150;
        RenderableText renderableText(*egaGraph->GetFont(3));
        renderableText.Centered("Restore game", EgaBrightYellow,160,12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic),30, 4 + ((m_menuItemSelected - m_menuItemOffset) * 10));

        if (m_menuItemOffset == 0)
        {
            renderableText.LeftAligned("Back to main menu", (m_menuItemSelected == 0) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30);
        }
        uint8_t index = 1;
        for (auto savedGameName: m_savedGames)
        {
            if (index >= m_menuItemOffset && index <= m_menuItemOffset + 7)
            {
                renderableText.LeftAligned(savedGameName, (m_menuItemSelected == index) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + ((index - m_menuItemOffset) * 10));
            }
            index++;
        }
        renderer.RenderText(renderableText);
    }
    else if (m_subMenuSelected == subMenuSaveGame)
    {
        const uint16_t xOffset = 60;
        const uint16_t xOffset2 = 150;
        RenderableText renderableText(*egaGraph->GetFont(3));
        renderableText.Centered("Save game", EgaBrightYellow, 160, 12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic), 30, 4 + ((m_menuItemSelected - m_menuItemOffset) * 10));

        if (m_menuItemOffset == 0)
        {
            renderableText.LeftAligned("Back to main menu", (m_menuItemSelected == 0) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30);
        }

        if (m_menuItemOffset < 2)
        {
            if (!m_waitingForNewSaveGameName)
            {
                renderableText.LeftAligned("<< new saved game >>", (m_menuItemSelected == 1) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + ((1 - m_menuItemOffset) * 10));
            }
            else
            {
                const std::string saveGameName = m_newSaveGameName + "_";
                renderableText.LeftAligned(saveGameName, EgaBrightCyan, xOffset, 30 + ((1 - m_menuItemOffset) * 10));
            }
        }

        uint8_t index = 2;
        for (auto savedGameName: m_savedGames)
        {
            if (index >= m_menuItemOffset && index <= m_menuItemOffset + 7)
            {
                renderableText.LeftAligned(savedGameName, (m_menuItemSelected == index) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30 + ((index - m_menuItemOffset) * 10));
            }
            index++;
        }
        renderer.RenderText(renderableText);
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

void ExtraMenu::CheckHighScore(const uint16_t level, const uint32_t score)
{
    // Not applicable
}