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

#include "ManaBar.h"

const uint32_t maxMana = 2500u;
const uint32_t manaRequiredForOneShot = 500u;

ManaBar::ManaBar(const ManaBarConfig config) :
    m_config(config),
    m_enabled(false),
    m_mana(maxMana),
    m_previousTimestamp(0)
{

}

ManaBar::~ManaBar()
{

}

void ManaBar::Reset(const bool enabled)
{
    m_enabled = enabled;
    m_mana = maxMana;
    m_previousTimestamp = 0;
}

bool ManaBar::IsEnabled()
{
    return m_enabled;
}

bool ManaBar::FireShot()
{
    if (!m_enabled)
    {
        // Always allowed to fire
        return true;
    }

    if (m_mana < manaRequiredForOneShot)
    {
        // Mana depleted
        return false;
    }

    m_mana -= manaRequiredForOneShot;
    return true;
}

void ManaBar::Update(const uint32_t timestamp)
{
    if (!m_enabled)
    {
        // Don't care
        return;
    }

    if (timestamp > m_previousTimestamp)
    {
        const uint32_t acquiredMana = timestamp - m_previousTimestamp;
        m_mana += acquiredMana;
        if (m_mana > maxMana)
        {
            m_mana = maxMana;
        }

        m_previousTimestamp = timestamp;
    }
}

void ManaBar::Draw(IRenderer& renderer)
{
    if (m_enabled)
    {
        const int16_t barLengthInPixels = 80;
        const int16_t manaLengthInPixels = (int16_t)(barLengthInPixels * ((float)m_mana / (float)maxMana));
        const int16_t xOffset = m_config.centerX - (barLengthInPixels / 2);
        const uint32_t manaForGreenBar = manaRequiredForOneShot * 2;
        const uint32_t manaForYellowBar = manaRequiredForOneShot;
        const egaColor barColor =
            (m_mana >= manaForGreenBar) ? EgaGreen :
            (m_mana >= manaForYellowBar) ? EgaBrightYellow :
            EgaRed;

        // Top of frame
        renderer.Render2DBar(xOffset - 1, m_config.offsetY - 2, barLengthInPixels + 2, 1, m_config.frameColor);
        renderer.Render2DBar(xOffset - 1, m_config.offsetY - 1, barLengthInPixels + 1, 1, m_config.frameShadowColor);
        

        // Left side of frame
        renderer.Render2DBar(xOffset - 2, m_config.offsetY - 1, 1, 4, m_config.frameColor);
        renderer.Render2DBar(xOffset - 1, m_config.offsetY, 1, 2, m_config.frameShadowColor);

        // Right side of frame
        renderer.Render2DBar(xOffset + barLengthInPixels + 1, m_config.offsetY - 1, 1, 4, m_config.frameShadowColor);
        renderer.Render2DBar(xOffset + barLengthInPixels, m_config.offsetY - 1, 1, 3, m_config.frameColor);

        // Bottom of frame
        renderer.Render2DBar(xOffset - 1, m_config.offsetY + 2, barLengthInPixels + 2, 1, m_config.frameColor);
        renderer.Render2DBar(xOffset - 1, m_config.offsetY + 3, barLengthInPixels + 2, 1, m_config.frameShadowColor);

        renderer.Render2DBar(xOffset, m_config.offsetY, manaLengthInPixels, 2, barColor);
        if (manaLengthInPixels < barLengthInPixels)
        {
            renderer.Render2DBar(xOffset + manaLengthInPixels, m_config.offsetY, barLengthInPixels - manaLengthInPixels, 2, EgaDarkGray);
        }
    }
}