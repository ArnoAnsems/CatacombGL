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

#include "GuiElementEditText.h"
#include "..\..\ThirdParty\SDL\include\SDL_keyboard.h"

GuiElementEditText::GuiElementEditText(
    const PlayerInput& playerInput,
    std::string& outputText,
    const std::string& initialText,
    RenderableText& renderableText,
    GuiEvent& textCompleteAction) :
    GuiElementBase(playerInput),
    m_outputText(outputText),
    m_initialText(initialText),
    m_renderableText(renderableText),
    m_textCompleteAction(textCompleteAction),
    m_enteringText(false)
{

}

GuiElementEditText::~GuiElementEditText()
{

}

const GuiEvent& GuiElementEditText::ProcessInput()
{
    bool isTextComplete = false;
    const SDL_Keycode keyCode = m_playerInput.GetFirstKeyPressed();
    if (keyCode == SDLK_RETURN)
    {
        if (m_enteringText)
        {
            if (!m_outputText.empty())
            {
                isTextComplete = true;
            }
        }
        else
        {
            m_enteringText = true;
            m_outputText = "";
        }
    }
    else
    {
        const uint16_t maxTextLength = 20;
        // Check which key is pressed
        if (KeyIsSuitableForText(keyCode) && m_outputText.length() < maxTextLength)
        {
            m_outputText += std::string(SDL_GetKeyName(keyCode));
        }
        else if (keyCode == SDLK_BACKSPACE && !m_outputText.empty())
        {
            m_outputText.pop_back();
        }
    }

    return (isTextComplete) ? m_textCompleteAction : GetEvent();
}

void GuiElementEditText::Draw(IRenderer& renderer, const int16_t originX, const int16_t originY, const bool selected) const
{
    const egaColor color = GetMenuItemColor(selected, true);
    if (m_enteringText)
    {
        const std::string text = m_outputText + "_";
        m_renderableText.LeftAligned(text, color, originX, originY);
    }
    else
    {
        m_renderableText.LeftAligned(m_initialText, color, originX, originY);
    }
}

bool GuiElementEditText::KeyIsSuitableForText(const SDL_Keycode keyCode)
{
    return ((keyCode >= SDLK_a && keyCode <= SDLK_z) ||
        (keyCode >= SDLK_0 && keyCode <= SDLK_9));
}