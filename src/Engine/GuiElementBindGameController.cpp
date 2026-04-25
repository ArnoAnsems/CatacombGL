// Copyright (C) 2026 Arno Ansems
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

#include "GuiElementBindGameController.h"
#include "RenderableText.h"
#include "PlayerInput.h"

GuiElementBindGameController::GuiElementBindGameController(
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
    m_waitingForGameController(false)
{

}

const GuiEvent& GuiElementBindGameController::ProcessInput()
{
    if (m_enabled)
    {
        if (m_waitingForGameController)
        {
            // Check which key is pressed
            const SDL_GameControllerButton button = m_playerInput.GetFirstGameControllerButtonPressed();
            if (button != SDL_CONTROLLER_BUTTON_INVALID && m_controlsMap.AssignActionToGameControllerButton(m_controlAction, button))
            {
                m_waitingForGameController = false;
            }
            else
            {
                const SDL_GameControllerAxis axis = m_playerInput.GetFirstGameControllerAxisPressed();
                if (axis != SDL_CONTROLLER_AXIS_INVALID && m_controlsMap.AssignActionToGameControllerAxis(m_controlAction, axis))
                {
                    m_waitingForGameController = false;
                }
            }
        }
        else
        {
            if (m_playerInput.IsGameControllerButtonJustPressed(SDL_CONTROLLER_BUTTON_A))
            {
                m_waitingForGameController = true;
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

void GuiElementBindGameController::Draw(IRenderer& renderer) const
{
    const std::string& actionLabel = m_controlsMap.GetActionLabels().at(m_controlAction);
    const egaColor color = GetMenuItemColor(m_selected, m_enabled);
    m_renderableText.LeftAligned(actionLabel, color, m_originX, m_originY);
    if (m_waitingForGameController && m_selected)
    {
        m_renderableText.LeftAligned("< Press button to bind >", EgaBrightCyan, m_originX + m_offsetXValue, m_originY);
    }
    else
    {
        // The name of the buttons is shown with the default font, as the original font from the game lacks some required characters.
        m_renderableText.LeftAligned(m_controlsMap.GetGameControllerStringFromAction(m_controlAction), color, m_originX + m_offsetXValue, m_originY);
    }
}

const std::string& GuiElementBindGameController::GetLabel() const
{
    return m_controlsMap.GetActionLabels().at(m_controlAction);
}