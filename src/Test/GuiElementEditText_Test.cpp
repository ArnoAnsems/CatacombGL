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
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RETURN);
    const GuiEvent& busyTypingEvent = guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that we're ready to receive key presses
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "_");

    // Check that busy event is received
    EXPECT_EQ(busyTypingEvent.guiAction, GuiActionBusy);

    // Press "c"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_c);
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that "C" is displayed
    EXPECT_EQ(outputText, "C");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "C_");

    // Press "a"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_a);
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // "Check that "CA" is displayed
    EXPECT_EQ(outputText, "CA");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "CA_");

    // Press "t"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_t);
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // "Check that "CAT" is displayed
    EXPECT_EQ(outputText, "CAT");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "CAT_");

    // Press Enter to finalize text
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RETURN);
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

TEST(GuiElementEditText_Test, ClearTextWithBackspace)
{
    PlayerInput playerInput;
    std::string outputText;
    GuiEvent completeEvent{ GuiActionRestoreGame, 2 };
    RendererStub rendererStub;
    RenderableText renderableText(GuiElementEditText_Test::GetDefaultFont());
    GuiElementEditText guiElementEditText(playerInput, outputText, "Type text ...", renderableText, completeEvent);

    // Press Enter to start typing
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RETURN);
    const GuiEvent& busyTypingEvent = guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that busy event is received
    EXPECT_EQ(busyTypingEvent.guiAction, GuiActionBusy);

    // Check that we're ready to receive key presses
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "_");

    // Press "3"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_3);
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that "3" is displayed
    EXPECT_EQ(outputText, "3");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "3_");

    // Press "D"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_d);
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that "3D" is displayed
    EXPECT_EQ(outputText, "3D");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "3D_");

    // Press "Backspace"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_BACKSPACE);
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that "3" is displayed
    EXPECT_EQ(outputText, "3");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "3_");

    // Press "Backspace" again
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_BACKSPACE);
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that we're back to an empty output text, but still ready to receive key presses
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "_");

    // Press "Backspace" again
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_BACKSPACE);
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that this has no effect, output text was already empty
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "_");
}

TEST(GuiElementEditText_Test, CancelEditWithEsc)
{
    PlayerInput playerInput;
    std::string outputText;
    GuiEvent completeEvent{ GuiActionRestoreGame, 2 };
    RendererStub rendererStub;
    RenderableText renderableText(GuiElementEditText_Test::GetDefaultFont());
    GuiElementEditText guiElementEditText(playerInput, outputText, "Type text ...", renderableText, completeEvent);

    // Press Enter to start typing
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RETURN);
    const GuiEvent& busyTypingEvent = guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that busy event is received
    EXPECT_EQ(busyTypingEvent.guiAction, GuiActionBusy);

    // Check that we're ready to receive key presses
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "_");

    // Press "Z"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_z);
    guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that "Z" is displayed
    EXPECT_EQ(outputText, "Z");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "Z_");

    // Press "Esc"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_ESCAPE);
    const GuiEvent& escEvent = guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that default action is returned
    EXPECT_EQ(escEvent.guiAction, GuiActionNone);

    // Check that we have returned to the initial state
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "Type text ...");
}

TEST(GuiElementEditText_Test, CheckOtherKeysAreIgnored)
{
    PlayerInput playerInput;
    std::string outputText;
    GuiEvent completeEvent{ GuiActionRestoreGame, 2 };
    RendererStub rendererStub;
    RenderableText renderableText(GuiElementEditText_Test::GetDefaultFont());
    GuiElementEditText guiElementEditText(playerInput, outputText, "Type text ...", renderableText, completeEvent);

    // Press UP
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_UP);
    const GuiEvent& notBusyTypingEvent = guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that default action is returned
    EXPECT_EQ(notBusyTypingEvent.guiAction, GuiActionNone);

    // Check that we are still in the initial state
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "Type text ...");

    // Press Enter to start typing
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RETURN);
    const GuiEvent& busyTypingEvent = guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that busy event is received
    EXPECT_EQ(busyTypingEvent.guiAction, GuiActionBusy);

    // Press RIGHT
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RIGHT);
    const GuiEvent& stillBusyTypingEvent = guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub, 0, 0, true);

    // Check that still the busy event is received
    EXPECT_EQ(stillBusyTypingEvent.guiAction, GuiActionBusy);

    // Check that we're still ready to receive key presses
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "_");
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

void GuiElementEditText_Test::PressKey(PlayerInput& playerInput, const SDL_Keycode key)
{
    playerInput.ClearAll();
    playerInput.SetKeyPressed(key, true);
}