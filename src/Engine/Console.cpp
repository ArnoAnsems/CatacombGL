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

Console::Console(const Logging* logging, const std::string& label) :
    m_logging(logging),
    m_active(false),
    m_label(label)
{

}
Console::~Console()
{

}

void Console::Draw(IRenderer& renderer) const
{
    if(!m_active)
    {
        return;
    }

    renderer.Prepare2DRendering(true);
    renderer.Render2DBar(0, 0, 640, 180, EgaDarkGray);
    const Font* defaultFont = DefaultFont::Get(renderer);
    for (uint16_t i = 0; i < m_logging->GetAllLogMessages().size(); i++)
    {
        const std::string& logMessage = m_logging->GetAllLogMessages().at(i);
        renderer.RenderTextLeftAlignedTruncated(logMessage.c_str(), defaultFont, EgaBrightWhite, 8, 8 + (10 * i), 620);
    }
    renderer.RenderTextLeftAlignedTruncated(m_label.c_str(), defaultFont, EgaLightGray, 480, 170, 170);
    renderer.Unprepare2DRendering();
}

void Console::ProcessInput(PlayerInput& playerInput)
{
    if (playerInput.IsKeyJustPressed(SDLK_BACKQUOTE))
    {
        m_active = !m_active;
    }
}