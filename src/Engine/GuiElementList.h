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
        const int16_t originX,
        const int16_t originY,
        const uint16_t elementHeight,
        const Picture* cursorPicture);
    ~GuiElementList();

    virtual uint8_t ProcessInput() override;
    virtual void Draw(IRenderer& renderer, const int16_t originX, const int16_t originY, const bool selected) const override;

    void AddElement(GuiElementBase* element);

private:
    std::vector<GuiElementBase*> m_elements;
    const int16_t m_originX;
    const int16_t m_originY;
    const uint16_t m_elementHeight;
    const uint16_t m_maxElementsDrawn;
    uint16_t m_elementSelected;
    uint16_t m_firstElementDrawn;
    const Picture* m_cursorPicture;
};




