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
    RenderableText renderableText(GuiElementEditText_Test::GetDefaultFont());
    GuiElementEditText guiElementEditText(playerInput, outputText, "Type text ...", renderableText, completeEvent);

    // Initially no key is pressed
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check initial state
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "Type text ...");

    // Press Enter to start typing
    renderableText.Reset();
    playerInput.ClearAll();
    playerInput.SetKeyPressed(SDLK_RETURN, true);
    const GuiEvent& firstEvent = guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that we're ready to receive key presses
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "_");

    // Check that default event is received
    EXPECT_EQ(firstEvent.guiAction, GuiActionNone);
    EXPECT_EQ(firstEvent.guiParameter, 0);

    // Press "c"
    renderableText.Reset();
    playerInput.ClearAll();
    playerInput.SetKeyPressed(SDLK_c, true);
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that "C" is displayed
    EXPECT_EQ(outputText, "C");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "C_");

    // Press "a"
    renderableText.Reset();
    playerInput.ClearAll();
    playerInput.SetKeyPressed(SDLK_a, true);
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // "Check that "CA" is displayed
    EXPECT_EQ(outputText, "CA");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "CA_");

    // Press "t"
    renderableText.Reset();
    playerInput.ClearAll();
    playerInput.SetKeyPressed(SDLK_t, true);
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // "Check that "CAT" is displayed
    EXPECT_EQ(outputText, "CAT");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "CAT_");

    // Press Enter to finalize text
    renderableText.Reset();
    playerInput.ClearAll();
    playerInput.SetKeyPressed(SDLK_RETURN, true);
    const GuiEvent& finalEvent = guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // "Check that "CAT" is still in the outputText
    EXPECT_EQ(outputText, "CAT");

    // Check that the initial text is displayed again
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "Type text ...");

    // Check that completed event is received
    EXPECT_EQ(finalEvent.guiAction, completeEvent.guiAction);
    EXPECT_EQ(finalEvent.guiParameter, completeEvent.guiParameter);
}

const Font& GuiElementEditText_Test::GetDefaultFont()
{
    RendererStub rendererStub;
    const Font* font = DefaultFont::Get(rendererStub, 10);
    return *font;
}

const std::string GuiElementEditText_Test::RenderableTextToString(const RenderableText& renderableText)
{
    std::string outputStr = "";
    const std::vector<RenderableText::renderableCharacter>& text = renderableText.GetText();
    for (size_t i = 0; i < text.size(); i++)
    {
        outputStr += (char)(text.at(i).imageIndex);
    }

    return outputStr;
}