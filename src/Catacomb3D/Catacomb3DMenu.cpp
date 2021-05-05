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
#include "..\Engine\GuiElementList.h"
#include "GuiElementBoolSelectionCat3D.h"
#include "GuiElementEnumSelectionCat3D.h"
#include "GuiElementIntSelectionCat3D.h"
#include "GuiElementBindKeyCat3D.h"
#include "GuiPageFrameCat3D.h"
#include "GuiElementSaveSlotStaticCat3D.h"
#include "GuiElementSaveSlotEditableCat3D.h"
#include "GuiElementButtonCat3D.h"
#include "GuiElementEnumSelectionCat3DRadio.h"
#include "..\Engine\GuiElementHighScores.h"
#include "..\Engine\GuiPage.h"

const uint16_t browseMenuSound = 0;

const int16_t restoreGameListId = 1;
const int16_t saveGameListId = 2;
const int16_t pageMainId = 3;
const int16_t pageVideoId = 4;
const int16_t pageControlsId = 5;
const int16_t pageSoundId = 6;
const int16_t pageMusicId = 7;
const int16_t pageGameplayId = 8;
const int16_t pageRestoreGameId = 9;
const int16_t pageSaveGameId = 10;
const int16_t pageHighScoresId = 11;
const int16_t pageSkullNBonesId = 12;
const int16_t pageNewGameId = 13;
const int16_t pageOptionsId = 14;
const int16_t goToSaveGameId = 15;
const int16_t endGameId = 16;
const int16_t selectVSyncId = 17;
const int16_t selectScreenResolutionId = 18;

const std::vector<std::string> enumNamesSound = { "NO SOUND EFFECTS", "PC SPEAKER", "ADLIB/SOUNDBLASTER" };

const std::vector<std::string> enumNamesMusic = { "NO MUSIC", "ADLIB/SOUNDBLASTER" };

