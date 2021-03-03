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

//
// IMenu
//
// Interface for abstracting menu interaction.
//
#pragma once

#include <stdint.h>
#include "PlayerInput.h"
#include "IRenderer.h"
#include "EgaGraph.h"

enum MenuCommand
{
    MenuCommandNone,
    MenuCommandStartNewGame,
    MenuCommandStartNewGameEasy,
    MenuCommandStartNewGameNormal,
    MenuCommandStartNewGameHard,
    MenuCommandExitGame,
    MenuCommandSaveGame,
    MenuCommandLoadGame,
    MenuCommandEndGame,
    MenuCommandCloseMenu
};

class IMenu
{
public:
    virtual ~IMenu() {};
    virtual bool IsActive() const = 0;
    virtual void SetActive(bool active) = 0;
    virtual MenuCommand ProcessInput(const PlayerInput& playerInput) = 0;
    virtual void Draw(IRenderer& renderer, EgaGraph* const egaGraph, const uint16_t menuCursorPic, const uint32_t timeStamp) = 0;
    virtual void SetSaveGameEnabled(const bool enabled) = 0;
    virtual const std::string& GetNewSaveGameName() const = 0;
    virtual void AddNewSavedGame(const PlayerInput& playerInput, const std::string& name) = 0;
    virtual void OpenRestoreGameMenu() = 0;
    virtual void OpenSaveGameMenu() = 0;
    virtual void OpenSoundMenu() = 0;
    virtual void CheckHighScore(const uint16_t level, const uint32_t score) = 0;
};