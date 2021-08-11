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
// RenderableLevelStatistics
//
// Class for rendering the level statistics.
//

#include "LevelStatistics.h"
#include "IRenderer.h"
#include "Font.h"

class RenderableLevelStatistics
{
public:
    RenderableLevelStatistics(const LevelStatistics& levelStatistics);
    ~RenderableLevelStatistics();

    void Draw(IRenderer& renderer, const Font& font, const uint16_t additionalMargin, const uint32_t timeStamp);

private:
    const LevelStatistics& m_levelStatistics;
};