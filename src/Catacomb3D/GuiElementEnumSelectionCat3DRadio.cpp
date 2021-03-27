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

#include "GuiElementEnumSelectionCat3DRadio.h"

GuiElementEnumSelectionCat3DRadio::GuiElementEnumSelectionCat3DRadio(
    const PlayerInput& playerInput,
    ConsoleVariableEnum& cvarEnum,
    RenderableText& renderableText,
    RenderableTiles& renderableTiles,
    bool& flashIcon,
    const std::vector<std::string>& enumNames) :
    GuiElementEnumSelectionCat3D(playerInput, cvarEnum, 0, renderableText, renderableTiles, flashIcon),
    m_enumNames(enumNames),
    m_elementSelected(0)
{

}

const GuiEvent& GuiElementEnumSelectionCat3DRadio::ProcessInput()
{
    if (m_enabled)
    {
        if (m_playerInput.IsKeyJustPressed(SDLK_RETURN))
        {
            m_cvarEnum.SetItemIndex(m_elementSelected);
        }
        else if (m_playerInput.IsKeyJustPressed(SDLK_DOWN))
        {
            if (m_elementSelected == m_enumNames.size() - 1)
            {
                m_elementSelected = 0;
            }
            else
            {
                m_elementSelected++;
            }
        }
        else if (m_playerInput.IsKeyJustPressed(SDLK_UP))
        {
            if (m_elementSelected == 0)
            {
                m_elementSelected = (uint8_t)(m_enumNames.size() - 1);
            }
            else
            {
                m_elementSelected--;
            }
        }
    }

    return GetEvent();
}

void GuiElementEnumSelectionCat3DRadio::Draw(IRenderer& renderer, const int16_t originX, const int16_t originY, const bool selected) const
{
    const uint8_t settingActive = m_cvarEnum.GetItemIndex();
    for (uint8_t element = 0; element < m_enumNames.size(); element++)
    {
        const bool elementIsSet = (settingActive == element);
        const bool elementIsSelected = (m_elementSelected == element);
        const bool elementIsFlashing = elementIsSelected && m_flashIcon;
        m_renderableTiles.DrawRadioButton(originX, originY + (element * 8), elementIsSet, elementIsFlashing);
        m_renderableText.LeftAligned(m_enumNames.at(element), elementIsSelected ? EgaBrightRed : EgaRed, originX + 8, originY + 1 + (element * 8));
    }
}