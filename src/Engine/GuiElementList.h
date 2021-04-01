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

#include "GuiElementBase.h"
#include "RenderableText.h"

class GuiElementList : public GuiElementBase
{
public:
    GuiElementList(
        const PlayerInput& playerInput,
        const uint16_t maxElementsDrawn,
        const uint16_t elementHeight,
        const Picture* cursorPicture,
        const uint16_t soundWhenBrowsing);
    ~GuiElementList() override;

    virtual void AddChild(GuiElementBase* child, const int16_t offsetX = 0, const int16_t offsetY = 0, const int16_t parentId = 0) override;
    virtual const GuiEvent& ProcessInput() override;
    virtual void Draw(IRenderer& renderer) const override;
    virtual void SetEnabled(const bool enabled, const int16_t id = 0) override;

private:
    std::vector<GuiElementBase*> m_elements;
    const uint16_t m_elementHeight;
    const uint16_t m_maxElementsDrawn;
    uint16_t m_elementSelected;
    uint16_t m_firstElementDrawn;
    const Picture* m_cursorPicture;
    const GuiEvent m_guiEventPlayBrowseSound;
};




