// Copyright (C) 2026 Arno Ansems
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

// This Catacomb 3-D specific element list overrides the base class GuiElementList
// to give the scrollbar a look and feel that matches with Catacomb 3-D.
#pragma once

#include "../Engine/GuiElementList.h"

class GuiElementListCat3D : public GuiElementList
{
public:
    explicit GuiElementListCat3D(
        const PlayerInput& playerInput,
        const uint16_t maxElementsDrawn,
        const uint16_t elementHeight,
        const Picture* cursorPicture,
        const uint16_t soundWhenBrowsing);

    void DrawScrollbar(IRenderer& renderer) const override;
};
