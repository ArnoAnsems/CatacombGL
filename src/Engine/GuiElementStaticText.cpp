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

#include "GuiElementStaticText.h"

GuiElementStaticText::GuiElementStaticText(
    const PlayerInput& playerInput,
    const std::string& text,
    const egaColor color,
    RenderableText& renderableText) :
    GuiElementBase(playerInput),
    m_text(text),
    m_color(color),
    m_renderableText(renderableText)
{

}

GuiElementStaticText::~GuiElementStaticText()
{

}

const GuiEvent& GuiElementStaticText::ProcessInput()
{
    // Just static text; no interaction possible.
    return GetEvent();
}

void GuiElementStaticText::Draw(IRenderer& renderer, const int16_t originX, const int16_t originY, const bool selected) const
{
    m_renderableText.Centered(m_text, m_color, originX, originY);
}