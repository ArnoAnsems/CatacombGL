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

#include "GuiElementBoolSelection.h"

GuiElementBoolSelection::GuiElementBoolSelection(
    const PlayerInput& playerInput,
    ConsoleVariableBool& cvarBool,
    const int16_t offsetXValue,
    RenderableText& renderableText) :
    GuiElementBase(playerInput),
    m_cvarBool(cvarBool),
    m_offsetXValue(offsetXValue),
    m_renderableText(renderableText)
{

}

GuiElementBoolSelection::~GuiElementBoolSelection()
{

}

const GuiEvent& GuiElementBoolSelection::ProcessInput()
{
    if (m_enabled && m_playerInput.IsKeyJustPressed(SDLK_RETURN))
    {
        m_cvarBool.Toggle();
    }

    return GetEvent();
}

void GuiElementBoolSelection::Draw(IRenderer& /*renderer*/) const
{
    const egaColor color = GetMenuItemColor(m_selected, m_enabled);
    m_renderableText.LeftAligned(m_cvarBool.GetNameInMenu(), color, m_originX, m_originY);
    const std::string& valueStr = (!m_enabled) ? "Not supported" : m_cvarBool.GetValueInMenu();
    m_renderableText.LeftAligned(valueStr, color, m_originX + m_offsetXValue, m_originY);
}

const std::string& GuiElementBoolSelection::GetLabel() const
{
    return m_cvarBool.GetNameInMenu();
}