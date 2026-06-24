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
#include "RenderableText.h"
#include "PlayerInput.h"
#include "SDL3/SDL_mouse.h"

GuiElementBindKey::GuiElementBindKey(
    const PlayerInput& playerInput,
    ControlsMap& controlsMap,
    ControlAction controlAction,
    const int16_t offsetXValue,
    RenderableText& renderableText) :
    GuiElementBase(playerInput),
    m_controlsMap(controlsMap),
    m_controlAction(controlAction),
    m_offsetXValue(offsetXValue),
    m_renderableText(renderableText),
    m_waitingForKey(false)
{

}

const GuiEvent& GuiElementBindKey::ProcessInput()
{
    if (m_enabled)
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
            const int32_t mouseX = m_playerInput.GetMouseXPos();
            const int32_t mouseY = m_playerInput.GetMouseYPos();
            const bool isJustActivatedByMouse =
                m_playerInput.IsMouseButtonJustPressed(SDL_BUTTON_LEFT) &&
                mouseX >= m_originX + m_offsetXValue &&
                mouseX < m_originX + m_offsetXValue + 70 &&
                mouseY >= m_originY &&
                mouseY < m_originY + 10;

            if (m_playerInput.IsKeyJustPressed(SDLK_RETURN) ||
                isJustActivatedByMouse ||
                m_playerInput.IsGameControllerButtonJustPressed(SDL_GAMEPAD_BUTTON_SOUTH))
            {
                m_waitingForKey = true;
                SetEvent(GuiActionKeyBinding, 0);
            }
            else
            {
                SetEvent(GuiActionNone, 0);
            }
        }
    }
    else
    {
        // Disabled
        SetEvent(GuiActionNone, 0);
    }

    return GetEvent();
}

void GuiElementBindKey::Draw(IRenderer& renderer) const
{
    const std::string& actionLabel = m_controlsMap.GetActionLabels().at(m_controlAction);
    const egaColor color = GetMenuItemColor(m_selected, m_enabled);
    m_renderableText.LeftAligned(actionLabel, color, m_originX, m_originY);
    if (m_waitingForKey && m_selected)
    {
        m_renderableText.LeftAligned("< Press key to bind >", EgaBrightCyan, m_originX + m_offsetXValue, m_originY);
    }
    else
    {
        // The name of the keys is shown with the default font, as the original font from the game lacks some required characters.
        m_renderableText.LeftAligned(m_controlsMap.GetKeyStringFromAction(m_controlAction), color, m_originX + m_offsetXValue, m_originY);
    }
}

const std::string& GuiElementBindKey::GetLabel() const
{
    return m_controlsMap.GetActionLabels().at(m_controlAction);
}