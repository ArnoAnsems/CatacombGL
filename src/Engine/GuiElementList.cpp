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
#include "Picture.h"
#include "PlayerInput.h"
#include "IRenderer.h"
#include "SDL_mouse.h"
#include <string>
#include <cmath>
#include <algorithm>

GuiElementList::GuiElementList(
    const PlayerInput& playerInput,
    const uint16_t maxElementsDrawn,
    const uint16_t elementHeight,
    const Picture* cursorPicture,
    const uint16_t soundWhenBrowsing) :
    GuiElementBase(playerInput),
    m_scrollbarWidth(10u),
    m_scrollbarHeight(78u),
    m_scrollbarOffsetX(200),
    m_maxElementsDrawn(maxElementsDrawn),
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
    if (m_elements.empty())
    {
        return GetEvent();
    }

    GuiElementBase* element = m_elements.at(m_elementSelected);
    const GuiEvent& guiEvent = element->ProcessInput();
    bool makeBrowseSound = false;

    if (guiEvent.guiAction == GuiActionNone)
    {
        // The selected element didn't perform any action with the given input.
        // Check if the list itself can do something with the input.

        const bool isUpJustActivated =
            m_playerInput.IsKeyJustPressed(SDLK_UP) ||
            m_playerInput.IsGameControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_DPAD_UP) ||
            m_playerInput.GetGameControllerAxisJustPressedTowardsNegative(SDL_CONTROLLER_AXIS_LEFTY) ||
            m_playerInput.GetGameControllerAxisJustPressedTowardsNegative(SDL_CONTROLLER_AXIS_RIGHTY);
        const bool isDownJustActivated =
            m_playerInput.IsKeyJustPressed(SDLK_DOWN) ||
            m_playerInput.IsGameControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_DPAD_DOWN) ||
            m_playerInput.GetGameControllerAxisJustPressedTowardsPositive(SDL_CONTROLLER_AXIS_LEFTY) ||
            m_playerInput.GetGameControllerAxisJustPressedTowardsPositive(SDL_CONTROLLER_AXIS_RIGHTY);

        if (isUpJustActivated)
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

            makeBrowseSound = true;
        }
        else if (isDownJustActivated)
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

            makeBrowseSound = true;
        }
        else
        {
            const uint16_t numberOfElements = (uint16_t)m_elements.size();
            if (numberOfElements > 1)
            {
                // Check keys a through z to select an element based on the first letter of the label
                bool elementFound = false;
                uint16_t elementToCheck = (m_elementSelected + 1) % numberOfElements;
                while (elementToCheck != m_elementSelected && !elementFound)
                {
                    const std::string& label = m_elements.at(elementToCheck)->GetLabel();
                    if (!label.empty())
                    {
                        const char firstLetter = label.at(0);
                        const char firstLetterLowerCase = (firstLetter >= 'A' && firstLetter <= 'Z') ? firstLetter - 'A' + 'a' : firstLetter;
                        const SDL_Keycode keyToCheck = firstLetterLowerCase - 'a' + SDLK_a;

                        if (m_playerInput.IsKeyJustPressed(keyToCheck))
                        {
                            elementFound = true;
                        }
                    }

                    if (!elementFound)
                    {
                        elementToCheck = (elementToCheck + 1) % numberOfElements;
                    }
                }

                if (elementFound && m_elementSelected != elementToCheck)
                {
                    m_elementSelected = elementToCheck;
                    m_firstElementDrawn = (m_elementSelected >= m_maxElementsDrawn) ? m_elementSelected - m_maxElementsDrawn + 1 : 0;
                    makeBrowseSound = true;
                }
            }
        }

        const int32_t mouseX = m_playerInput.GetMouseXPos();
        const int32_t mouseY = m_playerInput.GetMouseYPos();

        if (m_playerInput.GetRelativeMouseXPos() != 0 || m_playerInput.GetRelativeMouseYPos() != 0)
        {
            // The mouse moved. Check to which element the mouse is pointing to, if any.
            uint16_t index = 0;
            const size_t elementsInView = (m_elements.size() > m_maxElementsDrawn) ? m_maxElementsDrawn : m_elements.size();
            while (index < elementsInView)
            {
                constexpr uint16_t elementWidth = 140u;
                const int16_t offsetY = m_originY + (index * m_elementHeight);
                if (mouseX >= m_originX &&
                    mouseX < m_originX + elementWidth &&
                    mouseY >= offsetY &&
                    mouseY < offsetY + m_elementHeight)
                {
                    const uint8_t itemIndex = index + m_firstElementDrawn;
                    if (itemIndex != m_elementSelected)
                    {
                        m_elementSelected = itemIndex;
                        makeBrowseSound = true;
                    }
                }
                index++;
            }
        }

        if (m_elements.size() > m_maxElementsDrawn)
        {
            const int16_t scrollbarOriginX = m_originX + m_scrollbarOffsetX;
            const int16_t scrollbarOriginY = m_originY;
            const bool isMousePressedAboveScrollbar =
                m_playerInput.IsMouseButtonPressed(SDL_BUTTON_LEFT) &&
                mouseX >= scrollbarOriginX &&
                mouseX < scrollbarOriginX + m_scrollbarWidth &&
                mouseY >= scrollbarOriginY &&
                mouseY < scrollbarOriginY + m_scrollbarHeight;

            if (isMousePressedAboveScrollbar)
            {
                uint16_t elementPointedAt = 0u;
                if (mouseY < scrollbarOriginY + 3u)
                {
                    elementPointedAt = 0u;
                }
                else if (mouseY > scrollbarOriginY + m_scrollbarHeight - 3u)
                {
                    elementPointedAt = m_elements.size() - 1u;
                }
                else
                {
                    elementPointedAt = ((mouseY - scrollbarOriginY - 3u) * m_elements.size()) / (m_scrollbarHeight - 6u);
                }
                const uint16_t halfList = m_maxElementsDrawn / 2u;
                if (elementPointedAt < halfList)
                {
                    m_firstElementDrawn = 0u;
                }
                else
                {
                    m_firstElementDrawn = std::min(elementPointedAt - halfList, static_cast<uint16_t>(m_elements.size()) - m_maxElementsDrawn);
                }

                if (m_elementSelected < m_firstElementDrawn)
                {
                    m_elementSelected = m_firstElementDrawn;
                }
                else if (m_elementSelected > m_firstElementDrawn + m_maxElementsDrawn - 1u)
                {
                    m_elementSelected = m_firstElementDrawn + m_maxElementsDrawn - 1u;
                }
            }
        }
    }

    // Update origin of elements
    uint16_t index = 0;
    const size_t elementsToDraw = (m_elements.size() > m_maxElementsDrawn) ? m_maxElementsDrawn : m_elements.size();
    while (index < elementsToDraw)
    {
        const int16_t offsetY = m_originY + (index * m_elementHeight);
        const uint8_t itemIndex = index + m_firstElementDrawn;
        GuiElementBase* element = m_elements.at(itemIndex);
        element->SetOrigin(m_originX, offsetY);
        element->SetSelected(itemIndex == m_elementSelected);
        index++;
    }

    return makeBrowseSound ? m_guiEventPlayBrowseSound : guiEvent;
}

