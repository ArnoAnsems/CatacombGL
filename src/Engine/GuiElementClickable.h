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
#pragma once

#include "GuiElementBase.h"

class GuiElementClickable: public GuiElementBase
{
public:
    explicit GuiElementClickable(const PlayerInput& playerInput, uint16_t elementWidth, uint16_t elementHeight);
    virtual ~GuiElementClickable() = default;

protected:
    bool isClicked() const;

private:
    const uint16_t m_elementWidth;
    const uint16_t m_elementHeight;
};
