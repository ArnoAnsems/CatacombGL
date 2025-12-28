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

#include <gtest/gtest.h>
#include "Gui_TestBase.h"
#include "../Engine/GuiMenu.h"
#include "../Engine/GuiElementButton.h"
#include "../Engine/GuiPage.h"
#include "../Engine/DefaultFont.h"
#include "RendererStub.h"

class GuiMenu_Test : public Gui_TestBase
{
};

TEST_F(GuiMenu_Test, NavigateThroughPages)
{
    const int16_t pageId1 = 1;
    const int16_t pageId2 = 2;
    const int16_t pageId3 = 3;

    PlayerInput playerInput;
    RendererStub rendererStub;
    RenderableText renderableText(GuiMenu_Test::GetDefaultFont());

    GuiMenu guiMenu(playerInput);

    GuiPage* guiPage1 = new GuiPage(playerInput);
    guiPage1->SetId(pageId1);
    GuiEvent eventGotoPage2 = { GuiActionNavigateTo, pageId2 };
    GuiElementButton* guiButton1 = new GuiElementButton(playerInput, "Button on first page", eventGotoPage2, renderableText);
    guiPage1->AddChild(guiButton1, 0, 0, pageId1);
    guiMenu.AddChild(guiPage1, 0, 0, 0);

    GuiPage* guiPage2 = new GuiPage(playerInput);
    guiPage2->SetId(pageId2);
    GuiEvent eventGotoPage3 = { GuiActionNavigateTo, pageId3 };
    GuiElementButton* guiButton2 = new GuiElementButton(playerInput, "Button on second page", eventGotoPage3, renderableText);
    guiPage2->AddChild(guiButton2, 0, 0, pageId2);
    guiMenu.AddChild(guiPage2, 0, 0, 0);

    GuiPage* guiPage3 = new GuiPage(playerInput);
    guiPage3->SetId(pageId3);
    GuiEvent eventNone = { GuiActionNone, 0 };
    GuiElementButton* guiButton3 = new GuiElementButton(playerInput, "Button on third page", eventNone, renderableText);
    guiPage3->AddChild(guiButton3, 0, 0, pageId3);
    guiMenu.AddChild(guiPage3, 0, 0, 0);

    // Initially no key is pressed
    EXPECT_EQ(guiMenu.ProcessInput().guiAction, GuiActionNone);

    // Check that the first page is rendered.
    renderableText.Reset();
    guiMenu.Draw(rendererStub);
    EXPECT_EQ(GuiMenu_Test::RenderableTextToString(renderableText), "Button on first page");

    // When ENTER is pressed, the menu navigates to the second page.
    GuiMenu_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiMenu.ProcessInput().guiAction, GuiActionNone);

    // Check that the second page is rendered.
    renderableText.Reset();
    guiMenu.Draw(rendererStub);
    EXPECT_EQ(GuiMenu_Test::RenderableTextToString(renderableText), "Button on second page");

    // When ENTER is pressed, the menu navigates to the third page.
    GuiMenu_Test::PressKey(playerInput, SDLK_RETURN);
    EXPECT_EQ(guiMenu.ProcessInput().guiAction, GuiActionNone);

    // Check that the third page is rendered.
    renderableText.Reset();
    guiMenu.Draw(rendererStub);
    EXPECT_EQ(GuiMenu_Test::RenderableTextToString(renderableText), "Button on third page");

    // When ESC is pressed, the menu navigates back to the second page.
    GuiMenu_Test::PressKey(playerInput, SDLK_ESCAPE);
    EXPECT_EQ(guiMenu.ProcessInput().guiAction, GuiActionNone);

    // Check that the second page is rendered.
    renderableText.Reset();
    guiMenu.Draw(rendererStub);
    EXPECT_EQ(GuiMenu_Test::RenderableTextToString(renderableText), "Button on second page");

    // When ESC is pressed, the menu navigates back to the first page.
    GuiMenu_Test::PressKey(playerInput, SDLK_ESCAPE);
    EXPECT_EQ(guiMenu.ProcessInput().guiAction, GuiActionNone);

    // Check that the first page is rendered.
    renderableText.Reset();
    guiMenu.Draw(rendererStub);
    EXPECT_EQ(GuiMenu_Test::RenderableTextToString(renderableText), "Button on first page");

    // When ESC is pressed while on the first page, the menu returns the close action.
    GuiMenu_Test::PressKey(playerInput, SDLK_ESCAPE);
    EXPECT_EQ(guiMenu.ProcessInput().guiAction, GuiActionClose);

    // Check that the first page is still rendered.
    renderableText.Reset();
    guiMenu.Draw(rendererStub);
    EXPECT_EQ(GuiMenu_Test::RenderableTextToString(renderableText), "Button on first page");

    // Open the second page
    guiMenu.Open(pageId2);

    // Check that the second page is rendered.
    renderableText.Reset();
    guiMenu.Draw(rendererStub);
    EXPECT_EQ(GuiMenu_Test::RenderableTextToString(renderableText), "Button on second page");
}
