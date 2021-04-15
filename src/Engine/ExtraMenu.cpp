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
#include "GuiCatalog.h"

const uint16_t browseMenuSound = 0;

const int16_t loadGameListId = 1;
const int16_t saveGameListId = 2;
const int16_t pageMainId = 3;
const int16_t pageOptionsId = 4;
const int16_t pageCustomizeControlsId = 5;
const int16_t pageVideoOptionsId = 6;
const int16_t pageSoundOptionsId = 7;
const int16_t pageGameplayOptionsId = 8;
const int16_t pageLoadGameId = 9;
const int16_t pageSaveGameId = 10;
const int16_t pageCatalogId = 11;
const int16_t goToSaveGameId = 12;
const int16_t selectVSyncId = 13;
const int16_t selectScreenResolutionId = 14;

ExtraMenu::ExtraMenu(
    ConfigurationSettings& configurationSettings,
    AudioPlayer& audioPlayer,
    PlayerInput& playerInput,
    EgaGraph* const egaGraph,
    const uint16_t menuCursorPic,
    std::vector<std::string>& savedGames,
    const IRenderer& renderer,
    const CatalogInfo& catalogInfo,
    const std::string& gameFolder) :
    m_menuActive(false),
    m_configurationSettings(configurationSettings),
    m_audioPlayer(audioPlayer),
    m_savedGames(savedGames),
    m_newSaveGameName(""),
    m_askForOverwrite(false),
    m_askForReset(false),
    m_askForResetClassic(false),
    m_guiMenu(playerInput),
    m_renderableText(*egaGraph->GetFont(3)),
    m_renderableTextDefaultFont(*egaGraph->GetDefaultFont(10))
{
    // Main menu
    GuiPage* pageMain = new GuiPage(playerInput);
    pageMain->SetId(pageMainId);

    GuiElementList* elementListMain = new GuiElementList(playerInput, 8, 10, egaGraph->GetPicture(menuCursorPic), browseMenuSound);
    elementListMain->AddChild(new GuiElementButton(playerInput, "New Game", {GuiActionNewGame, 0}, m_renderableText));
    elementListMain->AddChild(new GuiElementButton(playerInput, "Load Game", { GuiActionNavigateTo, pageLoadGameId }, m_renderableText));
    GuiElementButton* goToSaveGameButton = new GuiElementButton(playerInput, "Save Game", { GuiActionNavigateTo, pageSaveGameId }, m_renderableText);
    goToSaveGameButton->SetId(goToSaveGameId);
    elementListMain->AddChild(goToSaveGameButton);
    elementListMain->AddChild(new GuiElementButton(playerInput, "Options", { GuiActionNavigateTo, pageOptionsId }, m_renderableText));
    elementListMain->AddChild(new GuiElementButton(playerInput, catalogInfo.label, { GuiActionNavigateTo, pageCatalogId }, m_renderableText));
    elementListMain->AddChild(new GuiElementButton(playerInput, "Quit", { GuiActionQuit, 0 }, m_renderableText));
    pageMain->AddChild(elementListMain, 140, 30);

    GuiElementStaticText* pageLabelMain = new GuiElementStaticText(playerInput, "Main Menu", EgaBrightYellow, m_renderableText);
    pageMain->AddChild(pageLabelMain, 160, 12);

    // Options menu
    GuiPage* pageOptions = new GuiPage(playerInput);
    pageOptions->SetId(pageOptionsId);

    GuiElementList* elementListOptions = new GuiElementList(playerInput, 8, 10, egaGraph->GetPicture(menuCursorPic), browseMenuSound);
    elementListOptions->AddChild(new GuiElementButton(playerInput, "Customize Controls", { GuiActionNavigateTo, pageCustomizeControlsId }, m_renderableText));
    elementListOptions->AddChild(new GuiElementButton(playerInput, "Video Options", { GuiActionNavigateTo, pageVideoOptionsId }, m_renderableText));
    elementListOptions->AddChild(new GuiElementButton(playerInput, "Sound Options", { GuiActionNavigateTo, pageSoundOptionsId }, m_renderableText));
    elementListOptions->AddChild(new GuiElementButton(playerInput, "Gameplay Options", { GuiActionNavigateTo, pageGameplayOptionsId }, m_renderableText));
    elementListOptions->AddChild(new GuiElementButton(playerInput, "Reset To Defaults", { GuiActionResetToDefaults, 0 }, m_renderableText));
    elementListOptions->AddChild(new GuiElementButton(playerInput, "Reset To Classic Look", { GuiActionResetToClassic, 0 }, m_renderableText));
    pageOptions->AddChild(elementListOptions, 120, 30);

    GuiElementStaticText* pageLabelOptions = new GuiElementStaticText(playerInput, "Options", EgaBrightYellow, m_renderableText);
    pageOptions->AddChild(pageLabelOptions, 160, 12);

    // Video menu
    GuiPage* pageVideo = new GuiPage(playerInput);
    pageVideo->SetId(pageVideoOptionsId);

    GuiElementList* elementListVideo = new GuiElementList(playerInput, 8, 10, egaGraph->GetPicture(menuCursorPic), browseMenuSound);
    elementListVideo->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdScreenMode), 132, m_renderableText));
    GuiElementEnumSelection* VScreenResolutionSelection = new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdScreenResolution), 132, m_renderableText);
    VScreenResolutionSelection->SetId(selectScreenResolutionId);
    elementListVideo->AddChild(VScreenResolutionSelection);
    elementListVideo->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdAspectRatio), 132, m_renderableText));
    elementListVideo->AddChild(new GuiElementIntSelection(playerInput, configurationSettings.GetCVarIntMutable(CVarIdFov), 132, m_renderableText));
    elementListVideo->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdTextureFilter), 132, m_renderableText));
    elementListVideo->AddChild(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdDepthShading), 132, m_renderableText));
    elementListVideo->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdShowFpsMode), 132, m_renderableText));
    GuiElementBoolSelection* VSyncSelection = new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdVSync), 132, m_renderableText);
    VSyncSelection->SetId(selectVSyncId);
    elementListVideo->AddChild(VSyncSelection);
    elementListVideo->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdAutoMapMode), 132, m_renderableText));
    pageVideo->AddChild(elementListVideo, 60, 30);

    GuiElementStaticText* pageLabelVideo = new GuiElementStaticText(playerInput, "Video Options", EgaBrightYellow, m_renderableText);
    pageVideo->AddChild(pageLabelVideo, 160, 12);

    // Controls menu
    GuiPage* pageControls = new GuiPage(playerInput);
    pageControls->SetId(pageCustomizeControlsId);

    GuiElementList* elementListControls = new GuiElementList(playerInput, 8, 10, egaGraph->GetPicture(menuCursorPic), browseMenuSound);
    ControlsMap& controlsMap = configurationSettings.GetControlsMap();
    const std::map<ControlAction, std::string>& actionLabels = controlsMap.GetActionLabels();
    for (const std::pair<ControlAction, std::string>& actionLabel : actionLabels)
    {
        if (actionLabel.first != None)
        {
            elementListControls->AddChild(new GuiElementBindKey(playerInput, controlsMap, actionLabel.first, 95, m_renderableTextDefaultFont));
        }
    }
    elementListControls->AddChild(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdMouseLook), 95, m_renderableText));
    elementListControls->AddChild(new GuiElementIntSelection(playerInput, configurationSettings.GetCVarIntMutable(CVarIdMouseSensitivity), 95, m_renderableText));
    elementListControls->AddChild(new GuiElementIntSelection(playerInput, configurationSettings.GetCVarIntMutable(CVarIdTurnSpeed), 95, m_renderableText));
    elementListControls->AddChild(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdAlwaysRun), 95, m_renderableText));
    elementListControls->AddChild(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdAutoFire), 95, m_renderableText));

    pageControls->AddChild(elementListControls, 60, 30);

    GuiElementStaticText* pageLabelControls = new GuiElementStaticText(playerInput, "Customize Controls", EgaBrightYellow, m_renderableText);
    pageControls->AddChild(pageLabelControls, 160, 12);

    // Gameplay menu
    GuiPage* pageGameplay = new GuiPage(playerInput);
    pageGameplay->SetId(pageGameplayOptionsId);

    GuiElementList* elementListGameplay = new GuiElementList(playerInput, 8, 10, egaGraph->GetPicture(menuCursorPic), browseMenuSound);
    elementListGameplay->AddChild(new GuiElementBoolSelection(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdManaBar), 95, m_renderableText));

    pageGameplay->AddChild(elementListGameplay, 60, 30);

    GuiElementStaticText* pageLabelGameplay = new GuiElementStaticText(playerInput, "Gameplay Options", EgaBrightYellow, m_renderableText);
    pageGameplay->AddChild(pageLabelGameplay, 160, 12);

    // Sound menu
    GuiPage* pageSound = new GuiPage(playerInput);
    pageSound->SetId(pageSoundOptionsId);

    GuiElementList* elementListSound = new GuiElementList(playerInput, 8, 10, egaGraph->GetPicture(menuCursorPic), browseMenuSound);
    elementListSound->AddChild(new GuiElementEnumSelection(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdSoundMode), 140, m_renderableText));
    pageSound->AddChild(elementListSound, 60, 30);

    GuiElementStaticText* pageLabelSound = new GuiElementStaticText(playerInput, "Sound Options", EgaBrightYellow, m_renderableText);
    pageSound->AddChild(pageLabelSound, 160, 12);

    // Load game menu
    GuiPage* pageLoadGame = new GuiPage(playerInput);
    pageLoadGame->SetId(pageLoadGameId);

    GuiElementList* elementListLoadGame = new GuiElementList(playerInput, 8, 10, egaGraph->GetPicture(menuCursorPic), browseMenuSound);
    elementListLoadGame->SetId(loadGameListId);
    if (savedGames.size() > 0)
    {
        int16_t savedGameIndex = 0;
        for (const std::string& savedGame : savedGames)
        {
            elementListLoadGame->AddChild(new GuiElementButton(playerInput, savedGame, { GuiActionLoadGame, savedGameIndex }, m_renderableText));
            savedGameIndex++;
        }
        pageLoadGame->AddChild(elementListLoadGame, 60, 30);
    }

    GuiElementStaticText* pageLabelLoadGame = new GuiElementStaticText(playerInput, "Load Game", EgaBrightYellow, m_renderableText);
    pageLoadGame->AddChild(pageLabelLoadGame, 160, 12);

    // Save game menu
    GuiPage* pageSaveGame = new GuiPage(playerInput);
    pageSaveGame->SetId(pageSaveGameId);

    GuiElementList* elementListSaveGame = new GuiElementList(playerInput, 8, 10, egaGraph->GetPicture(menuCursorPic), browseMenuSound);
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
        pageSaveGame->AddChild(elementListSaveGame, 60, 30);
    }

    GuiElementStaticText* pageLabelSaveGame = new GuiElementStaticText(playerInput, "Save Game", EgaBrightYellow, m_renderableText);
    pageSaveGame->AddChild(pageLabelSaveGame, 160, 12);

    GuiCatalog* guiCatalog = new GuiCatalog(playerInput, renderer, catalogInfo.filenames, gameFolder);
    guiCatalog->SetId(pageCatalogId);

    m_guiMenu.AddChild(pageMain);
    m_guiMenu.AddChild(pageOptions);
    m_guiMenu.AddChild(pageVideo);
    m_guiMenu.AddChild(pageControls);
    m_guiMenu.AddChild(pageGameplay);
    m_guiMenu.AddChild(pageSound);
    m_guiMenu.AddChild(pageLoadGame);
    m_guiMenu.AddChild(pageSaveGame);
    m_guiMenu.AddChild(guiCatalog);
}

