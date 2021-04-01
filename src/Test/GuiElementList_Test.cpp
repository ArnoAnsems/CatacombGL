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

#include "GuiElementList_Test.h"

#include "..\Engine\GuiElementList.h"
#include "..\Engine\GuiElementButton.h"
#include "..\Engine\DefaultFont.h"
#include "RendererStub.h"

GuiElementList_Test::GuiElementList_Test()
{

}

GuiElementList_Test::~GuiElementList_Test()
{

}

TEST(GuiElementList_Test, ScrollThroughListThatFitsOnScreen)
{
    const uint8_t actionFirstButtonPressed = 1;
    const uint8_t actionSecondButtonPressed = 2;
    const uint8_t actionThirdButtonPressed = 3;

    const uint16_t soundWhenBrowsing = 13;

    PlayerInput playerInput;
    RendererStub rendererStub;
    RenderableText renderableText(GuiElementList_Test::GetDefaultFont());

    GuiElementList guiElementList(playerInput, 4, 16, nullptr, soundWhenBrowsing);

    GuiElementButton* firstButton = new GuiElementButton(playerInput, "First button", { actionFirstButtonPressed, 0 }, renderableText);
    guiElementList.AddChild(firstButton);
    GuiElementButton* secondButton = new GuiElementButton(playerInput, "Second button", { actionSecondButtonPressed, 0 }, renderableText);
    guiElementList.AddChild(secondButton);
    GuiElementButton* thirdButton = new GuiElementButton(playerInput, "Third button", { actionThirdButtonPressed, 0 }, renderableText);
    guiElementList.AddChild(thirdButton);

    // Initially no key is pressed
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionNone);

    // Check that all three buttons are rendered.
    renderableText.Reset();
    guiElementList.Draw(rendererStub);
    EXPECT_EQ(GuiElementList_Test::RenderableTextToString(renderableText), "First button\nSecond button\nThird button");

    // When ENTER is pressed, the first button is triggered.
    GuiElementList_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, actionFirstButtonPressed);

    // Scroll down to go from the first to the second button.
    playerInput.ClearAll();
    GuiElementList_Test::PressKey(playerInput, SDLK_DOWN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionPlaySound);

    // When ENTER is pressed, the second button is triggered.
    GuiElementList_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, actionSecondButtonPressed);

    // Scroll down to go from the second to the third button.
    playerInput.ClearAll();
    GuiElementList_Test::PressKey(playerInput, SDLK_DOWN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionPlaySound);

    // When ENTER is pressed, the third button is triggered.
    GuiElementList_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, actionThirdButtonPressed);

    // Check that all three buttons are rendered.
    renderableText.Reset();
    guiElementList.Draw(rendererStub);
    EXPECT_EQ(GuiElementList_Test::RenderableTextToString(renderableText), "First button\nSecond button\nThird button");

    // Scroll down to go from the third back to the first button.
    playerInput.ClearAll();
    GuiElementList_Test::PressKey(playerInput, SDLK_DOWN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionPlaySound);

    // When ENTER is pressed, the first button is triggered again.
    GuiElementList_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, actionFirstButtonPressed);

    // Scroll up to go from the first to the third button.
    playerInput.ClearAll();
    GuiElementList_Test::PressKey(playerInput, SDLK_UP);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionPlaySound);

    // When ENTER is pressed, the third button is triggered.
    GuiElementList_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, actionThirdButtonPressed);
}

