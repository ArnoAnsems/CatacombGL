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

const uint8_t menuItemMainNewGame = 0;
const uint8_t menuItemMainRestoreGame = 1;
const uint8_t menuItemMainSaveGame = 2;
const uint8_t menuItemMainConfigure = 3;
const uint8_t menuItemMainReturnToDemo = 4;
const uint8_t menuItemMainEndGame = 5;
const uint8_t menuItemMainSkullNBones = 6;
const uint8_t menuItemMainQuit = 7;

const uint16_t browseMenuSound = 0;

const int16_t restoreGameListId = 1;
const int16_t saveGameListId = 2;
const int16_t pageMainId = 3;
const int16_t pageVideoId = 4;
const int16_t pageControlsId = 5;
const int16_t pageSoundId = 6;
const int16_t pageRestoreGameId = 7;
const int16_t pageSaveGameId = 8;
const int16_t goToSaveGameId = 9;
const int16_t selectVSyncId = 10;
const int16_t selectScreenResolutionId = 11;

Catacomb3DMenu::Catacomb3DMenu(
    ConfigurationSettings& configurationSettings,
    AudioPlayer& audioPlayer,
    PlayerInput& playerInput,
    EgaGraph* const egaGraph,
    std::vector<std::string>& savedGames,
    HighScores& highScores) :
    m_menuActive (false),
    m_menuItemSelected (0),
    m_subMenuSelected (0),
    m_menuItemOffset (0),
    m_saveGameEnabled (false),
    m_configurationSettings (configurationSettings),
    m_audioPlayer (audioPlayer),
    m_savedGames (savedGames),
    m_newSaveGameName (""),
    m_askForOverwrite (false),
    m_askForEndGame (false),
    m_askForQuit (false),
    m_highScores(highScores),
    m_skullNBones(audioPlayer),
    m_menuActivatedTimestamp(0u),
    m_guiPageVideo(nullptr),
    m_guiPageControls(nullptr),
    m_guiPageSaveGame(nullptr),
    m_guiPageLoadGame(nullptr),
    m_renderableText(*egaGraph->GetFont(4)),
    m_renderableTextDefaultFont(*egaGraph->GetDefaultFont(7)),
    m_renderableTiles(*egaGraph->GetTilesSize8()),
    m_flashIcon(false)
{
    // Video menu
    m_guiPageVideo = new GuiPage(playerInput);
    GuiPageFrameCat3D* pageFrameVideo = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderVideo, m_renderableText);
    pageFrameVideo->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    m_guiPageVideo->AddChild(pageFrameVideo);
    GuiElementList* elementListVideo = new GuiElementList(playerInput, 8, 76, 62, 8, nullptr, browseMenuSound);
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
    m_guiPageVideo->AddChild(elementListVideo, 76, 62);

    // Controls menu
    m_guiPageControls = new GuiPage(playerInput);
    GuiPageFrameCat3D* pageFrameControls = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderControls, m_renderableText);
    pageFrameControls->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    m_guiPageControls->AddChild(pageFrameControls);
    GuiElementList* elementListControls = new GuiElementList(playerInput, 7, 76, 54, 8, nullptr, browseMenuSound);
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
    elementListControls->AddChild(new GuiElementBoolSelectionCat3D(playerInput, configurationSettings.GetCVarBoolMutable(CVarIdManaBar), 84, m_renderableText, m_renderableTiles, m_flashIcon));
    m_guiPageControls->AddChild(elementListControls, 76, 62);

    // Restore game menu
    m_guiPageLoadGame = new GuiPage(playerInput);
    m_guiPageLoadGame->SetId(pageRestoreGameId);

    GuiPageFrameCat3D* pageFrameLoadGame = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderLoadGame, m_renderableText);
    pageFrameLoadGame->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    m_guiPageLoadGame->AddChild(pageFrameLoadGame);

    GuiElementList* elementListRestoreGame = new GuiElementList(playerInput, 6, 80, 60, 11, nullptr, browseMenuSound);
    elementListRestoreGame->SetId(restoreGameListId);
    if (savedGames.size() > 0)
    {
        int16_t savedGameIndex = 0;
        for (const std::string& savedGame : savedGames)
        {
            elementListRestoreGame->AddChild(new GuiElementSaveSlotStaticCat3D(playerInput, savedGame, { GuiActionRestoreGame, savedGameIndex }, m_renderableText, m_flashIcon));
            savedGameIndex++;
        }
        m_guiPageLoadGame->AddChild(elementListRestoreGame, 80, 60);
    }

    // Save game menu
    m_guiPageSaveGame = new GuiPage(playerInput);
    m_guiPageSaveGame->SetId(pageSaveGameId);

    GuiPageFrameCat3D* pageFrameSaveGame = new GuiPageFrameCat3D(playerInput, *egaGraph, GuiPageFrameCat3D::MenuHeaderSaveGame, m_renderableText);
    pageFrameSaveGame->SetInstructions("Arrows move", "Enter selects", "Esc to back out");
    m_guiPageSaveGame->AddChild(pageFrameSaveGame);

    GuiElementList* elementListSaveGame = new GuiElementList(playerInput, 6, 80, 60, 11, nullptr, browseMenuSound);
    GuiElementSaveSlotEditableCat3D* saveGameEditText = new GuiElementSaveSlotEditableCat3D(playerInput, m_newSaveGameName, "<< new saved game >>", 20, m_renderableText, GuiEvent({ GuiActionSaveGame, -1 }), m_flashIcon);
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
        m_guiPageSaveGame->AddChild(elementListSaveGame, 80, 60);
    }
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
    if (m_askForQuit)
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
    else if (m_subMenuSelected == subMenuVideo)
    {
        m_guiPageVideo->ProcessInput();
    }
    else if (m_subMenuSelected == subMenuControls)
    {
        m_guiPageControls->ProcessInput();
    }
    else if (m_subMenuSelected == subMenuSaveGame)
    {
        const GuiEvent& guiEvent = m_guiPageSaveGame->ProcessInput();
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
            }
        }
    }
    else if (m_subMenuSelected == subMenuRestoreGame)
    {
        const GuiEvent& guiEvent = m_guiPageLoadGame->ProcessInput();
        if (guiEvent.guiAction == GuiActionRestoreGame)
        {
            m_newSaveGameName = m_savedGames.at(guiEvent.guiParameter);
            if (m_saveGameEnabled)
            {
                m_askForOverwrite = false;
                m_askForEndGame = true;
            }
            else
            {
                command = MenuCommandLoadGame;
            }
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

    if (playerInput.IsKeyJustPressed(SDLK_RETURN) &&
        m_subMenuSelected != subMenuRestoreGame &&
        m_subMenuSelected != subMenuSaveGame &&
        m_subMenuSelected != subMenuVideo &&
        m_subMenuSelected != subMenuControls)
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
    }
}

