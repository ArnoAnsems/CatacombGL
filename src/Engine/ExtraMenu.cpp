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

const uint8_t menuItemMainStartNewGame = 0;
const uint8_t menuItemMainRestoreGame = 1;
const uint8_t menuItemMainSaveGame = 2;
const uint8_t menuItemMainVideo = 3;
const uint8_t menuItemMainSound = 4;
const uint8_t menuItemMainControls = 5;
const uint8_t menuItemMainExitGame = 6;

const uint8_t menuItemVideoBack = 0;
const uint8_t menuItemVideoScreenMode = 1;
const uint8_t menuItemVideoScreenResolution = 2;
const uint8_t menuItemVideoAspectRatio = 3;
const uint8_t menuItemVideoFov = 4;
const uint8_t menuItemVideoTextureFilter = 5;
const uint8_t menuItemVideoDepthShading = 6;
const uint8_t menuItemVideoShowFps = 7;
const uint8_t menuItemVideoVSync = 8;
const uint8_t menuItemVideoAutoMapMode = 9;

const uint8_t menuItemControlsBack = 0;

const uint8_t menuItemSoundBack = 0;
const uint8_t menuItemSoundMode = 1;

const uint16_t browseMenuSound = 0;

ExtraMenu::ExtraMenu(ConfigurationSettings& configurationSettings, AudioPlayer& audioPlayer, std::vector<std::string>& savedGames) :
    m_menuActive(false),
    m_menuItemSelected(0),
    m_subMenuSelected(0),
    m_menuItemOffset(0),
    m_waitingForKeyToBind(false),
    m_saveGameEnabled(false),
    m_configurationSettings(configurationSettings),
    m_audioPlayer(audioPlayer),
    m_savedGames(savedGames),
    m_newSaveGameName(""),
    m_waitingForNewSaveGameName(false),
    m_askForOverwrite(false)
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
            if (m_menuItemSelected == menuItemMainExitGame)
            {
                m_menuItemSelected = 0;
            }
            else
            {
                m_menuItemSelected++;
            }
            if (m_menuItemSelected == menuItemMainSaveGame && !m_saveGameEnabled)
            {
                m_menuItemSelected++;
            }
        }
        else if (m_subMenuSelected == subMenuVideo)
        {
            if (m_menuItemSelected == menuItemVideoAutoMapMode)
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
            if (m_menuItemSelected == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 5)
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
                m_menuItemSelected = menuItemSoundMode;
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
                m_menuItemSelected = menuItemMainExitGame;
            }
            else
            {
                m_menuItemSelected--;
            }
            if (m_menuItemSelected == menuItemMainSaveGame && !m_saveGameEnabled)
            {
                m_menuItemSelected--;
            }
        }
        else if (m_subMenuSelected == subMenuVideo)
        {
            if (m_menuItemSelected == 0)
            {
                m_menuItemSelected = menuItemVideoAutoMapMode;
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
                m_menuItemSelected = (uint8_t)m_configurationSettings.GetControlsMap().GetActionLabels().size() + 5;
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
                m_menuItemSelected = menuItemSoundMode;
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
            if (m_menuItemSelected == menuItemVideoFov)
            {
                m_configurationSettings.GetCVarIntMutable(CVarIdFov).Decrease();
            }
        }
        else if (m_subMenuSelected == subMenuControls)
        {
            const uint8_t numberOfActionLabels = (uint8_t)m_configurationSettings.GetControlsMap().GetActionLabels().size();
            if (m_menuItemSelected == numberOfActionLabels + 1)
            {
                m_configurationSettings.GetCVarIntMutable(CVarIdMouseSensitivity).Decrease();
            }
            else if (m_menuItemSelected == numberOfActionLabels + 2)
            {
                m_configurationSettings.GetCVarIntMutable(CVarIdTurnSpeed).Decrease();
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
            if (m_menuItemSelected == menuItemVideoFov)
            {
                m_configurationSettings.GetCVarIntMutable(CVarIdFov).Increase();
            }
        }
        else if (m_subMenuSelected == subMenuControls)
        {
            const uint8_t numberOfActionLabels = (uint8_t)m_configurationSettings.GetControlsMap().GetActionLabels().size();
            if (m_menuItemSelected == numberOfActionLabels + 1)
            {
                m_configurationSettings.GetCVarIntMutable(CVarIdMouseSensitivity).Increase();
            }
            if (m_menuItemSelected == numberOfActionLabels + 2)
            {
                m_configurationSettings.GetCVarIntMutable(CVarIdTurnSpeed).Increase();
            }
        }
    }
}

