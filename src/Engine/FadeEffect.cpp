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

#include "FadeEffect.h"

FadeEffect::FadeEffect(IRenderer& renderer) :
    m_renderer(renderer),
    m_rawImage(nullptr),
    m_picture(nullptr),
    m_timeStamp(0),
    m_pixelsRemoved(0)
{

}

FadeEffect::~FadeEffect()
{
    delete m_rawImage;
    delete m_picture;
}

void FadeEffect::SetOverlay(const uint32_t timeStamp)
{
    const unsigned int textureId = (m_picture != nullptr) ? m_picture->GetTextureId() : 0;
    delete m_picture;
    m_picture = m_renderer.GetScreenCapture(textureId);
    m_timeStamp = timeStamp;
    m_pixelsRemoved = 0;
}

void FadeEffect::DrawOverlay(const uint32_t timeStamp)
{
    const uint32_t timePassed = (timeStamp - m_timeStamp);
    if (timePassed <= 1000)
    {
        uint32_t pixelsToRemove = (timePassed * 64000) / 1000;
        std::vector < std::pair<int16_t, int16_t>> coordinates;
        for (uint32_t p = m_pixelsRemoved; p < pixelsToRemove; p++)
        {
            coordinates.push_back(std::make_pair(p % 320, p / 320));
        }
        m_pixelsRemoved = pixelsToRemove;
        m_renderer.RemovePixelsFromScreenCapture(coordinates);
    }

    m_renderer.RenderScreenCapture(m_picture);
}

bool FadeEffect::OverlayActive(const uint32_t timeStamp) const
{
    return timeStamp < m_timeStamp + 1000;
}