// Copyright (C) 2026 Arno Ansems
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

#include "GuiElementClickable.h"
#include "PlayerInput.h"
#include "SDL3/SDL_mouse.h"

GuiElementClickable::GuiElementClickable(const PlayerInput& playerInput, uint16_t elementWidth, uint16_t elementHeight) :
    GuiElementBase(playerInput),
    m_elementWidth(elementWidth),
    m_elementHeight(elementHeight)
{

}

bool GuiElementClickable::isClicked() const
{
    const int32_t mouseX = m_playerInput.GetMouseXPos();
    const int32_t mouseY = m_playerInput.GetMouseYPos();
    const bool isJustClickedByMouse =
        m_playerInput.IsMouseButtonJustPressed(SDL_BUTTON_LEFT) &&
        mouseX >= m_originX &&
        mouseX < m_originX + m_elementWidth &&
        mouseY >= m_originY &&
        mouseY < m_originY + m_elementHeight;

    return isJustClickedByMouse;
}