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

#pragma once

#include <gtest/gtest.h>
#include <SDL_keycode.h>

class Font;
class RenderableText;
class PlayerInput;

class Gui_TestBase : public ::testing::Test
{
protected:
    static const Font& GetDefaultFont();
    static const std::string RenderableTextToString(const RenderableText& renderableText);
    static void PressKey(PlayerInput& playerInput, const SDL_Keycode key);
};