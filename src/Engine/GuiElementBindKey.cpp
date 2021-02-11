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

#include "GuiElementBindKey.h"

GuiElementBindKey::GuiElementBindKey(
    const PlayerInput& playerInput,
    ControlsMap& controlsMap,
    ControlAction controlAction,
    const int16_t offsetXValue,
    RenderableText& renderableText,
    RenderableText& renderableTexDefaultFont) :
    GuiElementBase(playerInput),
    m_controlsMap(controlsMap),
    m_controlAction(controlAction),
    m_offsetXValue(offsetXValue),
    m_renderableText(renderableText),
    m_renderableTextDefaultFont(renderableTexDefaultFont),
    m_waitingForKey(false)
{

}

GuiElementBindKey::~GuiElementBindKey()
{

}

const GuiEvent& GuiElementBindKey::ProcessInput()
{
    if (m_waitingForKey)
    {
        // Check which key is pressed
        const SDL_Keycode keyCode = m_playerInput.GetFirstKeyPressed();
        if (keyCode != SDLK_UNKNOWN && m_controlsMap.AssignActionToKey(m_controlAction, keyCode))
        {
            m_waitingForKey = false;
        }
        else
        {
            const uint8_t buttonCode = m_playerInput.GetFirstMouseButtonPressed();
            if (buttonCode != 0 && m_controlsMap.AssignActionToMouseButton(m_controlAction, buttonCode))
            {
                m_waitingForKey = false;
            }
        }
    }
    else
    {
        if (m_playerInput.IsKeyJustPressed(SDLK_RETURN))
        {
            m_waitingForKey = true;
            SetEvent(GuiActionKeyBinding, 0);
        }
        else
        {
            SetEvent(GuiActionNone, 0);
        }
    }

    return GetEvent();
}

void GuiElementBindKey::Draw(IRenderer& renderer, const int16_t originX, const int16_t originY, const bool selected) const
{
    const std::string& actionLabel = m_controlsMap.GetActionLabels().at(m_controlAction);
    const egaColor color = GetMenuItemColor(selected, true);
    m_renderableText.LeftAligned(actionLabel, color, originX, originY);
    if (m_waitingForKey && selected)
    {
        m_renderableText.LeftAligned("< Press key to bind >", EgaBrightCyan, originX + m_offsetXValue, originY);
    }
    else
    {
        // The name of the keys is shown with the default font, as the original font from the game lacks some required characters.
        m_renderableTextDefaultFont.LeftAligned(m_controlsMap.GetKeyStringFromAction(m_controlAction), color, originX + m_offsetXValue, originY);
    }
}