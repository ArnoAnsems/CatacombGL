// Copyright (C) 2020 Arno Ansems
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

#include "IRenderer.h"

class FadeEffect
{
public:
    FadeEffect(IRenderer& renderer);
    ~FadeEffect();

    void SetOverlay(const uint32_t timeStamp);
    void DrawOverlay(const uint32_t timeStamp);
    bool OverlayActive(const uint32_t timeStamp) const;

private:
    IRenderer& m_renderer;
    uint8_t* m_rawImage;
    Picture* m_picture;
    uint32_t m_timeStamp;
    uint32_t m_pixelsRemoved;
};


