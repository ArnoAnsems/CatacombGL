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
    const int numJoysticks = SDL_NumJoysticks();
    int joyStickIndex = 0;
    while (m_sdlGameController == nullptr && joyStickIndex < numJoysticks)
    {
        if (SDL_IsGameController(joyStickIndex) == SDL_TRUE)
        {
            m_sdlGameController = SDL_GameControllerOpen(joyStickIndex);
        }
        else
        {
            joyStickIndex++;
        }
    }

    if (m_sdlGameController != nullptr)
    {
        const std::string gameControllerName = SDL_GameControllerName(m_sdlGameController);
        const SDL_GameControllerType gameControllerType = SDL_GameControllerGetType(m_sdlGameController);
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
        SDL_GameControllerClose(m_sdlGameController);
    }
}

bool GameController::IsDetected() const
{
    return (m_sdlGameController != nullptr);
}

bool GameController::IsButtonPressed(const SDL_GameControllerButton gameControllerButton) const
{
    return (m_sdlGameController != nullptr) ? SDL_GameControllerGetButton(m_sdlGameController, gameControllerButton) == 1u : false;
}

int16_t GameController::GetAxis(const SDL_GameControllerAxis axis) const
{
    int16_t value = 0;
    if (m_sdlGameController != nullptr)
    {
        if (SDL_GameControllerHasAxis(m_sdlGameController, axis))
        {
            value = SDL_GameControllerGetAxis(m_sdlGameController, axis);
        }
    }

    return value;
}
