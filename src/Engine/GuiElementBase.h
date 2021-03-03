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
const uint8_t GuiActionRestoreGame = 2;
const uint8_t GuiActionBusy = 3;
const uint8_t GuiActionSaveGame = 4;

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
    virtual void Draw(IRenderer& renderer, const int16_t originX, const int16_t originY, const bool selected) const;

protected:
    void SetEvent(const uint8_t guiAction, const int16_t guiParameter);
    const GuiEvent& GetEvent() const;

    static egaColor GetMenuItemColor(const bool selected, const bool supported);

    const PlayerInput& m_playerInput;
    GuiEvent m_guiEvent;
    int16_t m_id;
};