bool ExtraMenu::IsActive() const
{
    return m_menuActive;
}

void ExtraMenu::SetActive(bool active)
{
    m_menuActive = active;
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

    if (m_askForReset)
    {
        if (keyCode != SDLK_UNKNOWN)
        {
            if (keyCode == SDLK_y)
            {
                m_configurationSettings.ResetToDefaults();
            }
            m_askForReset = false;
        }
    }

    if (m_askForResetClassic)
    {
        if (keyCode != SDLK_UNKNOWN)
        {
            if (keyCode == SDLK_y)
            {
                m_configurationSettings.ResetToClassic();
            }
            m_askForResetClassic = false;
        }
    }

    const GuiEvent& guiEvent = m_guiMenu.ProcessInput();
 
    if (guiEvent.guiAction == GuiActionLoadGame)
    {
        command = MenuCommandLoadGame;
        m_newSaveGameName = m_savedGames.at(guiEvent.guiParameter);
    }
    else if (guiEvent.guiAction == GuiActionSaveGame)
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
        }
    }
    else if (guiEvent.guiAction == GuiActionClose)
    {
        command = MenuCommandCloseMenu;
    }
    else if (guiEvent.guiAction == GuiActionNewGame)
    {
        // New game
        command = MenuCommandStartNewGame;
        m_menuActive = false;
    }
    else if (guiEvent.guiAction == GuiActionQuit)
    {
        command = MenuCommandExitGame;
    }
    else if (guiEvent.guiAction == GuiActionPlaySound)
    {
        m_audioPlayer.Play((uint16_t)guiEvent.guiParameter);
    }
    else if (guiEvent.guiAction == GuiActionResetToDefaults)
    {
        m_askForReset = true;
    }
    else if (guiEvent.guiAction == GuiActionResetToClassic)
    {
        m_askForResetClassic = true;
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

    if (m_askForReset)
    {
        RenderableText renderableText(*egaGraph->GetFont(3));
        renderableText.Centered("Reset To Defaults", EgaBrightYellow, 160, 32);
        renderableText.Centered("Are you sure you want to do this?", EgaBrightWhite, 160, 52);
        renderableText.Centered("Y / N", EgaBrightWhite, 160, 72);
        renderer.RenderText(renderableText);
        return;
    }

    if (m_askForResetClassic)
    {
        RenderableText renderableText(*egaGraph->GetFont(3));
        renderableText.Centered("Reset To Classic Look", EgaBrightYellow, 160, 32);
        renderableText.Centered("Are you sure you want to do this?", EgaBrightWhite, 160, 52);
        renderableText.Centered("Y / N", EgaBrightWhite, 160, 72);
        renderer.RenderText(renderableText);
        return;
    }

    // Check if video features are supported by graphics hardware
    m_guiMenu.SetEnabled(renderer.IsVSyncSupported(), selectVSyncId);
    m_guiMenu.SetEnabled(renderer.IsOriginalScreenResolutionSupported(), selectScreenResolutionId);

    m_renderableText.Reset();
    m_renderableTextDefaultFont.Reset();
    m_guiMenu.Draw(renderer);
    renderer.RenderText(m_renderableText);
    renderer.RenderText(m_renderableTextDefaultFont);
}

