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

#include "GuiElementSaveSlotEditableCat3D.h"

GuiElementSaveSlotEditableCat3D::GuiElementSaveSlotEditableCat3D(
    const PlayerInput& playerInput,
    std::string& outputText,
    const std::string& initialText,
    const uint16_t maxTextLength,
    RenderableText& renderableText,
    GuiEvent& textCompleteAction,
    bool& flashIcon,
    GuiPageFrameCat3D* pageFrame) :
    GuiElementEditText(playerInput, outputText, initialText, maxTextLength, renderableText, textCompleteAction),
    m_flashIcon(flashIcon),
    m_pageFrame(pageFrame)
{

}

void GuiElementSaveSlotEditableCat3D::Draw(IRenderer& renderer) const
{
    const bool bright = m_selected && m_flashIcon;
    const egaColor color = bright ? EgaBrightRed : EgaRed;
    if (m_enteringText)
    {
        const std::string text = m_outputText + "_";
        m_renderableText.LeftAligned(text, color, m_originX + 2, m_originY + 2);
    }
    else
    {
        m_renderableText.LeftAligned(m_initialText, color, m_originX + 2, m_originY + 2);
    }

    renderer.Render2DBar(m_originX, m_originY, 148, 1, color);
    renderer.Render2DBar(m_originX, m_originY + 9, 148, 1, color);
    renderer.Render2DBar(m_originX, m_originY + 1, 1, 8, color);
    renderer.Render2DBar(m_originX + 147, m_originY + 1, 1, 8, color);

    if (m_pageFrame != nullptr)
    {
        const std::string firstInstruction = (m_enteringText) ? "Type name" : "Arrows move";
        m_pageFrame->SetInstructions(firstInstruction, "Enter accepts", "Esc to back out");
    }
}