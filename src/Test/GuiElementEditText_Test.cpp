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
    GuiElementEditText guiElementEditText(playerInput, outputText, "Type text ...", 5, renderableText, completeEvent);

    // Initially no key is pressed
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionNone);
    guiElementEditText.Draw(rendererStub);

    // Check initial state
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "Type text ...");

    // Press Enter to start typing
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // Check that we're ready to receive key presses
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "_");

    // Press "c"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_c);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // Check that "C" is displayed
    EXPECT_EQ(outputText, "C");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "C_");

    // Press "a"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_a);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // "Check that "CA" is displayed
    EXPECT_EQ(outputText, "CA");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "CA_");

    // Press "t"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_t);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // "Check that "CAT" is displayed
    EXPECT_EQ(outputText, "CAT");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "CAT_");

    // Press Enter to finalize text
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RETURN);
    const GuiEvent& finalEvent = guiElementEditText.ProcessInput();
    guiElementEditText.Draw(rendererStub);

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
    GuiElementEditText guiElementEditText(playerInput, outputText, "Type text ...", 5, renderableText, completeEvent);

    // Press Enter to start typing
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // Check that we're ready to receive key presses
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "_");

    // Press "3"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_3);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // Check that "3" is displayed
    EXPECT_EQ(outputText, "3");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "3_");

    // Press "D"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_d);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // Check that "3D" is displayed
    EXPECT_EQ(outputText, "3D");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "3D_");

    // Press "Backspace"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_BACKSPACE);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // Check that "3" is displayed
    EXPECT_EQ(outputText, "3");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "3_");

    // Press "Backspace" again
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_BACKSPACE);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // Check that we're back to an empty output text, but still ready to receive key presses
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "_");

    // Press "Backspace" again
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_BACKSPACE);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

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
    GuiElementEditText guiElementEditText(playerInput, outputText, "Type text ...", 5, renderableText, completeEvent);

    // Press Enter to start typing
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // Check that we're ready to receive key presses
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "_");

    // Press "Z"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_z);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // Check that "Z" is displayed
    EXPECT_EQ(outputText, "Z");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "Z_");

    // Press "Esc"
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_ESCAPE);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionNone);
    guiElementEditText.Draw(rendererStub);

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
    GuiElementEditText guiElementEditText(playerInput, outputText, "Type text ...", 5, renderableText, completeEvent);

    // Press UP
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_UP);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionNone);
    guiElementEditText.Draw(rendererStub);

    // Check that we are still in the initial state
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "Type text ...");

    // Press Enter to start typing
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // Press RIGHT
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RIGHT);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
    guiElementEditText.Draw(rendererStub);

    // Check that we're still ready to receive key presses
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "_");
}

TEST(GuiElementEditText_Test, CheckMaxTextLength)
{
    const uint16_t maxTextLength = 5;
    PlayerInput playerInput;
    std::string outputText = "";
    GuiEvent completeEvent{ GuiActionRestoreGame, 2 };
    RendererStub rendererStub;
    RenderableText renderableText(GuiElementEditText_Test::GetDefaultFont());
    GuiElementEditText guiElementEditText(playerInput, outputText, "Type text ...", maxTextLength, renderableText, completeEvent);

    // Press Enter to start typing
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);

    // Press keys until maximum text length is reached
    for (uint16_t i = 0; i < maxTextLength; i++)
    {
        GuiElementEditText_Test::PressKey(playerInput, SDLK_a);
        EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionBusy);
        playerInput.ClearAll();
    }

    // Check that output is at the maximum text length
    renderableText.Reset();
    guiElementEditText.Draw(rendererStub);
    EXPECT_EQ(outputText, "AAAAA");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "AAAAA_");

    // Press one more key
    GuiElementEditText_Test::PressKey(playerInput, SDLK_b);
    guiElementEditText.ProcessInput();

    // Check that output did not change
    renderableText.Reset();
    guiElementEditText.Draw(rendererStub);
    EXPECT_EQ(outputText, "AAAAA");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "AAAAA_");
}

TEST(GuiElementEditText_Test, CheckDisabled)
{
    const uint16_t maxTextLength = 5;
    PlayerInput playerInput;
    std::string outputText = "";
    GuiEvent completeEvent{ GuiActionRestoreGame, 2 };
    RendererStub rendererStub;
    RenderableText renderableText(GuiElementEditText_Test::GetDefaultFont());
    GuiElementEditText guiElementEditText(playerInput, outputText, "Type text ...", maxTextLength, renderableText, completeEvent);

    // Disable
    guiElementEditText.SetEnabled(false);

    // Press Enter to start typing
    renderableText.Reset();
    GuiElementEditText_Test::PressKey(playerInput, SDLK_RETURN);

    // Check that input is ignored
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionNone);

    // Press one more key
    GuiElementEditText_Test::PressKey(playerInput, SDLK_b);

    // Input is still ignored
    EXPECT_EQ(guiElementEditText.ProcessInput().guiAction, GuiActionNone);

    // Check that output is still at default
    renderableText.Reset();
    guiElementEditText.Draw(rendererStub);
    EXPECT_EQ(outputText, "");
    EXPECT_EQ(GuiElementEditText_Test::RenderableTextToString(renderableText), "Type text ...");
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