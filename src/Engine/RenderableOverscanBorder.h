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

//
// RenderableOverscanBorder
//
// Class for rendering the overscan border to the screen.
//

#include "OverscanBorder.h"
#include "IRenderer.h"

class RenderableOverscanBorder
{
public:
    RenderableOverscanBorder(const OverscanBorder& overscanBorder);
    ~RenderableOverscanBorder();

    void Draw(IRenderer& renderer, const uint16_t additionalMargin, const uint32_t timeStamp, const bool helpWindow);

private:
    const OverscanBorder& m_overscanBorder;
};

