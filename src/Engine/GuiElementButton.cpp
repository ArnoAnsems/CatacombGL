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

#include "GuiElementButton.h"

GuiElementButton::GuiElementButton(
    const PlayerInput& playerInput,
    const std::string& buttonLabel,
    const GuiEvent& guiEventWhenActivated,
    RenderableText& renderableText) :
    GuiElementBase(playerInput),
    m_buttonLabel(buttonLabel),
    m_guiEventWhenActivated(guiEventWhenActivated),
    m_renderableText(renderableText)
{

}

GuiElementButton::~GuiElementButton()
{

}

const GuiEvent& GuiElementButton::ProcessInput()
{
    return (m_enabled && m_playerInput.IsKeyJustPressed(SDLK_RETURN)) ? m_guiEventWhenActivated : GetEvent();
}

void GuiElementButton::Draw(IRenderer& renderer) const
{
    m_renderableText.LeftAligned(m_buttonLabel, GetMenuItemColor(m_selected, m_enabled), m_originX, m_originY);
}

void GuiElementButton::SetLabel(const std::string& buttonLabel)
{
    m_buttonLabel = buttonLabel;
}