void ExtraMenu::SetSaveGameEnabled(const bool enabled)
{
    m_guiMenu.SetEnabled(enabled, goToSaveGameId);
}

const std::string& ExtraMenu::GetNewSaveGameName() const
{
    return m_newSaveGameName;
}

void ExtraMenu::AddNewSavedGame(const PlayerInput& playerInput, const std::string& name)
{
    m_guiMenu.AddChild(new GuiElementButton(playerInput, name, { GuiActionLoadGame, (int16_t)(m_savedGames.size() - 1) }, m_renderableText), 0, 0, loadGameListId);
    m_guiMenu.AddChild(new GuiElementButton(playerInput, name, { GuiActionSaveGame, (int16_t)(m_savedGames.size() - 1) }, m_renderableText), 0, 0, saveGameListId);
}

void ExtraMenu::OpenRestoreGameMenu()
{
    m_menuActive = true;
    m_guiMenu.Open(pageLoadGameId);
}

void ExtraMenu::OpenSaveGameMenu()
{
    m_menuActive = true;
    m_guiMenu.Open(pageSaveGameId);
}

void ExtraMenu::OpenSoundMenu()
{
    m_menuActive = true;
    m_guiMenu.Open(pageSoundOptionsId);
}

void ExtraMenu::OpenDemo()
{
    m_menuActive = true;
    m_guiMenu.Open(pageCatalogId);
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
