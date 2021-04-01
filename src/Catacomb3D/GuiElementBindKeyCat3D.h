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

#include "..\Engine\GuiElementBindKey.h"
#include "..\Engine\RenderableTiles.h"

class GuiElementBindKeyCat3D : public GuiElementBindKey
{
public:
    GuiElementBindKeyCat3D(
        const PlayerInput& playerInput,
        ControlsMap& controlsMap,
        ControlAction controlAction,
        const int16_t offsetXValue,
        RenderableText& renderableText,
        RenderableTiles& renderableTiles,
        bool& flashIcon);

    virtual void Draw(IRenderer& renderer) const override;

private:
    RenderableTiles& m_renderableTiles;
    bool& m_flashIcon;
};