TEST(GuiElementList_Test, ScrollThroughListThatDoesNotFitOnScreen)
{
    const uint8_t actionFirstButtonPressed = 1;
    const uint8_t actionSecondButtonPressed = 2;
    const uint8_t actionThirdButtonPressed = 3;
    const uint8_t actionFourthButtonPressed = 4;

    const uint16_t soundWhenBrowsing = 13;

    PlayerInput playerInput;
    std::string outputText;
    GuiEvent completeEvent{ GuiActionRestoreGame, 2 };
    RendererStub rendererStub;
    RenderableText renderableText(GuiElementList_Test::GetDefaultFont());

    GuiElementList guiElementList(playerInput, 3, 16, nullptr, soundWhenBrowsing);

    GuiElementButton* firstButton = new GuiElementButton(playerInput, "First button", { actionFirstButtonPressed, 0 }, renderableText);
    guiElementList.AddChild(firstButton);
    GuiElementButton* secondButton = new GuiElementButton(playerInput, "Second button", { actionSecondButtonPressed, 0 }, renderableText);
    guiElementList.AddChild(secondButton);
    GuiElementButton* thirdButton = new GuiElementButton(playerInput, "Third button", { actionThirdButtonPressed, 0 }, renderableText);
    guiElementList.AddChild(thirdButton);
    GuiElementButton* fourthButton = new GuiElementButton(playerInput, "Fourth button", { actionFourthButtonPressed, 0 }, renderableText);
    guiElementList.AddChild(fourthButton);

    // Initially no key is pressed
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionNone);

    // Check that the first three buttons are rendered.
    guiElementList.Draw(rendererStub);
    EXPECT_EQ(GuiElementList_Test::RenderableTextToString(renderableText), "First button\nSecond button\nThird button");

    // When ENTER is pressed, the first button is triggered.
    GuiElementList_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, actionFirstButtonPressed);

    // Scroll up to go from the first to the last button.
    GuiElementList_Test::PressKey(playerInput, SDLK_UP);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionPlaySound);

    // Check that the last three buttons are rendered.
    renderableText.Reset();
    guiElementList.Draw(rendererStub);
    EXPECT_EQ(GuiElementList_Test::RenderableTextToString(renderableText), "Second button\nThird button\nFourth button");

    // When ENTER is pressed, the last button is triggered.
    GuiElementList_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, actionFourthButtonPressed);

    // Scroll down to go from the last button back to the first button.
    GuiElementList_Test::PressKey(playerInput, SDLK_DOWN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionPlaySound);

    // Check that the first three buttons are rendered again.
    renderableText.Reset();
    guiElementList.Draw(rendererStub);
    EXPECT_EQ(GuiElementList_Test::RenderableTextToString(renderableText), "First button\nSecond button\nThird button");

    // Scroll down to go from the first to the second button.
    playerInput.ClearAll();
    GuiElementList_Test::PressKey(playerInput, SDLK_DOWN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionPlaySound);

    // When ENTER is pressed, the second button is triggered.
    GuiElementList_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, actionSecondButtonPressed);

    // Scroll down to go from the second to the third button.
    playerInput.ClearAll();
    GuiElementList_Test::PressKey(playerInput, SDLK_DOWN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionPlaySound);

    // When ENTER is pressed, the third button is triggered.
    GuiElementList_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, actionThirdButtonPressed);

    // Check that still only the first three buttons are rendered.
    renderableText.Reset();
    guiElementList.Draw(rendererStub);
    EXPECT_EQ(GuiElementList_Test::RenderableTextToString(renderableText), "First button\nSecond button\nThird button");

    // Scroll down to go from the third to the fourth button.
    playerInput.ClearAll();
    GuiElementList_Test::PressKey(playerInput, SDLK_DOWN);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionPlaySound);

    // Check that the last three buttons are rendered.
    renderableText.Reset();
    guiElementList.Draw(rendererStub);
    EXPECT_EQ(GuiElementList_Test::RenderableTextToString(renderableText), "Second button\nThird button\nFourth button");

    // Scroll up to go from the fourth to the third button.
    playerInput.ClearAll();
    GuiElementList_Test::PressKey(playerInput, SDLK_UP);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionPlaySound);

    // Scroll up to go from the third to the second button.
    playerInput.ClearAll();
    GuiElementList_Test::PressKey(playerInput, SDLK_UP);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionPlaySound);

    // Check that still the last three buttons are rendered.
    renderableText.Reset();
    guiElementList.Draw(rendererStub);
    EXPECT_EQ(GuiElementList_Test::RenderableTextToString(renderableText), "Second button\nThird button\nFourth button");

    // Scroll up to go from the second to the first button.
    playerInput.ClearAll();
    GuiElementList_Test::PressKey(playerInput, SDLK_UP);
    EXPECT_EQ(guiElementList.ProcessInput().guiAction, GuiActionPlaySound);

    // Check that the first three buttons are rendered again.
    renderableText.Reset();
    guiElementList.Draw(rendererStub);
    EXPECT_EQ(GuiElementList_Test::RenderableTextToString(renderableText), "First button\nSecond button\nThird button");
}

const Font& GuiElementList_Test::GetDefaultFont()
{
    RendererStub rendererStub;
    const Font* font = DefaultFont::Get(rendererStub, 10);
    return *font;
}

const std::string GuiElementList_Test::RenderableTextToString(const RenderableText& renderableText)
{
    std::string outputStr = "";
    const std::vector<RenderableText::renderableCharacter>& text = renderableText.GetText();
    int16_t previousOffsetY = -1;
    for (size_t i = 0; i < text.size(); i++)
    {
        // Detect new lines by checking the offsetY
        if (previousOffsetY != -1 && previousOffsetY != text.at(i).offsetY)
        {
            outputStr += "\n";
        }
        previousOffsetY = text.at(i).offsetY;

        outputStr += (char)(text.at(i).imageIndex);
    }

    return outputStr;
}

void GuiElementList_Test::PressKey(PlayerInput& playerInput, const SDL_Keycode key)
{
    playerInput.ClearAll();
    playerInput.SetKeyPressed(key, true);
}