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

#include "GuiElementList.h"

GuiElementList::GuiElementList(
    const PlayerInput& playerInput,
    const uint16_t maxElementsDrawn,
    const int16_t originX,
    const int16_t originY,
    const uint16_t elementHeight,
    const Picture* cursorPicture,
    const uint16_t soundWhenBrowsing) :
    GuiElementBase(playerInput),
    m_maxElementsDrawn(maxElementsDrawn),
    m_originX(originX),
    m_originY(originY),
    m_elementHeight(elementHeight),
    m_elementSelected(0),
    m_firstElementDrawn(0),
    m_cursorPicture(cursorPicture),
    m_guiEventPlayBrowseSound({GuiActionPlaySound, (int16_t)soundWhenBrowsing})
{

}

GuiElementList::~GuiElementList()
{
    for (size_t i = 0; i < m_elements.size(); i++)
    {
        delete m_elements.at(i);
    }
}

const GuiEvent& GuiElementList::ProcessInput()
{
    GuiElementBase* element = m_elements.at(m_elementSelected);
    const GuiEvent& guiEvent = element->ProcessInput();

    if (guiEvent.guiAction == GuiActionNone)
    {
        if (m_playerInput.IsKeyJustPressed(SDLK_UP))
        {
            if (m_elementSelected == 0)
            {
                m_elementSelected = (uint16_t)m_elements.size() - 1;
                m_firstElementDrawn = (m_elementSelected >= m_maxElementsDrawn) ? m_elementSelected - m_maxElementsDrawn + 1 : 0;
            }
            else
            {
                m_elementSelected--;
                if (m_elementSelected < m_firstElementDrawn)
                {
                    m_firstElementDrawn = m_elementSelected;
                }
            }

            return m_guiEventPlayBrowseSound;
        }
        else if (m_playerInput.IsKeyJustPressed(SDLK_DOWN))
        {
            if (m_elementSelected == m_elements.size() - 1)
            {
                m_elementSelected = 0;
                m_firstElementDrawn = 0;
            }
            else
            {
                m_elementSelected++;
                if (m_elementSelected - m_firstElementDrawn > m_maxElementsDrawn - 1)
                {
                    m_firstElementDrawn = m_elementSelected - m_maxElementsDrawn + 1;
                }
            }

            return m_guiEventPlayBrowseSound;
        }
    }

    return guiEvent;
}

void GuiElementList::Draw(IRenderer& renderer, const int16_t originX, const int16_t originY, const bool selected) const
{
    if (m_cursorPicture != nullptr)
    {
        renderer.Render2DPicture(m_cursorPicture, originX - 40, 4 + ((m_elementSelected - m_firstElementDrawn) * m_elementHeight));
    }

    uint16_t index = 0;
    const size_t elementsToDraw = (m_elements.size() > m_maxElementsDrawn) ? m_maxElementsDrawn : m_elements.size();
    while (index < elementsToDraw)
    {
        const int16_t offsetY = originY + (index * m_elementHeight);
        const uint8_t itemIndex = index + m_firstElementDrawn;
        const GuiElementBase* element = m_elements.at(itemIndex);
        element->Draw(renderer, originX, offsetY, itemIndex == m_elementSelected);
        index++;
    }
}

void GuiElementList::AddChild(GuiElementBase* child, const int16_t offsetX, const int16_t offsetY, const int16_t parentId)
{
    if (parentId == GetId() || parentId == 0)
    {
        // Add child to this list
        m_elements.push_back(child);
    }
    else
    {
        // Add child to one of the existing children in the list
        for (size_t i = 0; i < m_elements.size(); i++)
        {
            m_elements.at(i)->AddChild(child, offsetX, offsetY, parentId);
        }
    }
}