MenuCommand Catacomb3DMenu::EnterKeyPressed()
{
    MenuCommand command = MenuCommandNone;
    if (m_subMenuSelected == subMenuMain) // Main menu
    {
        if (m_menuItemSelected == menuItemMainNewGame)
        {
            // New game
            m_subMenuSelected = subMenuNewGame;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == menuItemMainRestoreGame && m_savedGames.size() > 0)
        {
            m_subMenuSelected = subMenuRestoreGame;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == menuItemMainSaveGame && m_saveGameEnabled)
        {
            m_subMenuSelected = subMenuSaveGame;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == menuItemMainConfigure)
        {
            // Configure
            m_subMenuSelected = subMenuConfigure;
            m_menuItemSelected = 0;
        }
        else if (m_menuItemSelected == menuItemMainReturnToDemo)
        {
            // Return to demo or game
            m_menuItemSelected = 0;
            command = MenuCommandCloseMenu;
        }
        else if (m_menuItemSelected == menuItemMainEndGame && m_saveGameEnabled)
        {
            // End game
            m_askForEndGame = true;
        }
        else if (m_menuItemSelected == menuItemMainSkullNBones)
        {
            // Skull 'n' Bones
            m_subMenuSelected = subMenuSkullNBones;
            m_menuItemSelected = 0;
            m_skullNBones.Reset();
        }
        else if (m_menuItemSelected == menuItemMainQuit)
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
    else if (m_subMenuSelected == subMenuSound)
    {
        m_configurationSettings.GetCVarEnumMutable(CVarIdSoundMode).SetItemIndex(m_menuItemSelected);
    }
    else if (m_subMenuSelected == subMenuMusic)
    {
        m_configurationSettings.GetCVarEnumMutable(CVarIdMusicMode).SetItemIndex(m_menuItemSelected);
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
    m_flashIcon = flashIcon;
    if (m_subMenuSelected == subMenuMain)
    {
        RenderableTiles renderableTiles(*egaGraph->GetTilesSize8());
        RenderableText renderableText(*egaGraph->GetFont(4));
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_MAINMENUPIC), 80, 48);
        renderableTiles.DrawListBullet(112, 62, true, (m_menuItemSelected == menuItemMainNewGame) && flashIcon);
        renderableText.LeftAligned("NEW GAME", (m_menuItemSelected == menuItemMainNewGame) ? EgaBrightRed : EgaRed, 120, 63);
        renderableTiles.DrawListBullet(112, 70, m_savedGames.size() > 0, (m_menuItemSelected == menuItemMainRestoreGame) && flashIcon);
        renderableText.LeftAligned("LOAD GAME", (m_menuItemSelected == menuItemMainRestoreGame) ? EgaBrightRed : EgaRed, 120, 71);
        renderableTiles.DrawListBullet(112, 78, m_saveGameEnabled, (m_menuItemSelected == menuItemMainSaveGame) && flashIcon);
        renderableText.LeftAligned("SAVE GAME", (m_menuItemSelected == menuItemMainSaveGame) ? EgaBrightRed : EgaRed, 120, 79);
        renderableTiles.DrawListBullet(112, 86, true, (m_menuItemSelected == menuItemMainConfigure) && flashIcon);
        renderableText.LeftAligned("CONFIGURE", (m_menuItemSelected == menuItemMainConfigure) ? EgaBrightRed : EgaRed, 120, 87);
        renderableTiles.DrawListBullet(112, 94, true, (m_menuItemSelected == menuItemMainReturnToDemo) && flashIcon);
        const char* returnOptionText = (m_saveGameEnabled) ? "RETURN TO GAME" : "RETURN TO DEMO";
        renderableText.LeftAligned(returnOptionText, (m_menuItemSelected == menuItemMainReturnToDemo) ? EgaBrightRed : EgaRed, 120, 95);
        renderableTiles.DrawListBullet(112, 102, m_saveGameEnabled, (m_menuItemSelected == menuItemMainEndGame) && flashIcon);
        renderableText.LeftAligned("END GAME", (m_menuItemSelected == menuItemMainEndGame) ? EgaBrightRed : EgaRed, 120, 103);
        renderableTiles.DrawListBullet(112, 110, true, (m_menuItemSelected == menuItemMainSkullNBones) && flashIcon);
        renderableText.LeftAligned("SKULL 'N' BONES", (m_menuItemSelected == menuItemMainSkullNBones) ? EgaBrightRed : EgaRed, 120, 111);
        renderableTiles.DrawListBullet(112, 118, true, (m_menuItemSelected == menuItemMainQuit) && flashIcon);
        renderableText.LeftAligned("QUIT", (m_menuItemSelected == menuItemMainQuit) ? EgaBrightRed : EgaRed, 120, 119);

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
        m_renderableText.Reset();
        m_renderableTiles.Reset();
        m_guiPageVideo->Draw(renderer, 0, 0, false);
        renderer.RenderText(m_renderableText);
        renderer.RenderTiles(m_renderableTiles);
        renderer.RenderText(m_renderableText);
    }
    else if (m_subMenuSelected == subMenuControls)
    {
        m_renderableText.Reset();
        m_renderableTextDefaultFont.Reset();
        m_renderableTiles.Reset();
        m_guiPageControls->Draw(renderer, 0, 0, false);
        renderer.RenderTiles(m_renderableTiles);
        renderer.RenderText(m_renderableText);
        renderer.RenderText(m_renderableTextDefaultFont);
    }
    else if (m_subMenuSelected == subMenuSound)
    {
        RenderableTiles renderableTiles(*egaGraph->GetTilesSize8());
        RenderableText renderableText(*egaGraph->GetFont(4));
        renderer.Render2DBar(77, 55, 154, 1, EgaBrightRed);
        renderer.Render2DBar(77, 133, 154, 1, EgaBrightRed);
        renderer.Render2DPicture(egaGraph->GetPicture(CP_SOUNDMENUPIC), 80, 48);

        const uint8_t soundMode = m_configurationSettings.GetCVarEnum(CVarIdSoundMode).GetItemIndex();
        renderableTiles.DrawRadioButton(88, 62, (soundMode == CVarItemIdSoundModeOff), (m_menuItemSelected == 0) && flashIcon);
        renderableText.LeftAligned("NO SOUND EFFECTS", (m_menuItemSelected == 0) ? EgaBrightRed : EgaRed, 96, 63);
        renderableTiles.DrawRadioButton(88, 70, (soundMode == CVarItemIdSoundModePCSpeaker), (m_menuItemSelected == 1) && flashIcon);
        renderableText.LeftAligned("PC SPEAKER", (m_menuItemSelected == 1) ? EgaBrightRed : EgaRed, 96, 71);
        renderableTiles.DrawRadioButton(88, 78, (soundMode == CVarItemIdSoundModeAdlib), (m_menuItemSelected == 2) && flashIcon);
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

        const bool musicOn = m_configurationSettings.GetCVarEnum(CVarIdMusicMode).GetItemIndex() == CVarItemIdMusicModeAdlib;
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
        m_renderableText.Reset();
        m_renderableTextDefaultFont.Reset();
        m_renderableTiles.Reset();
        m_guiPageLoadGame->Draw(renderer, 0, 0, false);
        renderer.RenderTiles(m_renderableTiles);
        renderer.RenderText(m_renderableText);
        renderer.RenderText(m_renderableTextDefaultFont);

        if (m_askForEndGame)
        {
            DrawConfirmationDialog(renderer, *egaGraph, 142, "YOU'RE IN A GAME", "PRESS Y TO LOAD GAME", "ESC TO BACK OUT");
        }
    }
    else if (m_subMenuSelected == subMenuSaveGame)
    {
        m_renderableText.Reset();
        m_renderableTextDefaultFont.Reset();
        m_renderableTiles.Reset();
        m_guiPageSaveGame->Draw(renderer, 0, 0, false);
        renderer.RenderTiles(m_renderableTiles);
        renderer.RenderText(m_renderableText);
        renderer.RenderText(m_renderableTextDefaultFont);

        //const char* instructionText = m_waitingForNewSaveGameName ? "Type name" : "Arrows move";
        //renderableText.LeftAligned(instructionText, EgaRed, 78, 135);
        //renderableText.LeftAligned("Enter accepts", EgaRed, 163, 135);
        //renderableText.Centered("Esc to back out", EgaRed, 154, 144);
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

void Catacomb3DMenu::AddNewSavedGame(const PlayerInput& playerInput, const std::string& name)
{
    m_guiPageLoadGame->AddChild(new GuiElementSaveSlotStaticCat3D(playerInput, name, { GuiActionRestoreGame, (int16_t)(m_savedGames.size() - 1) }, m_renderableText, m_flashIcon), 0, 0, restoreGameListId);
    m_guiPageSaveGame->AddChild(new GuiElementSaveSlotStaticCat3D(playerInput, name, { GuiActionSaveGame, (int16_t)(m_savedGames.size() - 1) }, m_renderableText, m_flashIcon), 0, 0, saveGameListId);
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
}

void Catacomb3DMenu::OpenSaveGameMenu()
{
    m_menuActive = true;
    m_menuItemSelected = 0;
    m_subMenuSelected = subMenuSaveGame;
    m_menuItemOffset = 0;
}

void Catacomb3DMenu::OpenSoundMenu()
{
    m_menuActive = true;
    m_menuItemSelected = 0;
    m_subMenuSelected = subMenuSound;
    m_menuItemOffset = 0;
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
