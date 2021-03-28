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

#include "GuiElementHighScores.h"
#include "..\..\ThirdParty\SDL\include\SDL_keyboard.h"

GuiElementHighScores::GuiElementHighScores(
    const PlayerInput& playerInput,
    HighScores& highscores,
    uint32_t& timestamp) :
    GuiElementBase(playerInput),
    m_highScores(highscores),
    m_timestamp(timestamp)
{

}

const GuiEvent& GuiElementHighScores::ProcessInput()
{
    // Check if a name for a new high score is entered
    const SDL_Keycode keyCode = m_playerInput.GetFirstKeyPressed();
    if (KeyIsSuitableForName(keyCode))
    {
        m_highScores.AddCharactersToNameOfNewScore(std::string(SDL_GetKeyName(keyCode)));
    }
    else if (keyCode == SDLK_RETURN)
    {
        m_highScores.FinishNameOfNewScore();
        return m_nameFinishedEvent;
    }
    else if (keyCode == SDLK_BACKSPACE)
    {
        m_highScores.RemoveACharacterFromNameOfNewScore();
    }

    return GetEvent();
}

void GuiElementHighScores::Draw(IRenderer& renderer, const int16_t, const int16_t, const bool) const
{
    m_highScores.Draw(renderer, m_timestamp);
}

bool GuiElementHighScores::KeyIsSuitableForName(const SDL_Keycode keyCode)
{
    return ((keyCode >= SDLK_a && keyCode <= SDLK_z) ||
        (keyCode >= SDLK_0 && keyCode <= SDLK_9));
}