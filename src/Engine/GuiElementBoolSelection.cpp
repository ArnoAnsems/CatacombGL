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
    ConsoleVariableBool& cvarEnum,
    const int16_t offsetXValue,
    RenderableText& renderableText) :
    GuiElementBase(playerInput),
    m_cvarEnum(cvarEnum),
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
        m_cvarEnum.Toggle();
    }

    return GetEvent();
}

void GuiElementBoolSelection::Draw(IRenderer& /*renderer*/, const int16_t originX, const int16_t originY, const bool selected) const
{
    const egaColor color = GetMenuItemColor(selected, m_enabled);
    m_renderableText.LeftAligned(m_cvarEnum.GetNameInMenu(), color, originX, originY);
    const std::string& valueStr = (!m_enabled) ? "Not supported" : m_cvarEnum.GetValueInMenu();
    m_renderableText.LeftAligned(valueStr, color, originX + m_offsetXValue, originY);
}