// Copyright (C) 2021 Arno Ansems
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
#pragma once

#include "PlayerInput.h"
#include "IRenderer.h"
#include "EgaColor.h"

const uint8_t GuiActionNone = 0;
const uint8_t GuiActionKeyBinding = 1;
const uint8_t GuiActionLoadGame = 2;
const uint8_t GuiActionBusy = 3;
const uint8_t GuiActionSaveGame = 4;
const uint8_t GuiActionNavigateTo = 5;
const uint8_t GuiActionNewGame = 6;
const uint8_t GuiActionNewGameEasy = 7;
const uint8_t GuiActionNewGameNormal = 8;
const uint8_t GuiActionNewGameHard = 9;
const uint8_t GuiActionClose = 10;
const uint8_t GuiActionQuit = 11;
const uint8_t GuiActionPlaySound = 12;
const uint8_t GuiActionEndGame = 13;
const uint8_t GuiActionHighScoreFinished = 14;
const uint8_t GuiActionResetToDefaults = 15;
const uint8_t GuiActionResetToClassic = 16;

struct GuiEvent
{
    uint8_t guiAction;
    int16_t guiParameter;
};

class GuiElementBase
{
public:
    GuiElementBase(const PlayerInput& playerInput);
    virtual ~GuiElementBase();
    void SetId(const int16_t id);
    int16_t GetId() const;

    virtual void AddChild(GuiElementBase* child, const int16_t offsetX = 0, const int16_t offsetY = 0, const int16_t parentId = 0);
    virtual const GuiEvent& ProcessInput();
    virtual void Draw(IRenderer& renderer) const;
    virtual void SetEnabled(const bool enabled, const int16_t id = 0);
    virtual void SetOrigin(const int16_t originX, const int16_t originY);
    virtual void SetSelected(const bool selected);
    virtual const std::string& GetLabel() const;

protected:
    void SetEvent(const uint8_t guiAction, const int16_t guiParameter);
    const GuiEvent& GetEvent() const;

    static egaColor GetMenuItemColor(const bool selected, const bool supported);

    const PlayerInput& m_playerInput;
    GuiEvent m_guiEvent;
    int16_t m_id;
    bool m_enabled;
    int16_t m_originX;
    int16_t m_originY;
    bool m_selected;

private:
    const std::string m_emptyLabel = "";
};
