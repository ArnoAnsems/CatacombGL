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
    m_picture(nullptr),
    m_pixelsRemoved(0),
    m_rndval(0)
{

}

FadeEffect::~FadeEffect()
{
    delete m_picture;
}

void FadeEffect::SetOverlay()
{
    const unsigned int textureId = (m_picture != nullptr) ? m_picture->GetTextureId() : 0;
    delete m_picture;
    m_picture = m_renderer.GetScreenCapture(textureId);
    m_pixelsRemoved = 0;
    m_rndval = 0;
}

void FadeEffect::DrawOverlay(const uint32_t milliSec)
{
    const uint32_t totalDurationInMilliSec = 1000;
    if (milliSec <= totalDurationInMilliSec)
    {
        // FizzleFade effect implementation based on Wolf4SDL source code, file ID_VH.CPP.
        const uint32_t rndmask = 0x00012000;
        const uint32_t rndbits_y = 8;
        const uint32_t screenWidth = 320;
        const uint32_t screenHeight = 200;

        const uint32_t pixelsToRemove = (milliSec * screenWidth * screenHeight) / totalDurationInMilliSec;
        std::vector < std::pair<int16_t, int16_t>> coordinates;
        for (uint32_t p = m_pixelsRemoved; p < pixelsToRemove; p++)
        {
            // Seperate random value into x/y pair
            const int32_t x = m_rndval >> rndbits_y;
            const int32_t y = m_rndval & ((1 << rndbits_y) - 1);

            // Advance to next random element
            m_rndval = (m_rndval >> 1) ^ (m_rndval & 1 ? 0 : rndmask);

            if (x >= screenWidth || y >= screenHeight)
            {
                if (m_rndval == 0)     // entire sequence has been completed
                {
                    break;
                }
                p--;
                continue;
            }
            coordinates.push_back(std::make_pair(x, y));

            if (m_rndval == 0)		// entire sequence has been completed
            {
                break;
            }
        }
        m_pixelsRemoved = pixelsToRemove;
        m_renderer.RemovePixelsFromScreenCapture(coordinates);
    }

    m_renderer.RenderScreenCapture(m_picture);
}

bool FadeEffect::OverlayActive() const
{
    return (m_pixelsRemoved > 0);
}