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

#include "GuiPage.h"

GuiPage::GuiPage(
    const PlayerInput& playerInput) :
    GuiElementBase(playerInput)
{
    m_elements.clear();
}

GuiPage::~GuiPage()
{
    for (PageElement& pageElement : m_elements)
    {
        delete pageElement.element;
    }
    m_elements.clear();
}

const GuiEvent& GuiPage::ProcessInput()
{
    for (PageElement& pageElement : m_elements)
    {
        const GuiEvent& elementGuiEvent = pageElement.element->ProcessInput();
        if (elementGuiEvent.guiAction != GuiActionNone)
        {
            return elementGuiEvent;
        }
    }

    if (m_playerInput.IsKeyJustPressed(SDLK_ESCAPE))
    {
        return m_closeEvent;
    }

    return GetEvent();
}

void GuiPage::Draw(IRenderer& renderer, const int16_t originX, const int16_t originY, const bool selected) const
{
    for (const PageElement& pageElement : m_elements)
    {
        pageElement.element->Draw(renderer, originX + pageElement.offsetX, originY + pageElement.offsetY, selected);
    }
}

void GuiPage::AddChild(GuiElementBase* child, const int16_t offsetX, const int16_t offsetY, const int16_t parentId)
{
    if (parentId == GetId() || parentId == 0)
    {
        // Add child to this page
        PageElement pageElement{ child, offsetX, offsetY };
        m_elements.push_back(pageElement);
    }
    else
    {
        // Add child to one of the existing elements on this page
        for (size_t i = 0; i < m_elements.size(); i++)
        {
            m_elements.at(i).element->AddChild(child, offsetX, offsetY, parentId);
        }
    }
}
