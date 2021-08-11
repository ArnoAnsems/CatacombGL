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

#include "RenderableLevelStatistics.h"

RenderableLevelStatistics::RenderableLevelStatistics(const LevelStatistics& levelStatistics) :
    m_levelStatistics(levelStatistics)
{

}

RenderableLevelStatistics::~RenderableLevelStatistics()
{
}

void RenderableLevelStatistics::Draw(IRenderer& renderer, const Font& font, const uint16_t additionalMargin, const uint32_t timeStamp)
{
    const int16_t offsetX = 6 - additionalMargin;

    RenderableText renderableText(font);
    const std::string monsterStr = std::to_string(m_levelStatistics.GetMonstersKilled()) + "/" + std::to_string(m_levelStatistics.GetTotalMonsters());
    renderableText.LeftAligned("MONSTERS: ", EgaRed, offsetX, 12);
    renderableText.LeftAligned(monsterStr, EgaBrightWhite, offsetX + renderableText.GetWidthInPixels("MONSTERS: "), 12);
    const std::string secretStr = std::to_string(m_levelStatistics.GetSecretsRevealed()) + "/" + std::to_string(m_levelStatistics.GetTotalSecrets());
    renderableText.LeftAligned("SECRETS: ", EgaRed, offsetX, 21);
    renderableText.LeftAligned(secretStr, EgaBrightWhite, offsetX + renderableText.GetWidthInPixels("SECRETS: "), 21);
    const std::string itemStr = std::to_string(m_levelStatistics.GetItemsTaken()) + "/" + std::to_string(m_levelStatistics.GetTotalItems());
    renderableText.LeftAligned("ITEMS: ", EgaRed, offsetX, 30);
    renderableText.LeftAligned(itemStr, EgaBrightWhite, offsetX + renderableText.GetWidthInPixels("ITEMS: "), 30);
    renderer.RenderText(renderableText);
}