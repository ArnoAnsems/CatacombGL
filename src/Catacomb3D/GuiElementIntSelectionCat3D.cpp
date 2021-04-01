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

#include "GuiElementIntSelectionCat3D.h"

GuiElementIntSelectionCat3D::GuiElementIntSelectionCat3D(
    const PlayerInput& playerInput,
    ConsoleVariableInt& cvarInt,
    const int16_t offsetXValue,
    RenderableText& renderableText,
    RenderableTiles& renderableTiles,
    bool& flashIcon) :
    GuiElementIntSelection (playerInput, cvarInt, offsetXValue, renderableText),
    m_renderableTiles(renderableTiles),
    m_flashIcon(flashIcon)
{

}

void GuiElementIntSelectionCat3D::Draw(IRenderer& renderer) const
{
    m_renderableTiles.DrawListBullet(m_originX, m_originY, m_enabled, m_selected && m_flashIcon);
    m_renderableText.LeftAligned(m_cvarInt.GetNameInMenu(), m_selected ? EgaBrightRed : EgaRed, m_originX + 8, m_originY + 1);
    const std::string& str = (!m_enabled) ? "Not support." : std::to_string(m_cvarInt.GetValue());
    m_renderableText.LeftAligned(str, m_selected ? EgaLightGray : EgaDarkGray, m_originX + m_offsetXValue, m_originY + 1);
}