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

#pragma once

#include <gtest/gtest.h>
#include "../Engine/Font.h"
#include "../Engine/RenderableText.h"
#include "../Engine/PlayerInput.h"

class GuiElementList_Test : public ::testing::Test
{
public:
    GuiElementList_Test();
    virtual ~GuiElementList_Test();
    static const Font& GetDefaultFont();
    static const std::string RenderableTextToString(const RenderableText& renderableText);
    static void PressKey(PlayerInput& playerInput, const SDL_Keycode key);
};



