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

#include "GameController.h"
#include "../Engine/Logging.h"

GameController::GameController()
{
    int numJoysticks = 0;
    SDL_JoystickID* joysticks = SDL_GetJoysticks(&numJoysticks);
    int joyStickIndex = 0;
    while (m_sdlGameController == nullptr && joyStickIndex < numJoysticks)
    {
        if (SDL_IsGamepad(joysticks[joyStickIndex]))
        {
            m_sdlGameController = SDL_OpenGamepad(joysticks[joyStickIndex]);
        }
        else
        {
            joyStickIndex++;
        }
    }

    if (m_sdlGameController != nullptr)
    {
        const std::string gameControllerName = SDL_GetGamepadName(m_sdlGameController);
        const std::string logMessage = "Detected game controller: " + gameControllerName;
        Logging::Instance().AddLogMessage(logMessage);
    }
    else
    {
        Logging::Instance().AddLogMessage("No game controller detected");
    }
}

GameController::~GameController()
{
    if (m_sdlGameController != nullptr)
    {
        SDL_CloseGamepad(m_sdlGameController);
    }
}

bool GameController::IsDetected() const
{
    return (m_sdlGameController != nullptr);
}

bool GameController::IsButtonPressed(const SDL_GamepadButton gameControllerButton) const
{
    return (m_sdlGameController != nullptr) ? SDL_GetGamepadButton(m_sdlGameController, gameControllerButton) == 1u : false;
}

int16_t GameController::GetAxis(const SDL_GamepadAxis axis) const
{
    int16_t value = 0;
    if (m_sdlGameController != nullptr)
    {
        if (SDL_GamepadHasAxis(m_sdlGameController, axis))
        {
            value = SDL_GetGamepadAxis(m_sdlGameController, axis);
        }
    }

    return value;
}