void GuiElementList::Draw(IRenderer& renderer) const
{
    if (m_cursorPicture != nullptr)
    {
        renderer.Render2DPicture(m_cursorPicture, m_originX - 40, 4 + ((m_elementSelected - m_firstElementDrawn) * m_elementHeight));
    }

    uint16_t index = 0;
    const size_t elementsToDraw = (m_elements.size() > m_maxElementsDrawn) ? m_maxElementsDrawn : m_elements.size();
    while (index < elementsToDraw)
    {
        const uint8_t itemIndex = index + m_firstElementDrawn;
        const GuiElementBase* element = m_elements.at(itemIndex);
        element->Draw(renderer);
        index++;
    }

    DrawScrollbar(renderer);
}

void GuiElementList::DrawScrollbar(IRenderer& renderer) const
{
    if (m_elements.size() > m_maxElementsDrawn)
    {
        const uint16_t scrollbarInnerHeight = m_scrollbarHeight - 6u;
        const int16_t scrollbarOriginX = m_originX + m_scrollbarOffsetX;
        const int16_t scrollbarOriginY = m_originY;

        // Top border
        renderer.Render2DBar(scrollbarOriginX, scrollbarOriginY, m_scrollbarWidth, 1u, EgaBrightWhite);
        renderer.Render2DBar(scrollbarOriginX + 2u, scrollbarOriginY + 2u, m_scrollbarWidth - 4u, 1u, EgaBlue);

        // Right border
        renderer.Render2DBar(scrollbarOriginX + m_scrollbarWidth - 1u, scrollbarOriginY + 1u, 1u, m_scrollbarHeight - 1u, EgaBrightWhite);
        renderer.Render2DBar(scrollbarOriginX + m_scrollbarWidth - 3u, scrollbarOriginY + 3u, 1u, m_scrollbarHeight - 5u, EgaBlue);

        // Left border
        renderer.Render2DBar(scrollbarOriginX, scrollbarOriginY + 1u, 1u, m_scrollbarHeight - 1u, EgaDarkGray);
        renderer.Render2DBar(scrollbarOriginX + 2u, scrollbarOriginY + 3u, 1u, m_scrollbarHeight - 5u, EgaBrightCyan);

        // Bottom border
        renderer.Render2DBar(scrollbarOriginX + 1u, scrollbarOriginY + m_scrollbarHeight - 1u, m_scrollbarWidth - 2u, 1u, EgaDarkGray);
        renderer.Render2DBar(scrollbarOriginX + 3u, scrollbarOriginY + m_scrollbarHeight - 3u, m_scrollbarWidth - 6u, 1u, EgaBrightCyan);

        const uint16_t visibleSectionHeight = std::ceil(static_cast<double>(scrollbarInnerHeight * m_maxElementsDrawn) / static_cast<double>(m_elements.size()));
        const uint16_t visibleSectionOffset = scrollbarInnerHeight * m_firstElementDrawn / m_elements.size();
        renderer.Render2DBar(scrollbarOriginX + 3u, scrollbarOriginY + 3u + visibleSectionOffset, m_scrollbarWidth - 6u, visibleSectionHeight, EgaBrightWhite);
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

void GuiElementList::SetEnabled(const bool enabled, const int16_t id)
{
    if (id == GetId() || id == 0)
    {
        // Set enable flag for the whole list
        m_enabled = enabled;
    }
    else
    {
        // Set enable flag for the children in the list
        for (size_t i = 0; i < m_elements.size(); i++)
        {
            m_elements.at(i)->SetEnabled(enabled, id);
        }
    }
}