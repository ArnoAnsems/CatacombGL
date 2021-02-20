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

#include "GuiElementEditText_Test.h"

#include "..\Engine\GuiElementEditText.h"
#include "..\Engine\DefaultFont.h"
#include "RendererStub.h"

GuiElementEditText_Test::GuiElementEditText_Test()
{

}

GuiElementEditText_Test::~GuiElementEditText_Test()
{

}

TEST(GuiElementEditText_Test, EnterTextAndPressEnter)
{
    PlayerInput playerInput;
    std::string outputText;
    GuiEvent completeEvent{ GuiActionRestoreGame, 2 };
    RendererStub rendererStub;
    const Font* font = DefaultFont::Get(rendererStub, 10);
    RenderableText renderableText(*font);
    GuiElementEditText guiElementEditText(playerInput, outputText, "", renderableText, completeEvent);

    EXPECT_EQ(outputText, "");
    playerInput.SetKeyPressed(SDLK_RETURN, true);
    guiElementEditText.ProcessInput();
    EXPECT_EQ(outputText, "");
    playerInput.ClearAll();
    playerInput.SetKeyPressed(SDLK_c, true);
    guiElementEditText.ProcessInput();
    EXPECT_EQ(outputText, "C");
    playerInput.ClearAll();
    playerInput.SetKeyPressed(SDLK_a, true);
    guiElementEditText.ProcessInput();
    EXPECT_EQ(outputText, "CA");
    playerInput.ClearAll();
    playerInput.SetKeyPressed(SDLK_t, true);
    guiElementEditText.ProcessInput();
    EXPECT_EQ(outputText, "CAT");
    playerInput.ClearAll();
    playerInput.SetKeyPressed(SDLK_RETURN, true);
    const GuiEvent& finalEvent = guiElementEditText.ProcessInput();
    EXPECT_EQ(outputText, "CAT");
    EXPECT_EQ(finalEvent.guiAction, completeEvent.guiAction);
    EXPECT_EQ(finalEvent.guiParameter, completeEvent.guiParameter);
}