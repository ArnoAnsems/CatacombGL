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

#include "GuiElementButtonCat3D.h"

GuiElementButtonCat3D::GuiElementButtonCat3D(
    const PlayerInput& playerInput,
    const std::string& buttonLabel,
    const GuiEvent& guiEventWhenActivated,
    RenderableText& renderableText,
    RenderableTiles& renderableTiles,
    bool& flashIcon) :
    GuiElementButton(playerInput, buttonLabel, guiEventWhenActivated, renderableText),
    m_renderableTiles(renderableTiles),
    m_flashIcon(flashIcon)
{

}

void GuiElementButtonCat3D::Draw(IRenderer& renderer) const
{
    m_renderableTiles.DrawListBullet(m_originX, m_originY, m_enabled, m_selected && m_flashIcon);
    m_renderableText.LeftAligned(m_buttonLabel, m_selected ? EgaBrightRed : EgaRed, m_originX + 8, m_originY + 1);
}