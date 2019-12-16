// Copyright (C) 2018 Arno Ansems
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

//
// DefaultFont
//
// Hard-coded font data, which is used to display text in the game selection dialog,
// even when no game data was found at all.
//
#pragma once

#include "Font.h"
#include "IRenderer.h"

static Font* defaultFontHeight7 = nullptr;
static Font* defaultFontHeight10 = nullptr;

class DefaultFont
{
public:
    static const Font* Get(IRenderer& renderer, const uint16_t lineHeight);
};
