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

#include "GuiElementBindKeyCat3D.h"

GuiElementBindKeyCat3D::GuiElementBindKeyCat3D(
    const PlayerInput& playerInput,
    ControlsMap& controlsMap,
    ControlAction controlAction,
    const int16_t offsetXValue,
    RenderableText& renderableText,
    RenderableTiles& renderableTiles,
    bool& flashIcon) :
    GuiElementBindKey(playerInput, controlsMap, controlAction, offsetXValue, renderableText),
    m_renderableTiles(renderableTiles),
    m_flashIcon(flashIcon)
{

}

void GuiElementBindKeyCat3D::Draw(IRenderer& renderer) const
{
    m_renderableTiles.DrawListBullet(m_originX, m_originY, m_enabled, m_selected && m_flashIcon);
    const std::string& actionLabel = m_controlsMap.GetActionLabels().at(m_controlAction);
    m_renderableText.LeftAligned(actionLabel, m_selected ? EgaBrightRed : EgaRed, m_originX + 8, m_originY + 1);
    if (m_waitingForKey && m_selected)
    {
        m_renderableText.LeftAligned("< Press key to bind >", EgaLightGray, m_originX + m_offsetXValue, m_originY + 1);
    }
    else
    {
        // The name of the keys is shown with the default font, as the original font from the game lacks some required characters.
        m_renderableText.LeftAligned(m_controlsMap.GetKeyStringFromAction(m_controlAction), m_selected ? EgaLightGray : EgaDarkGray, m_originX + m_offsetXValue, m_originY + 1);
    }
}