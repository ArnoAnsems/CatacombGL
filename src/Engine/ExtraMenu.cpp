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
#include "GuiElementBoolSelection.h"
#include "GuiElementEnumSelection.h"
#include "GuiElementIntSelection.h"
#include "GuiElementList.h"
#include "GuiElementStaticText.h"
#include "GuiElementBindKey.h"

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

ExtraMenu::ExtraMenu(
    ConfigurationSettings& configurationSettings,
    AudioPlayer& audioPlayer,
    PlayerInput& playerInput,
    EgaGraph* const egaGraph,
    const uint16_t menuCursorPic,
    std::vector<std::string>& savedGames) :
    m_menuActive(false),
    m_menuItemSelected(0),
    m_subMenuSelected(0),
    m_menuItemOffset(0),
    m_saveGameEnabled(false),
    m_configurationSettings(configurationSettings),
    m_audioPlayer(audioPlayer),
    m_savedGames(savedGames),
    m_newSaveGameName(""),
    m_waitingForNewSaveGameName(false),
    m_askForOverwrite(false),
    m_pageVideo(playerInput),
    m_pageControls(playerInput),
    m_renderableText(*egaGraph->GetFont(3)),
    m_renderableTextDefaultFont(*egaGraph->GetDefaultFont(10))
{
    GuiElementList* elementListVideo = new GuiElementList(playerInput, 8, 60, 30, 10, egaGraph->GetPicture(menuCursorPic));
    elementListVideo->AddElement(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdScreenMode), true, 132, m_renderableText));
    elementListVideo->AddElement(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdScreenResolution), true, 132, m_renderableText));
    elementListVideo->AddElement(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdAspectRatio), true, 132, m_renderableText));
    elementListVideo->AddElement(new GuiElementIntSelection(playerInput, configurationSettings.GetCVarIntMutable(CVarIdFov), true, 132, m_renderableText));
    elementListVideo->AddElement(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdTextureFilter), true, 132, m_renderableText));
    elementListVideo->AddElement(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdDepthShading), true, 132, m_renderableText));
    elementListVideo->AddElement(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdShowFpsMode), true, 132, m_renderableText));
    elementListVideo->AddElement(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdVSync), true, 132, m_renderableText));
    elementListVideo->AddElement(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdAutoMapMode), true, 132, m_renderableText));
    m_pageVideo.AddElement(elementListVideo, 60, 30);

    GuiElementStaticText* pageLabelVideo = new GuiElementStaticText(playerInput, "Video", EgaBrightYellow, m_renderableText);
    m_pageVideo.AddElement(pageLabelVideo, 160, 12);

    GuiElementList* elementListControls = new GuiElementList(playerInput, 8, 50, 30, 10, egaGraph->GetPicture(menuCursorPic));
    ControlsMap& controlsMap = configurationSettings.GetControlsMap();
    const std::map<ControlAction, std::string>& actionLabels = controlsMap.GetActionLabels();
    for (const std::pair<ControlAction, std::string>& actionLabel : actionLabels)
    {
        if (actionLabel.first != None)
        {
            elementListControls->AddElement(new GuiElementBindKey(playerInput, controlsMap, actionLabel.first, 95, m_renderableTextDefaultFont));
        }
    }
    elementListControls->AddElement(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdMouseLook), true, 95, m_renderableText));
    elementListControls->AddElement(new GuiElementIntSelection(playerInput, configurationSettings.GetCVarIntMutable(CVarIdMouseSensitivity), true, 95, m_renderableText));
    elementListControls->AddElement(new GuiElementIntSelection(playerInput, configurationSettings.GetCVarIntMutable(CVarIdTurnSpeed), true, 95, m_renderableText));
    elementListControls->AddElement(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdAlwaysRun), true, 95, m_renderableText));
    elementListControls->AddElement(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdAutoFire), true, 95, m_renderableText));
    elementListControls->AddElement(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdManaBar), true, 95, m_renderableText));

    m_pageControls.AddElement(elementListControls, 60, 30);

    GuiElementStaticText* pageLabelControls = new GuiElementStaticText(playerInput, "Controls", EgaBrightYellow, m_renderableText);
    m_pageControls.AddElement(pageLabelControls, 160, 12);
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

    if (m_waitingForNewSaveGameName)
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

    if (m_subMenuSelected == subMenuVideo)
    {
        m_pageVideo.ProcessInput();
    }
    else if (m_subMenuSelected == subMenuControls)
    {
        m_pageControls.ProcessInput();
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
    }
}

void ExtraMenu::MenuRight()
{
    if (m_menuActive)
    {
        m_audioPlayer.Play(browseMenuSound);
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
        m_renderableText.Reset();
        m_pageVideo.Draw(renderer, 0, 0, false);
        renderer.RenderText(m_renderableText);
    }
    else if (m_subMenuSelected == subMenuControls)
    {
        m_renderableText.Reset();
        m_renderableTextDefaultFont.Reset();
        m_pageControls.Draw(renderer, 0, 0, false);
        renderer.RenderText(m_renderableText);
        renderer.RenderText(m_renderableTextDefaultFont);
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
    m_waitingForNewSaveGameName = false;
}

void ExtraMenu::OpenSaveGameMenu()
{
    m_menuActive = true;
    m_menuItemSelected = 0;
    m_subMenuSelected = subMenuSaveGame;
    m_menuItemOffset = 0;
    m_waitingForNewSaveGameName = false;
}

void ExtraMenu::OpenSoundMenu()
{
    m_menuActive = true;
    m_menuItemSelected = menuItemSoundBack;
    m_subMenuSelected = subMenuSound;
    m_menuItemOffset = 0;
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

egaColor inline ExtraMenu::GetMenuItemColor(const bool selected, const bool supported)
{
    return !supported ? EgaDarkGray : selected ? EgaBrightCyan : EgaBrightWhite;
}