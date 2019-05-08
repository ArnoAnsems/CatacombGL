// Copyright (C) 2019 Arno Ansems
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

#include "Console.h"
#include "DefaultFont.h"
#include "..\..\ThirdParty\SDL\include\SDL_timer.h"

Console::Console(const std::string& label) :
    m_active(false),
    m_label(label),
    m_openTimestamp(0),
    m_closeTimestamp(0)
{

}
Console::~Console()
{

}

void Console::Draw(IRenderer& renderer)
{
    const uint32_t timestamp = SDL_GetTicks();
    const uint32_t animationDurationInMs = 300;
    const uint32_t maxNumberOfLinesShown = 16;
    const uint32_t numberOfLinesShown =
        m_active && (timestamp - m_openTimestamp >= animationDurationInMs) ? maxNumberOfLinesShown :
        m_active ? (uint32_t)(maxNumberOfLinesShown * ((float)(timestamp - m_openTimestamp) / (float)animationDurationInMs)) :
        (timestamp - m_closeTimestamp >= animationDurationInMs) ? 0 :
        maxNumberOfLinesShown - (uint32_t)((maxNumberOfLinesShown * ((float)(timestamp - m_closeTimestamp) / (float)animationDurationInMs)));

    if (numberOfLinesShown == 0)
    {
        return;
    }

    renderer.Prepare2DRendering(true);
    renderer.Render2DBar(0, 0, 640, (numberOfLinesShown * 10) + 20, EgaDarkGray);
    const Font* defaultFont = DefaultFont::Get(renderer);
    const uint32_t numberOfLogMessages = (uint32_t)Logging::Instance().GetAllLogMessages().size();
    const uint32_t firstMessage = (numberOfLogMessages > numberOfLinesShown) ? numberOfLogMessages - numberOfLinesShown : 0;
    const int16_t offset = (numberOfLogMessages > numberOfLinesShown) ? 0 - firstMessage : numberOfLinesShown - numberOfLogMessages;
    for (uint32_t i = firstMessage; i < numberOfLogMessages; i++)
    {
        const std::string& logMessage = Logging::Instance().GetAllLogMessages().at(i);
        renderer.RenderTextLeftAlignedTruncated(logMessage.c_str(), defaultFont, EgaBrightWhite, 8, 10 + (10 * (i + offset)), 620);
    }
    renderer.RenderTextLeftAlignedTruncated(m_label.c_str(), defaultFont, EgaLightGray, 480, (numberOfLinesShown * 10) + 10, 170);
    renderer.Unprepare2DRendering();
}

void Console::ProcessInput(PlayerInput& playerInput)
{
    if (playerInput.IsKeyJustPressed(SDLK_BACKQUOTE))
    {
        const uint32_t timestamp = SDL_GetTicks();
        if (m_active)
        {
            m_closeTimestamp = timestamp;
        }
        else
        {
            m_openTimestamp = timestamp;
        }
        m_active = !m_active;
    }
}