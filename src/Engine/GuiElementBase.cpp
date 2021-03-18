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

#include "GuiElementBase.h"

GuiElementBase::GuiElementBase(const PlayerInput& playerInput) :
    m_playerInput(playerInput),
    m_guiEvent({GuiActionNone, 0}),
    m_enabled(true)
{

}

GuiElementBase::~GuiElementBase()
{

}

void GuiElementBase::SetId(const int16_t id)
{
    m_id = id;
}

int16_t GuiElementBase::GetId() const
{
    return m_id;
}

void GuiElementBase::AddChild(GuiElementBase*, const int16_t, const int16_t, const int16_t)
{

}

const GuiEvent& GuiElementBase::ProcessInput()
{
    return m_guiEvent;
}

void GuiElementBase::Draw(IRenderer& /*renderer*/, const int16_t /*originX*/, const int16_t /*originY*/, const bool /*selected*/) const
{

}

void GuiElementBase::SetEnabled(const bool enabled, const int16_t id)
{
    if (id == m_id || id == 0)
    {
        m_enabled = enabled;
    }
}

void GuiElementBase::SetEvent(const uint8_t guiAction, const int16_t guiParameter)
{
    m_guiEvent.guiAction = guiAction;
    m_guiEvent.guiParameter = guiParameter;
}

const GuiEvent& GuiElementBase::GetEvent() const
{
    return m_guiEvent;
}

egaColor GuiElementBase::GetMenuItemColor(const bool selected, const bool supported)
{
    return !supported ? EgaDarkGray : selected ? EgaBrightCyan : EgaBrightWhite;
}