Catacomb3DMenu::Catacomb3DMenu(
    ConfigurationSettings& configurationSettings,
    AudioPlayer& audioPlayer,
    PlayerInput& playerInput,
    EgaGraph* const egaGraph,
    std::vector<std::string>& savedGames,
    HighScores& highScores) :
    m_menuActive (false),
    m_saveGameEnabled (false),
    m_configurationSettings (configurationSettings),
    m_audioPlayer (audioPlayer),
    m_savedGames (savedGames),
    m_newSaveGameName (""),
    m_askForOverwrite (false),
    m_askForReset(false),
    m_askForResetClassic(false),
    m_askForEndGame (false),
    m_askForEndGameGuiAction(GuiActionNone),
    m_askForQuit (false),
    m_highScores(highScores),
    m_menuActivatedTimestamp(0u),
    m_guiMenu(playerInput),
    m_renderableText(*egaGraph->GetFont(4)),
    m_renderableTextDefaultFont(*egaGraph->GetDefaultFont(7)),
    m_renderableTiles(*egaGraph->GetTilesSize8()),
    m_flashIcon(false),
    m_timeStamp(0),
    m_returnToGameButton(nullptr)
{
    // Main menu
    GuiPage* guiPageMain = new GuiPage(playerInput);
    guiPageMain->SetId(pageMainId);
    GuiPageFrameCat3D* pageFrameMain = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderMain, m_renderableText);
    pageFrameMain->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    guiPageMain->AddChild(pageFrameMain);

    GuiElementList* elementListMain = new GuiElementList(playerInput, 8, 8, nullptr, browseMenuSound);
    elementListMain->AddChild(new GuiElementButtonCat3D(playerInput, "NEW GAME", { GuiActionNavigateTo, pageNewGameId }, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListMain->AddChild(new GuiElementButtonCat3D(playerInput, "LOAD GAME", { GuiActionNavigateTo, pageRestoreGameId }, m_renderableText, m_renderableTiles, m_flashIcon));
    GuiElementButton* goToSaveGameButton = new GuiElementButtonCat3D(playerInput, "SAVE GAME", { GuiActionNavigateTo, pageSaveGameId }, m_renderableText, m_renderableTiles, m_flashIcon);
    goToSaveGameButton->SetId(goToSaveGameId);
    elementListMain->AddChild(goToSaveGameButton);
    elementListMain->AddChild(new GuiElementButtonCat3D(playerInput, "OPTIONS", { GuiActionNavigateTo, pageOptionsId }, m_renderableText, m_renderableTiles, m_flashIcon));
    m_returnToGameButton = new GuiElementButtonCat3D(playerInput, "RETURN TO DEMO", { GuiActionClose, 0 }, m_renderableText, m_renderableTiles, m_flashIcon);
    elementListMain->AddChild(m_returnToGameButton);
    GuiElementButton* endGameButton = new GuiElementButtonCat3D(playerInput, "END GAME", { GuiActionEndGame, pageNewGameId }, m_renderableText, m_renderableTiles, m_flashIcon);
    endGameButton->SetId(endGameId);
    elementListMain->AddChild(endGameButton);
    elementListMain->AddChild(new GuiElementButtonCat3D(playerInput, "SKULL 'N' BONES", { GuiActionNavigateTo, pageSkullNBonesId }, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListMain->AddChild(new GuiElementButtonCat3D(playerInput, "QUIT", { GuiActionQuit, 0 }, m_renderableText, m_renderableTiles, m_flashIcon));
    guiPageMain->AddChild(elementListMain, 112, 62);

    // Options
    GuiPage* guiPageOptions = new GuiPage(playerInput);
    guiPageOptions->SetId(pageOptionsId);
    GuiPageFrameCat3D* pageFrameOptions = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderOptions, m_renderableText);
    pageFrameOptions->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    guiPageOptions->AddChild(pageFrameOptions);

    GuiElementList* elementListOptions = new GuiElementList(playerInput, 8, 8, nullptr, browseMenuSound);
    elementListOptions->AddChild(new GuiElementButtonCat3D(playerInput, "CUSTOMIZE CONTROLS", { GuiActionNavigateTo, pageControlsId }, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListOptions->AddChild(new GuiElementButtonCat3D(playerInput, "VIDEO OPTIONS", { GuiActionNavigateTo, pageVideoId }, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListOptions->AddChild(new GuiElementButtonCat3D(playerInput, "SOUND OPTIONS", { GuiActionNavigateTo, pageSoundId }, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListOptions->AddChild(new GuiElementButtonCat3D(playerInput, "MUSIC OPTIONS", { GuiActionNavigateTo, pageMusicId }, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListOptions->AddChild(new GuiElementButtonCat3D(playerInput, "GAMEPLAY OPTIONS", { GuiActionNavigateTo, pageGameplayId }, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListOptions->AddChild(new GuiElementButtonCat3D(playerInput, "RESET TO DEFAULTS", { GuiActionResetToDefaults, 0 }, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListOptions->AddChild(new GuiElementButtonCat3D(playerInput, "RESET TO CLASSIC LOOK", { GuiActionResetToClassic, 0 }, m_renderableText, m_renderableTiles, m_flashIcon));

    guiPageOptions->AddChild(elementListOptions, 88, 62);

    // New game menu
    GuiPage* guiPageNewGame = new GuiPage(playerInput);
    guiPageNewGame->SetId(pageNewGameId);
    GuiPageFrameCat3D* pageFrameNewGame = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderNewGame, m_renderableText);
    pageFrameNewGame->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    guiPageNewGame->AddChild(pageFrameNewGame);

    GuiElementList* elementListNewGame = new GuiElementList(playerInput, 8, 8, nullptr, browseMenuSound);
    elementListNewGame->AddChild(new GuiElementButtonCat3D(playerInput, "BEGIN EASY GAME", { GuiActionNewGameEasy, 0 }, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListNewGame->AddChild(new GuiElementButtonCat3D(playerInput, "BEGIN NORMAL GAME", { GuiActionNewGameNormal, 0 }, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListNewGame->AddChild(new GuiElementButtonCat3D(playerInput, "BEGIN HARD GAME", { GuiActionNewGameHard, 0 }, m_renderableText, m_renderableTiles, m_flashIcon));

    guiPageNewGame->AddChild(elementListNewGame, 88, 62);

    // Video menu
    GuiPage* guiPageVideo = new GuiPage(playerInput);
    guiPageVideo->SetId(pageVideoId);
    GuiPageFrameCat3D* pageFrameVideo = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderVideo, m_renderableText);
    pageFrameVideo->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    guiPageVideo->AddChild(pageFrameVideo);

    GuiElementList* elementListVideo = new GuiElementList(playerInput, 8, 8, nullptr, browseMenuSound);
    elementListVideo->AddChild(new GuiElementEnumSelectionCat3D(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdScreenMode), 104, m_renderableText, m_renderableTiles, m_flashIcon));
    GuiElementEnumSelectionCat3D* VScreenResolutionSelection = new GuiElementEnumSelectionCat3D(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdScreenResolution), 104, m_renderableText, m_renderableTiles, m_flashIcon);
    VScreenResolutionSelection->SetId(selectScreenResolutionId);
    elementListVideo->AddChild(VScreenResolutionSelection);
    elementListVideo->AddChild(new GuiElementEnumSelectionCat3D(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdAspectRatio), 84, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListVideo->AddChild(new GuiElementIntSelectionCat3D(playerInput, configurationSettings.GetCVarIntMutable(CVarIdFov), 104, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListVideo->AddChild(new GuiElementEnumSelectionCat3D(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdTextureFilter), 104, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListVideo->AddChild(new GuiElementBoolSelectionCat3D(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdDepthShading), 104, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListVideo->AddChild(new GuiElementEnumSelectionCat3D(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdShowFpsMode), 104, m_renderableText, m_renderableTiles, m_flashIcon));
    GuiElementBoolSelectionCat3D* VSyncSelection = new GuiElementBoolSelectionCat3D(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdVSync), 104, m_renderableText, m_renderableTiles, m_flashIcon);
    VSyncSelection->SetId(selectVSyncId);
    elementListVideo->AddChild(VSyncSelection);
    elementListVideo->AddChild(new GuiElementEnumSelectionCat3D(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdAutoMapMode), 104, m_renderableText, m_renderableTiles, m_flashIcon));
    guiPageVideo->AddChild(elementListVideo, 76, 62);

    // Controls menu
    GuiPage* guiPageControls = new GuiPage(playerInput);
    guiPageControls->SetId(pageControlsId);
    GuiPageFrameCat3D* pageFrameControls = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderControls, m_renderableText);
    pageFrameControls->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    guiPageControls->AddChild(pageFrameControls);
    GuiElementList* elementListControls = new GuiElementList(playerInput, 8, 8, nullptr, browseMenuSound);
    ControlsMap& controlsMap = configurationSettings.GetControlsMap();
    const std::map<ControlAction, std::string>& actionLabels = controlsMap.GetActionLabels();
    for (const std::pair<ControlAction, std::string>& actionLabel : actionLabels)
    {
        if (actionLabel.first != None)
        {
            elementListControls->AddChild(new GuiElementBindKeyCat3D(playerInput, controlsMap, actionLabel.first, 84, m_renderableTextDefaultFont, m_renderableTiles, m_flashIcon));
        }
    }
    elementListControls->AddChild(new GuiElementBoolSelectionCat3D(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdMouseLook), 84, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListControls->AddChild(new GuiElementIntSelectionCat3D(playerInput, configurationSettings.GetCVarIntMutable(CVarIdMouseSensitivity), 84, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListControls->AddChild(new GuiElementIntSelectionCat3D(playerInput, configurationSettings.GetCVarIntMutable(CVarIdTurnSpeed), 84, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListControls->AddChild(new GuiElementBoolSelectionCat3D(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdAlwaysRun), 84, m_renderableText, m_renderableTiles, m_flashIcon));
    elementListControls->AddChild(new GuiElementBoolSelectionCat3D(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdAutoFire), 84, m_renderableText, m_renderableTiles, m_flashIcon));
    guiPageControls->AddChild(elementListControls, 76, 62);

    // Gameplay menu
    GuiPage* guiPageGameplay = new GuiPage(playerInput);
    guiPageGameplay->SetId(pageGameplayId);
    GuiPageFrameCat3D* pageFrameGameplay = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderGameplay, m_renderableText);
    pageFrameGameplay->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    guiPageGameplay->AddChild(pageFrameGameplay);

    GuiElementList* elementListGameplay = new GuiElementList(playerInput, 8, 8, nullptr, browseMenuSound);
    elementListGameplay->AddChild(new GuiElementBoolSelectionCat3D(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdManaBar), 84, m_renderableText, m_renderableTiles, m_flashIcon));
    guiPageGameplay->AddChild(elementListGameplay, 76, 62);

    // Restore game menu
    GuiPage* guiPageLoadGame = new GuiPage(playerInput);
    guiPageLoadGame->SetId(pageRestoreGameId);

    GuiPageFrameCat3D* pageFrameLoadGame = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderLoadGame, m_renderableText);
    pageFrameLoadGame->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    guiPageLoadGame->AddChild(pageFrameLoadGame);

    GuiElementList* elementListRestoreGame = new GuiElementList(playerInput, 6, 11, nullptr, browseMenuSound);
    elementListRestoreGame->SetId(restoreGameListId);
    if (savedGames.size() > 0)
    {
        int16_t savedGameIndex = 0;
        for (const std::string& savedGame : savedGames)
        {
            elementListRestoreGame->AddChild(new GuiElementSaveSlotStaticCat3D(playerInput, savedGame, { GuiActionLoadGame, savedGameIndex }, m_renderableText, m_flashIcon));
            savedGameIndex++;
        }
    }
    guiPageLoadGame->AddChild(elementListRestoreGame, 80, 60);

    // Save game menu
    GuiPage* guiPageSaveGame = new GuiPage(playerInput);
    guiPageSaveGame->SetId(pageSaveGameId);

    GuiPageFrameCat3D* pageFrameSaveGame = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderSaveGame, m_renderableText);
    pageFrameSaveGame->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    guiPageSaveGame->AddChild(pageFrameSaveGame);

    GuiElementList* elementListSaveGame = new GuiElementList(playerInput, 6, 11, nullptr, browseMenuSound);
    GuiElementSaveSlotEditableCat3D* saveGameEditText = new GuiElementSaveSlotEditableCat3D(playerInput, m_newSaveGameName, "<< new saved game >>", 20, m_renderableText, GuiEvent({ GuiActionSaveGame, -1 }), m_flashIcon, pageFrameSaveGame);
    elementListSaveGame->SetId(saveGameListId);
    elementListSaveGame->AddChild(saveGameEditText);

    if (savedGames.size() > 0)
    {
        int16_t savedGameIndex = 0;
        for (const std::string& savedGame : savedGames)
        {
            elementListSaveGame->AddChild(new GuiElementSaveSlotStaticCat3D(playerInput, savedGame, { GuiActionSaveGame, savedGameIndex }, m_renderableText, m_flashIcon));
            savedGameIndex++;
        }
    }
    guiPageSaveGame->AddChild(elementListSaveGame, 80, 60);

    // Sound menu
    GuiPage* guiPageSound = new GuiPage(playerInput);
    guiPageSound->SetId(pageSoundId);
    GuiPageFrameCat3D* pageFrameSound = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderSound, m_renderableText);
    pageFrameSound->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    guiPageSound->AddChild(pageFrameSound);

    guiPageSound->AddChild(new GuiElementEnumSelectionCat3DRadio(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdSoundMode), m_renderableText, m_renderableTiles, m_flashIcon, enumNamesSound), 88, 62);

    // Music menu
    GuiPage* guiPageMusic = new GuiPage(playerInput);
    guiPageMusic->SetId(pageMusicId);
    GuiPageFrameCat3D* pageFrameMusic = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderMusic, m_renderableText);
    pageFrameMusic->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    guiPageMusic->AddChild(pageFrameMusic);

    guiPageMusic->AddChild(new GuiElementEnumSelectionCat3DRadio(playerInput, configurationSettings.GetCVarEnumMutable(CVarIdMusicMode), m_renderableText, m_renderableTiles, m_flashIcon, enumNamesMusic), 88, 62);

    // High scores
    GuiPage* guiPageHighScores = new GuiPage(playerInput);
    guiPageHighScores->SetId(pageHighScoresId);
    GuiElementHighScores* guiElementHighScores = new GuiElementHighScores(playerInput, m_highScores, m_timeStamp);
    guiPageHighScores->AddChild(guiElementHighScores);

    SkullNBones* skullNBones = new SkullNBones(playerInput, audioPlayer, *egaGraph, m_timeStamp, m_renderableText);
    skullNBones->SetId(pageSkullNBonesId);

    m_guiMenu.AddChild(guiPageMain);
    m_guiMenu.AddChild(guiPageOptions);
    m_guiMenu.AddChild(guiPageNewGame);
    m_guiMenu.AddChild(guiPageVideo);
    m_guiMenu.AddChild(guiPageControls);
    m_guiMenu.AddChild(guiPageSound);
    m_guiMenu.AddChild(guiPageMusic);
    m_guiMenu.AddChild(guiPageGameplay);
    m_guiMenu.AddChild(guiPageLoadGame);
    m_guiMenu.AddChild(guiPageSaveGame);
    m_guiMenu.AddChild(guiPageHighScores);
    m_guiMenu.AddChild(skullNBones);
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
        m_guiMenu.Open(pageMainId);
    }
}

MenuCommand Catacomb3DMenu::ProcessInput(const PlayerInput& playerInput)
{
    MenuCommand command = MenuCommandNone;
    const SDL_Keycode keyCode = playerInput.GetFirstKeyPressed();
    if (m_askForOverwrite)
    {
        if (RepliedWithYes(keyCode))
        {
            m_askForOverwrite = false;
            command = MenuCommandSaveGame;
        }
        else if (RepliedWithNo(keyCode))
        {
            m_askForOverwrite = false;
        }
    }
    else if (m_askForReset)
    {
        if (RepliedWithYes(keyCode))
        {
            m_configurationSettings.ResetToDefaults();
            m_askForReset = false;
        }
        else if (RepliedWithNo(keyCode))
        {
            m_askForReset = false;
        }
    }
    else if (m_askForResetClassic)
    {
        if (RepliedWithYes(keyCode))
        {
            m_configurationSettings.ResetToClassic();
            m_askForResetClassic = false;
        }
        else if (RepliedWithNo(keyCode))
        {
            m_askForResetClassic = false;
        }
    }
    else if (m_askForQuit)
    {
        if (RepliedWithYes(keyCode))
        {
            command = MenuCommandExitGame;
            m_askForQuit = false;
        }
        else if (RepliedWithNo(keyCode))
        {
            m_askForQuit = false;
        }
    }
    else if (m_askForEndGame)
    {
        if (RepliedWithYes(keyCode))
        {
            m_askForEndGame = false;
            if (m_askForEndGameGuiAction == GuiActionNewGameEasy)
            {
                command = MenuCommandStartNewGameEasy;
            }
            else if (m_askForEndGameGuiAction == GuiActionNewGameNormal)
            {
                command = MenuCommandStartNewGameNormal;
            }
            else if (m_askForEndGameGuiAction == GuiActionNewGameHard)
            {
                command = MenuCommandStartNewGameHard;
            }
            else if (m_askForEndGameGuiAction == GuiActionLoadGame)
            {
                command = MenuCommandLoadGame;
            }
            else
            {
                command = MenuCommandEndGame;
            }
        }
        else if (RepliedWithNo(keyCode))
        {
            m_askForEndGame = false;
            command = MenuCommandNone;
        }
    }
    else
    {
        const GuiEvent& guiEvent = m_guiMenu.ProcessInput();
        if (guiEvent.guiAction == GuiActionClose)
        {
            command = MenuCommandCloseMenu;
        }
        else if (guiEvent.guiAction == GuiActionEndGame)
        {
            m_askForEndGame = true;
            m_askForEndGameGuiAction = GuiActionEndGame;
        }
        else if (guiEvent.guiAction == GuiActionHighScoreFinished)
        {
            command = MenuCommandEndGame;
        }
        else if (guiEvent.guiAction == GuiActionQuit)
        {
            m_askForQuit = true;
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
        else if (guiEvent.guiAction == GuiActionLoadGame)
        {
            m_newSaveGameName = m_savedGames.at(guiEvent.guiParameter);
            if (m_saveGameEnabled)
            {
                m_askForOverwrite = false;
                m_askForEndGame = true;
                m_askForEndGameGuiAction = guiEvent.guiAction;
            }
            else
            {
                command = MenuCommandLoadGame;
            }
        }
        else if (guiEvent.guiAction == GuiActionNewGameEasy ||
            guiEvent.guiAction == GuiActionNewGameNormal ||
            guiEvent.guiAction == GuiActionNewGameHard)
        {
            if (m_saveGameEnabled)
            {
                m_askForEndGame = true;
                m_askForEndGameGuiAction = guiEvent.guiAction;
            }
            else
            {
                command =
                    (guiEvent.guiAction == GuiActionNewGameEasy) ? MenuCommandStartNewGameEasy :
                    (guiEvent.guiAction == GuiActionNewGameNormal) ? MenuCommandStartNewGameNormal :
                    MenuCommandStartNewGameHard;
            }
        }
        else if (guiEvent.guiAction == GuiActionResetToDefaults)
        {
            m_askForReset = true;
        }
        else if (guiEvent.guiAction == GuiActionResetToClassic)
        {
            m_askForResetClassic = true;
        }
    }
    
    return command;
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

 //   if (m_subMenuSelected != subMenuHighScores)
 //   {
        renderer.Render2DPicture(egaGraph->GetPicture(CP_MENUSCREENPIC), 0, 0);
 //   }

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
    m_flashIcon = flashIcon;
    m_timeStamp = timeStamp;

    m_renderableText.Reset();
    m_renderableTextDefaultFont.Reset();
    m_renderableTiles.Reset();
    m_guiMenu.Draw(renderer);
    renderer.RenderTiles(m_renderableTiles);
    renderer.RenderText(m_renderableText);
    renderer.RenderText(m_renderableTextDefaultFont);

    if (m_askForEndGame)
    {
        if (m_askForEndGameGuiAction == GuiActionNewGameEasy ||
            m_askForEndGameGuiAction == GuiActionNewGameNormal ||
            m_askForEndGameGuiAction == GuiActionNewGameHard)
        {
            DrawConfirmationDialog(renderer, *egaGraph, 142, "YOU'RE IN A GAME", "PRESS Y FOR NEW GAME", "ESC TO BACK OUT");
        }
        else if (m_askForEndGameGuiAction == GuiActionEndGame)
        {
            DrawConfirmationDialog(renderer, *egaGraph, 142, "REALLY END CURRENT GAME?", "PRESS Y TO END IT", "ESC TO BACK OUT");
        }
        else if (m_askForEndGameGuiAction == GuiActionLoadGame)
        {
            DrawConfirmationDialog(renderer, *egaGraph, 142, "YOU'RE IN A GAME", "PRESS Y TO LOAD GAME", "ESC TO BACK OUT");
        }
    }

    if (m_askForQuit)
    {
        DrawConfirmationDialog(renderer, *egaGraph, 90, "REALLY QUIT?", "PRESS Y TO QUIT", "ESC TO BACK OUT");
    }
    else if (m_askForReset)
    {
        DrawConfirmationDialog(renderer, *egaGraph, 120, "RESET TO DEFAULTS?", "PRESS Y TO CONFIRM", "ESC TO BACK OUT");
    }
    else if (m_askForResetClassic)
    {
        DrawConfirmationDialog(renderer, *egaGraph, 120, "RESET TO CLASSIC LOOK?", "PRESS Y TO CONFIRM", "ESC TO BACK OUT");
    }
}

void Catacomb3DMenu::SetSaveGameEnabled(const bool enabled)
{
    m_saveGameEnabled = enabled;
    m_guiMenu.SetEnabled(enabled, goToSaveGameId);
    m_guiMenu.SetEnabled(enabled, endGameId);
    m_returnToGameButton->SetLabel(m_saveGameEnabled ? "RETURN TO GAME" : "RETURN TO DEMO");
}

const std::string& Catacomb3DMenu::GetNewSaveGameName() const
{
    return m_newSaveGameName;
}

void Catacomb3DMenu::AddNewSavedGame(const PlayerInput& playerInput, const std::string& name)
{
    m_guiMenu.AddChild(new GuiElementSaveSlotStaticCat3D(playerInput, name, { GuiActionLoadGame, (int16_t)(m_savedGames.size() - 1) }, m_renderableText, m_flashIcon), 0, 0, restoreGameListId);
    m_guiMenu.AddChild(new GuiElementSaveSlotStaticCat3D(playerInput, name, { GuiActionSaveGame, (int16_t)(m_savedGames.size() - 1) }, m_renderableText, m_flashIcon), 0, 0, saveGameListId);
}

void Catacomb3DMenu::OpenRestoreGameMenu()
{
    m_menuActive = true;
    m_guiMenu.Open(pageRestoreGameId);
}

void Catacomb3DMenu::OpenSaveGameMenu()
{
    m_menuActive = true;
    m_guiMenu.Open(pageSaveGameId);
}

void Catacomb3DMenu::OpenSoundMenu()
{
    m_menuActive = true;
    m_guiMenu.Open(pageSoundId);
}

void Catacomb3DMenu::OpenDemo()
{
    // No demo in Catacomb 3D
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
    m_guiMenu.Open(pageHighScoresId);
    m_highScores.TryToAddNewScore(score, level + 1);
}

bool Catacomb3DMenu::RepliedWithYes(const SDL_Keycode keyCode)
{
    return (keyCode == SDLK_y ||
        keyCode == SDLK_RETURN ||
        keyCode == SDLK_KP_ENTER);
}

bool Catacomb3DMenu::RepliedWithNo(const SDL_Keycode keyCode)
{
    return (keyCode == SDLK_n ||
        keyCode == SDLK_ESCAPE);
}
