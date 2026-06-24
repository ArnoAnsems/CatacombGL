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
#include "PlayerInput.h"
#include "RenderableText.h"
#include "SDL3/SDL_mouse.h"

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

const GuiEvent& GuiElementButton::ProcessInput()
{
    const int32_t mouseX = m_playerInput.GetMouseXPos();
    const int32_t mouseY = m_playerInput.GetMouseYPos();
    const bool isJustActivatedByMouse = 
        m_playerInput.IsMouseButtonJustPressed(SDL_BUTTON_LEFT) &&
        mouseX >= m_originX &&
        mouseX < m_originX + 120 &&
        mouseY >= m_originY &&
        mouseY < m_originY + 10;

    const bool isJustActivated =
        m_playerInput.IsKeyJustPressed(SDLK_RETURN) ||
        m_playerInput.IsGameControllerButtonJustPressed(SDL_GAMEPAD_BUTTON_SOUTH) ||
        isJustActivatedByMouse;

    return (m_enabled && isJustActivated) ? m_guiEventWhenActivated : GetEvent();
}

void GuiElementButton::Draw(IRenderer& renderer) const
{
    m_renderableText.LeftAligned(m_buttonLabel, GetMenuItemColor(m_selected, m_enabled), m_originX, m_originY);
}

void GuiElementButton::SetLabel(const std::string& buttonLabel)
{
    m_buttonLabel = buttonLabel;
}

const std::string& GuiElementButton::GetLabel() const
{
    return m_buttonLabel;
}