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
#include "ConsoleVariableEnum.h"
#include "RenderableText.h"

class GuiElementEnumSelection : public GuiElementBase
{
public:
    GuiElementEnumSelection(
        const PlayerInput& playerInput,
        ConsoleVariableEnum& cvarEnum,
        const int16_t offsetXValue,
        RenderableText& renderableText);
    ~GuiElementEnumSelection() override;

    virtual const GuiEvent& ProcessInput() override;
    virtual void Draw(IRenderer& renderer) const override;
    virtual const std::string& GetLabel() const override;

protected:
    ConsoleVariableEnum& m_cvarEnum;
    const int16_t m_offsetXValue;
    RenderableText& m_renderableText;
};

