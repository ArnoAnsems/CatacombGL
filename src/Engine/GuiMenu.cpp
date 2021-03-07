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

#include "GuiMenu.h"

GuiMenu::GuiMenu(
    const PlayerInput& playerInput) :
    GuiElementBase(playerInput),
    m_elementOpenIndex(0)
{
    m_elements.clear();
    m_previouslyOpen.clear();
}

GuiMenu::~GuiMenu()
{
    for (GuiElementBase* guiElement : m_elements)
    {
        delete guiElement;
    }
    m_elements.clear();
}

const GuiEvent& GuiMenu::ProcessInput()
{
    if (m_elementOpenIndex >= m_elements.size())
    {
        return GetEvent();
    }

    const GuiEvent& guiEvent = m_elements.at(m_elementOpenIndex)->ProcessInput();

    if (guiEvent.guiAction == GuiActionNavigateTo)
    {
        const int16_t targetId = guiEvent.guiParameter;
        const size_t targetIndex = GetIndexFromId(targetId);
        if (targetIndex < m_elements.size())
        {
            m_previouslyOpen.push_back(m_elements.at(m_elementOpenIndex)->GetId());
            m_elementOpenIndex = targetIndex;
        }

        // Higher level Gui element can ignore the navigation action.
        return m_noneEvent;
    }
    else if (guiEvent.guiAction == GuiActionClose && !m_previouslyOpen.empty())
    {
        // Return to previous page
        m_elementOpenIndex = GetIndexFromId(m_previouslyOpen.back());
        m_previouslyOpen.pop_back();

        // Higher level Gui element can ignore the close action.
        return m_noneEvent;
    }

    return guiEvent;
}

void GuiMenu::Draw(IRenderer& renderer, const int16_t originX, const int16_t originY, const bool selected) const
{
    if (m_elementOpenIndex < m_elements.size())
    {
        return m_elements.at(m_elementOpenIndex)->Draw(renderer, originX, originY, selected);
    }
}

void GuiMenu::AddChild(GuiElementBase* child, const int16_t offsetX, const int16_t offsetY, const int16_t parentId)
{
    if (parentId == GetId() || parentId == 0)
    {
        // Add child to this menu
        m_elements.push_back(child);
    }
    else
    {
        // Add child to one of the existing elements in this menu
        for (size_t i = 0; i < m_elements.size(); i++)
        {
            m_elements.at(i)->AddChild(child, offsetX, offsetY, parentId);
        }
    }
}

void GuiMenu::Open(const int16_t id)
{
    const size_t index = GetIndexFromId(id);
    if (index < m_elements.size())
    {
        m_elementOpenIndex = index;
        m_previouslyOpen.clear();
    }
}

size_t GuiMenu::GetIndexFromId(const int16_t id) const
{
    size_t i = 0;
    while (i < m_elements.size())
    {
        if (m_elements.at(i)->GetId() == id)
        {
            break;
        }
        i++;
    }

    return i;
}