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
#include "GuiElementButton.h"
#include "GuiElementEditText.h"

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

const uint16_t browseMenuSound = 0;

const int16_t restoreGameListId = 1;
const int16_t saveGameListId = 2;

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
    m_askForOverwrite(false),
    m_pageVideo(playerInput),
    m_pageControls(playerInput),
    m_pageSound(playerInput),
    m_pageRestoreGame(playerInput),
    m_pageSaveGame(playerInput),
    m_renderableText(*egaGraph->GetFont(3)),
    m_renderableTextDefaultFont(*egaGraph->GetDefaultFont(10))
{
    // Video menu
    GuiElementList* elementListVideo = new GuiElementList(playerInput, 8, 60, 30, 10, egaGraph->GetPicture(menuCursorPic));
    elementListVideo->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdScreenMode), true, 132, m_renderableText));
    elementListVideo->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdScreenResolution), true, 132, m_renderableText));
    elementListVideo->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdAspectRatio), true, 132, m_renderableText));
    elementListVideo->AddChild(new GuiElementIntSelection(playerInput, configurationSettings.GetCVarIntMutable(CVarIdFov), true, 132, m_renderableText));
    elementListVideo->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdTextureFilter), true, 132, m_renderableText));
    elementListVideo->AddChild(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdDepthShading), true, 132, m_renderableText));
    elementListVideo->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdShowFpsMode), true, 132, m_renderableText));
    elementListVideo->AddChild(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdVSync), true, 132, m_renderableText));
    elementListVideo->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdAutoMapMode), true, 132, m_renderableText));
    m_pageVideo.AddChild(elementListVideo, 60, 30);

    GuiElementStaticText* pageLabelVideo = new GuiElementStaticText(playerInput, "Video", EgaBrightYellow, m_renderableText);
    m_pageVideo.AddChild(pageLabelVideo, 160, 12);

    // Controls menu
    GuiElementList* elementListControls = new GuiElementList(playerInput, 8, 50, 30, 10, egaGraph->GetPicture(menuCursorPic));
    ControlsMap& controlsMap = configurationSettings.GetControlsMap();
    const std::map<ControlAction, std::string>& actionLabels = controlsMap.GetActionLabels();
    for (const std::pair<ControlAction, std::string>& actionLabel : actionLabels)
    {
        if (actionLabel.first != None)
        {
            elementListControls->AddChild(new GuiElementBindKey(playerInput, controlsMap, actionLabel.first, 95, m_renderableTextDefaultFont));
        }
    }
    elementListControls->AddChild(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdMouseLook), true, 95, m_renderableText));
    elementListControls->AddChild(new GuiElementIntSelection(playerInput, configurationSettings.GetCVarIntMutable(CVarIdMouseSensitivity), true, 95, m_renderableText));
    elementListControls->AddChild(new GuiElementIntSelection(playerInput, configurationSettings.GetCVarIntMutable(CVarIdTurnSpeed), true, 95, m_renderableText));
    elementListControls->AddChild(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdAlwaysRun), true, 95, m_renderableText));
    elementListControls->AddChild(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdAutoFire), true, 95, m_renderableText));
    elementListControls->AddChild(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdManaBar), true, 95, m_renderableText));

    m_pageControls.AddChild(elementListControls, 60, 30);

    GuiElementStaticText* pageLabelControls = new GuiElementStaticText(playerInput, "Controls", EgaBrightYellow, m_renderableText);
    m_pageControls.AddChild(pageLabelControls, 160, 12);

    // Sound menu
    GuiElementList* elementListSound = new GuiElementList(playerInput, 8, 60, 30, 10, egaGraph->GetPicture(menuCursorPic));
    elementListSound->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdSoundMode), true, 140, m_renderableText));
    m_pageSound.AddChild(elementListSound, 60, 30);

    GuiElementStaticText* pageLabelSound = new GuiElementStaticText(playerInput, "Sound", EgaBrightYellow, m_renderableText);
    m_pageSound.AddChild(pageLabelSound, 160, 12);

    // Restore game menu
    GuiElementList* elementListRestoreGame = new GuiElementList(playerInput, 8, 60, 30, 10, egaGraph->GetPicture(menuCursorPic));
    elementListRestoreGame->SetId(restoreGameListId);
    if (savedGames.size() > 0)
    {
        int16_t savedGameIndex = 0;
        for (const std::string& savedGame : savedGames)
        {
            elementListRestoreGame->AddChild(new GuiElementButton(playerInput, savedGame, { GuiActionRestoreGame, savedGameIndex }, m_renderableText));
            savedGameIndex++;
        }
        m_pageRestoreGame.AddChild(elementListRestoreGame, 60, 30);
    }

    GuiElementStaticText* pageLabelRestoreGame = new GuiElementStaticText(playerInput, "Restore game", EgaBrightYellow, m_renderableText);
    m_pageRestoreGame.AddChild(pageLabelRestoreGame, 160, 12);

    // Save game menu
    GuiElementList* elementListSaveGame = new GuiElementList(playerInput, 8, 60, 30, 10, egaGraph->GetPicture(menuCursorPic));
    GuiElementEditText* saveGameEditText = new GuiElementEditText(playerInput, m_newSaveGameName, "<< new saved game >>", 20, m_renderableText, GuiEvent({GuiActionSaveGame, -1}));
    elementListSaveGame->SetId(saveGameListId);
    elementListSaveGame->AddChild(saveGameEditText);
    
    if (savedGames.size() > 0)
    {
        int16_t savedGameIndex = 0;
        for (const std::string& savedGame : savedGames)
        {
            elementListSaveGame->AddChild(new GuiElementButton(playerInput, savedGame, { GuiActionSaveGame, savedGameIndex }, m_renderableText));
            savedGameIndex++;
        }
        m_pageSaveGame.AddChild(elementListSaveGame, 60, 30);
    }

    GuiElementStaticText* pageLabelSaveGame = new GuiElementStaticText(playerInput, "Save game", EgaBrightYellow, m_renderableText);
    m_pageSaveGame.AddChild(pageLabelSaveGame, 160, 12);
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
            m_subMenuSelected = subMenuMain;
            m_menuItemSelected = 0;
        }
        else if (keyCode != SDLK_UNKNOWN)
        {
            m_askForOverwrite = false;
        }
    }

    if (playerInput.IsKeyJustPressed(SDLK_UP))
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
    else if (m_subMenuSelected == subMenuSound)
    {
        m_pageSound.ProcessInput();
    }
    else if (m_subMenuSelected == subMenuRestoreGame)
    {
        const GuiEvent& guiEvent = m_pageRestoreGame.ProcessInput();
        if (guiEvent.guiAction == GuiActionRestoreGame)
        {
            command = MenuCommandLoadGame;
            m_newSaveGameName = m_savedGames.at(guiEvent.guiParameter);
            m_subMenuSelected = subMenuMain;
            m_menuItemSelected = 0;
        }
    }
    else if (m_subMenuSelected == subMenuSaveGame)
    {
        const GuiEvent& guiEvent = m_pageSaveGame.ProcessInput();
        if (guiEvent.guiAction == GuiActionSaveGame)
        {
            m_newSaveGameName = (guiEvent.guiParameter == -1) ? m_newSaveGameName : m_savedGames.at(guiEvent.guiParameter);
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
                m_subMenuSelected = subMenuMain;
                m_menuItemSelected = 0;
            }
        }
    }
    
    if (playerInput.IsKeyJustPressed(SDLK_ESCAPE))
    {
        command = MenuCommandCloseMenu;
    }
    
    if (command == MenuCommandNone && playerInput.IsKeyJustPressed(SDLK_RETURN))
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
        m_renderableText.Reset();
        m_pageSound.Draw(renderer, 0, 0, false);
        renderer.RenderText(m_renderableText);
    }
    else if (m_subMenuSelected == subMenuRestoreGame)
    {
        m_renderableText.Reset();
        m_pageRestoreGame.Draw(renderer, 0, 0, false);
        renderer.RenderText(m_renderableText);
    }
    else if (m_subMenuSelected == subMenuSaveGame)
    {
        m_renderableText.Reset();
        m_pageSaveGame.Draw(renderer, 0, 0, false);
        renderer.RenderText(m_renderableText);
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

void ExtraMenu::AddNewSavedGame(const PlayerInput& playerInput, const std::string& name)
{
    m_pageRestoreGame.AddChild(new GuiElementButton(playerInput, name, { GuiActionRestoreGame, (int16_t)(m_savedGames.size() - 1) }, m_renderableText), 0, 0, restoreGameListId);
    m_pageSaveGame.AddChild(new GuiElementButton(playerInput, name, { GuiActionSaveGame, (int16_t)(m_savedGames.size() - 1) }, m_renderableText), 0, 0, saveGameListId);
}

void ExtraMenu::OpenRestoreGameMenu()
{
    m_menuActive = true;
    m_menuItemSelected = 0;
    m_subMenuSelected = subMenuRestoreGame;
    m_menuItemOffset = 0;
}

void ExtraMenu::OpenSaveGameMenu()
{
    m_menuActive = true;
    m_menuItemSelected = 0;
    m_subMenuSelected = subMenuSaveGame;
    m_menuItemOffset = 0;
}

void ExtraMenu::OpenSoundMenu()
{
    m_menuActive = true;
    m_menuItemSelected = 0;
    m_subMenuSelected = subMenuSound;
    m_menuItemOffset = 0;
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

egaColor inline ExtraMenu::GetMenuItemColor(const bool selected, const bool supported)
{
    return !supported ? EgaDarkGray : selected ? EgaBrightCyan : EgaBrightWhite;
}