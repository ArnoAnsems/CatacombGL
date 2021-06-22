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

#include "../Engine/GuiElementButton.h"

class GuiElementSaveSlotStaticCat3D : public GuiElementButton
{
public:
    GuiElementSaveSlotStaticCat3D(
        const PlayerInput& playerInput,
        const std::string& buttonLabel,
        const GuiEvent& guiEventWhenActivated,
        RenderableText& renderableText,
        bool& flashIcon);

    virtual void Draw(IRenderer& renderer) const override;

private:
    bool& m_flashIcon;
};
