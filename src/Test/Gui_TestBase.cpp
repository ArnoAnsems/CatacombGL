// Copyright (C) 2025 Arno Ansems
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

#include "Gui_TestBase.h"
#include "../Engine/Font.h"
#include "../Engine/RenderableText.h"
#include "../Engine/PlayerInput.h"
#include "../Engine/DefaultFont.h"
#include "RendererStub.h"

const Font& Gui_TestBase::GetDefaultFont()
{
    RendererStub rendererStub;
    const Font* font = DefaultFont::Get(rendererStub, 10);
    return *font;
}

const std::string Gui_TestBase::RenderableTextToString(const RenderableText& renderableText)
{
    std::string outputStr = "";
    const std::vector<RenderableText::renderableCharacter>& text = renderableText.GetText();
    for (size_t i = 0; i < text.size(); i++)
    {
        outputStr += (char)(text.at(i).imageIndex);
    }

    return outputStr;
}

void Gui_TestBase::PressKey(PlayerInput& playerInput, const SDL_Keycode key)
{
    playerInput.ClearAll();
    playerInput.SetKeyPressed(key, true);
}