MenuCommand ExtraMenu::EnterKeyPressed()
{
    MenuCommand command = MenuCommandNone;
    if (m_subMenuSelected == subMenuMain) // Main menu
    {
        if (m_menuItemSelected == menuItemMainStartNewGame)
        {
            // New game
            command = MenuCommandStartNewGame;
            m_menuActive = false;
        }
        else if (m_menuItemSelected == menuItemMainRestoreGame)
        {
            m_subMenuSelected = subMenuRestoreGame;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == menuItemMainSaveGame)
        {
            m_subMenuSelected = subMenuSaveGame;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == menuItemMainVideo)
        {
            // Video
            m_subMenuSelected = subMenuVideo;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == menuItemMainSound)
        {
            // Sound
            m_subMenuSelected = subMenuSound;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == menuItemMainControls)
        {
            // Controls
            m_subMenuSelected = subMenuControls;
            m_menuItemSelected = 0;
            m_menuItemOffset = 0;
        }
        else if (m_menuItemSelected == menuItemMainExitGame)
        {
            command = MenuCommandExitGame;
        }
    }
    else if (m_subMenuSelected == subMenuVideo)
    {
        if (m_menuItemSelected == menuItemVideoBack)
        {
            m_subMenuSelected = subMenuMain;
            m_menuItemSelected = menuItemMainVideo;
        }
        else if (m_menuItemSelected == menuItemVideoScreenMode)
        {
            m_configurationSettings.GetCVarEnumMutable(CVarIdScreenMode).Next();
        }
        else if (m_menuItemSelected == menuItemVideoScreenResolution)
        {
            m_configurationSettings.GetCVarEnumMutable(CVarIdScreenResolution).Next();
        }
        else if (m_menuItemSelected == menuItemVideoAspectRatio)
        {
            m_configurationSettings.GetCVarEnumMutable(CVarIdAspectRatio).Next();
        }
        else if (m_menuItemSelected == menuItemVideoTextureFilter)
        {
            m_configurationSettings.GetCVarEnumMutable(CVarIdTextureFilter).Next();
        }
        else if (m_menuItemSelected == menuItemVideoDepthShading)
        {
            m_configurationSettings.GetCVarBoolMutable(CVarIdDepthShading).Toggle();
        }
        else if (m_menuItemSelected == menuItemVideoShowFps)
        {
            m_configurationSettings.GetCVarEnumMutable(CVarIdShowFpsMode).Next();
        }
        else if (m_menuItemSelected == menuItemVideoVSync)
        {
            m_configurationSettings.GetCVarBoolMutable(CVarIdVSync).Toggle();
        }
        else if (m_menuItemSelected == menuItemVideoAutoMapMode)
        {
            m_configurationSettings.GetCVarEnumMutable(CVarIdAutoMapMode).Next();
        }
    }
    else if (m_subMenuSelected == subMenuControls)
    {
        const uint8_t numberOfActionLabels = (uint8_t)m_configurationSettings.GetControlsMap().GetActionLabels().size();
        if (m_menuItemSelected == menuItemControlsBack)
        {
            // Go back to main menu
            m_subMenuSelected = 0;
            m_menuItemSelected = 5;
        }
        else if (m_menuItemSelected == numberOfActionLabels)
        {
            // Mouse look
            m_configurationSettings.GetCVarBoolMutable(CVarIdMouseLook).Toggle();
        }
        else if (m_menuItemSelected == numberOfActionLabels + 3)
        {
            // Always run
            m_configurationSettings.GetCVarBoolMutable(CVarIdAlwaysRun).Toggle();
        }
        else if (m_menuItemSelected == numberOfActionLabels + 4)
        {
            // Auto fire
            m_configurationSettings.GetCVarBoolMutable(CVarIdAutoFire).Toggle();
        }
        else if (m_menuItemSelected == numberOfActionLabels + 5)
        {
            // Mana bar
            m_configurationSettings.GetCVarBoolMutable(CVarIdManaBar).Toggle();
        }
        else if (m_menuItemSelected < numberOfActionLabels + 1)
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
        if (m_menuItemSelected == menuItemSoundBack)
        {
            m_subMenuSelected = subMenuMain;
            m_menuItemSelected = menuItemMainSound;
        }
        else if (m_menuItemSelected == menuItemSoundMode)
        {
            m_configurationSettings.GetCVarEnumMutable(CVarIdSoundMode).Next();
        }
    }
    else if (m_subMenuSelected == subMenuRestoreGame)
    {
        if (m_menuItemSelected == 0)
        {
            m_subMenuSelected = subMenuMain;
            m_menuItemSelected = menuItemMainRestoreGame;
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
            m_subMenuSelected = subMenuMain;
            m_menuItemSelected = menuItemMainSaveGame;
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
        renderableText.LeftAligned("New game", GetMenuItemColor(m_menuItemSelected == menuItemMainStartNewGame, true), xOffset, 30);
        renderableText.LeftAligned("Restore game", GetMenuItemColor(m_menuItemSelected == menuItemMainRestoreGame, true), xOffset, 40);
        renderableText.LeftAligned("Save game", GetMenuItemColor(m_menuItemSelected == menuItemMainSaveGame, m_saveGameEnabled), xOffset, 50);
        renderableText.LeftAligned("Video", GetMenuItemColor(m_menuItemSelected == menuItemMainVideo, true), xOffset, 60);
        renderableText.LeftAligned("Sound", GetMenuItemColor(m_menuItemSelected == menuItemMainSound, true) ,xOffset, 70);
        renderableText.LeftAligned("Controls", GetMenuItemColor(m_menuItemSelected == menuItemMainControls, true), xOffset, 80);
        renderableText.LeftAligned("Quit", GetMenuItemColor(m_menuItemSelected == menuItemMainExitGame, true), xOffset, 90);
        renderer.RenderText(renderableText);
    }
    else if (m_subMenuSelected == subMenuVideo)
    {
        const uint16_t xOffset = 60;
        const uint16_t xOffset2 = 192;
        RenderableText renderableText(*egaGraph->GetFont(3));
        renderableText.Centered("Video", EgaBrightYellow,160,12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic), 20, 4 + ((m_menuItemSelected - m_menuItemOffset) * 10));
        uint16_t index = 0;
        while (index < 8)
        {
            const int16_t offsetY = 30 + (index * 10);
            const uint8_t itemIndex = index + m_menuItemOffset;
            const bool itemSelected = m_menuItemSelected == itemIndex;
            if (itemIndex == menuItemVideoBack)
            {
                renderableText.LeftAligned("Back to main menu", itemSelected ? EgaBrightCyan : EgaBrightWhite, xOffset, offsetY);
            }
            else if (itemIndex == menuItemVideoScreenMode)
            {
                DrawMenuItemEnum(CVarIdScreenMode, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == menuItemVideoScreenResolution)
            {
                const bool screenResolutionSupported = renderer.IsOriginalScreenResolutionSupported();
                DrawMenuItemEnum(CVarIdScreenResolution, itemSelected, screenResolutionSupported, xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == menuItemVideoAspectRatio)
            {
                DrawMenuItemEnum(CVarIdAspectRatio, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == menuItemVideoFov)
            {
                DrawMenuItemInt(CVarIdFov, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == menuItemVideoTextureFilter)
            {
                DrawMenuItemEnum(CVarIdTextureFilter, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == menuItemVideoDepthShading)
            {
                DrawMenuItemBool(CVarIdDepthShading, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == menuItemVideoShowFps)
            {
                DrawMenuItemEnum(CVarIdShowFpsMode, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == menuItemVideoVSync)
            {
                DrawMenuItemBool(CVarIdVSync, itemSelected, renderer.IsVSyncSupported(), xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == menuItemVideoAutoMapMode)
            {
                DrawMenuItemEnum(CVarIdAutoMapMode, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
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
        uint8_t index = 0;
        while (index < 8)
        {
            const uint16_t offsetY = 30 + (index * 10);
            const uint8_t itemIndex = index + m_menuItemOffset;
            const bool itemSelected = m_menuItemSelected == itemIndex;
            if (itemIndex == menuItemControlsBack)
            {
                renderableText.LeftAligned("Back to main menu", itemSelected ? EgaBrightCyan : EgaBrightWhite, xOffset, offsetY);
            }
            else if (itemIndex < (uint16_t)m_configurationSettings.GetControlsMap().GetActionLabels().size())
            {
                const std::string& actionLabel = m_configurationSettings.GetControlsMap().GetActionLabels().at((ControlAction)(itemIndex));
                renderableText.LeftAligned(actionLabel, itemSelected ? EgaBrightCyan : EgaBrightWhite,xOffset, offsetY);
                if (m_waitingForKeyToBind && m_menuItemSelected == itemIndex)
                {
                    renderableText.LeftAligned("< Press key to bind >", EgaBrightCyan,xOffset2, offsetY);
                }
                else
                {
                    // The name of the keys is shown with the default font, as the original font from the game lacks some required characters.
                    renderableTextDefaultFont.LeftAligned(m_configurationSettings.GetControlsMap().GetKeyStringFromAction((ControlAction)(itemIndex)), itemSelected ? EgaBrightCyan : EgaBrightWhite,xOffset2, offsetY);
                }
            }
            else if (itemIndex == m_configurationSettings.GetControlsMap().GetActionLabels().size())
            {
                DrawMenuItemBool(CVarIdMouseLook, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 1)
            {
                DrawMenuItemInt(CVarIdMouseSensitivity, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 2)
            {
                DrawMenuItemInt(CVarIdTurnSpeed, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 3)
            {
                DrawMenuItemBool(CVarIdAlwaysRun, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 4)
            {
                DrawMenuItemBool(CVarIdAutoFire, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
            }
            else if (itemIndex == m_configurationSettings.GetControlsMap().GetActionLabels().size() + 5)
            {
                DrawMenuItemBool(CVarIdManaBar, itemSelected, true, xOffset, xOffset2, offsetY, renderableText);
            }
            index++;
        }
        renderer.RenderText(renderableText);
        renderer.RenderText(renderableTextDefaultFont);
    }
    else if (m_subMenuSelected == subMenuSound)
    {
        const ConsoleVariableEnum& cvar = m_configurationSettings.GetCVarEnum(CVarIdSoundMode);
        const uint16_t xOffset = 60;
        const uint16_t xOffset2 = 200;
        RenderableText renderableText(*egaGraph->GetFont(3));
        renderableText.Centered("Sound", EgaBrightYellow,160,12);
        renderer.Render2DPicture(egaGraph->GetPicture(menuCursorPic),30,4+(m_menuItemSelected * 10));
        renderableText.LeftAligned("Back to main menu", (m_menuItemSelected == menuItemSoundBack) ? EgaBrightCyan : EgaBrightWhite, xOffset, 30);
        DrawMenuItemEnum(CVarIdSoundMode, m_menuItemSelected == menuItemSoundMode, true, xOffset, xOffset2, 40, renderableText);
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
    m_menuItemSelected = menuItemSoundBack;
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

void ExtraMenu::DrawMenuItemBool(
    const uint8_t cvarId,
    const bool selected,
    const bool supported,
    const int16_t offsetXName,
    const int16_t offsetXValue,
    const int16_t offsetY,
    RenderableText& renderableText)
{
    const ConsoleVariableBool& cvar = m_configurationSettings.GetCVarBool(cvarId);
    const egaColor color = GetMenuItemColor(selected, supported);
    renderableText.LeftAligned(cvar.GetNameInMenu(), color, offsetXName, offsetY);
    const std::string& valueStr = (!supported) ? "Not supported" : cvar.GetValueInMenu();
    renderableText.LeftAligned(valueStr, color, offsetXValue, offsetY);
}

void ExtraMenu::DrawMenuItemEnum(
    const uint8_t cvarId,
    const bool selected,
    const bool supported,
    const int16_t offsetXName,
    const int16_t offsetXValue,
    const int16_t offsetY,
    RenderableText& renderableText)
{
    const ConsoleVariableEnum& cvar = m_configurationSettings.GetCVarEnum(cvarId);
    const egaColor color = GetMenuItemColor(selected, supported);
    renderableText.LeftAligned(cvar.GetNameInMenu(), color, offsetXName, offsetY);
    const std::string& valueStr = (!supported) ? "Not supported" : cvar.GetValueInMenu();
    renderableText.LeftAligned(valueStr, color, offsetXValue, offsetY);
}

void ExtraMenu::DrawMenuItemInt(
    const uint8_t cvarId,
    const bool selected,
    const bool supported,
    const int16_t offsetXName,
    const int16_t offsetXValue,
    const int16_t offsetY,
    RenderableText& renderableText)
{
    const ConsoleVariableInt& cvar = m_configurationSettings.GetCVarInt(cvarId);
    const egaColor color = GetMenuItemColor(selected, supported);
    renderableText.LeftAligned(cvar.GetNameInMenu(), color, offsetXName, offsetY);
    const std::string& valueStr = (!supported) ? "Not supported" : std::to_string(cvar.GetValue());
    renderableText.LeftAligned(valueStr, color, offsetXValue, offsetY);
}

egaColor inline ExtraMenu::GetMenuItemColor(const bool selected, const bool supported)
{
    return !supported ? EgaDarkGray : selected ? EgaBrightCyan : EgaBrightWhite;
}