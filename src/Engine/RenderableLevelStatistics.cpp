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

void RenderableLevelStatistics::Draw(IRenderer& renderer, const Font& font, const uint16_t additionalMargin, const uint32_t timeStamp, const egaColor floorColor)
{
    const int16_t offsetX = 6 - additionalMargin;
    const egaColor labelColor = (floorColor == EgaRed) ? EgaBrightRed : EgaRed;
    const egaColor countColor = (floorColor == EgaBrightWhite || floorColor == EgaBrightYellow) ? EgaDarkGray : EgaBrightWhite;
    RenderableText renderableText(font);
    const std::string monsterStr = std::to_string(m_levelStatistics.GetMonstersKilled()) + "/" + std::to_string(m_levelStatistics.GetTotalMonsters());
    renderableText.LeftAligned("MONSTERS: ", labelColor, offsetX, 12);
    renderableText.LeftAligned(monsterStr, countColor, offsetX + renderableText.GetWidthInPixels("MONSTERS: "), 12);
    const std::string secretStr = std::to_string(m_levelStatistics.GetSecretsRevealed()) + "/" + std::to_string(m_levelStatistics.GetTotalSecrets());
    renderableText.LeftAligned("SECRETS: ", labelColor, offsetX, 21);
    renderableText.LeftAligned(secretStr, countColor, offsetX + renderableText.GetWidthInPixels("SECRETS: "), 21);
    const std::string itemStr = std::to_string(m_levelStatistics.GetItemsTaken()) + "/" + std::to_string(m_levelStatistics.GetTotalItems());
    renderableText.LeftAligned("ITEMS: ", labelColor, offsetX, 30);
    renderableText.LeftAligned(itemStr, countColor, offsetX + renderableText.GetWidthInPixels("ITEMS: "), 30);
    const std::string timeStr = ConvertTimeStampToString(timeStamp);
    renderableText.LeftAligned(timeStr, countColor, offsetX, 48);
    renderer.RenderText(renderableText);
}

const std::string RenderableLevelStatistics::ConvertTimeStampToString(const uint32_t timeStamp)
{
    const uint32_t milliSecondsInSecond = 1000u;
    const uint32_t secondsInMinute = 60u;
    const uint32_t minutesInHour = 60u;
    const uint32_t milliSecondsInMinute = milliSecondsInSecond * secondsInMinute;
    const uint32_t milliSecondsInHour = milliSecondsInMinute * minutesInHour;
    const uint32_t hours = timeStamp / milliSecondsInHour;
    const uint32_t minutes = (timeStamp % milliSecondsInHour) / milliSecondsInMinute;
    const uint32_t seconds = (timeStamp % milliSecondsInMinute) / milliSecondsInSecond;
    return
        NumberToTwoDigitString(hours) + ":" +
        NumberToTwoDigitString(minutes) + ":" +
        NumberToTwoDigitString(seconds);
}

const std::string RenderableLevelStatistics::NumberToTwoDigitString(const uint32_t number)
{
    const uint32_t truncatedNumber = number % 100;
    const std::string str = std::to_string(truncatedNumber);
    return (truncatedNumber < 10) ? "0" + str : str;
}