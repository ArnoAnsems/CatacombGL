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
    MenuCommandCloseMenu
};

class IMenu
{
public:
    virtual ~IMenu() {};
    virtual bool IsActive() const = NULL;
    virtual void SetActive(bool active) = NULL;
    virtual MenuCommand ProcessInput(const PlayerInput& playerInput) = NULL;
    virtual void Draw(IRenderer& renderer, EgaGraph* const egaGraph, const uint16_t menuCursorPic, const uint32_t timeStamp) = NULL;
    virtual void SetSaveGameEnabled(const bool enabled) = NULL;
    virtual const std::string& GetNewSaveGameName() const = NULL;
    virtual void OpenRestoreGameMenu() = NULL;
    virtual void OpenSaveGameMenu() = NULL;
    virtual void OpenSoundMenu() = NULL;
};