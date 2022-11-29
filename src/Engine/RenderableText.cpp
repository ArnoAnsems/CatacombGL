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
#include "RenderableText.h"
#include <cstring>
#include <cstdlib>
#include <numeric>

RenderableText::RenderableText(const Font& font) :
    m_font(font)
{

}

const std::vector<RenderableText::renderableCharacter>& RenderableText::GetText() const
{
    return m_text;
}

const Font& RenderableText::GetFont() const
{
    return m_font;
}

void RenderableText::Reset()
{
    m_text.clear();
}

void RenderableText::LeftAligned(
    const std::string& text,
    const egaColor color,
    const int16_t offsetX,
    const int16_t offsetY)
{
    if (text.length() == 0)
    {
        // Nothing to compose
        return;
    }

    uint16_t combinedWidth = 0;

    for (uint16_t chari = 0; chari < text.length(); chari++)
    {
        const uint8_t charIndex = text[chari];
        const uint16_t charWidth = m_font.GetCharacterWidth(charIndex);

        m_text.push_back({ (int16_t)(offsetX + combinedWidth), offsetY, charIndex, color });

        combinedWidth += charWidth;
    }
}

void RenderableText::LeftAlignedTruncated(
    const std::string& text,
    const egaColor color,
    const int16_t offsetX,
    const int16_t offsetY,
    const uint16_t maxLength)
{
    if (text.length() == 0)
    {
        // Nothing to compose
        return;
    }

    const uint8_t charIndexDot = '.';
    const uint16_t dotsLength = m_font.GetCharacterWidth(charIndexDot) * 3;

    uint16_t combinedWidth = 0;
    for (uint16_t chari = 0; chari < text.length(); chari++)
    {
        const uint8_t charIndex = text[chari];
        uint16_t charWidth = m_font.GetCharacterWidth(charIndex);
        combinedWidth += charWidth;
    }

    char truncatedText[300];
    if (combinedWidth <= maxLength)
    {
        std::strcpy(truncatedText, text.c_str());
    }
    else
    {
        combinedWidth = 0;
        uint16_t chari = (uint16_t)text.length() - 1;
        bool maxLengthReached = false;
        while (chari != 0 && !maxLengthReached)
        {
            const uint8_t charIndex = text[chari];
            uint16_t charWidth = m_font.GetCharacterWidth(charIndex);
            combinedWidth += charWidth;
            maxLengthReached = (combinedWidth > maxLength);
            chari--;
        }
        std::strcpy(truncatedText, "...");
        chari += 2;
        uint16_t i = 3;
        while (chari < text.length())
        {
            truncatedText[i] = text[chari];
            chari++;
            i++;
        }
        truncatedText[i] = 0;
    }

    LeftAligned(truncatedText, color, offsetX, offsetY);
}

uint8_t RenderableText::LeftAlignedMultiLine(
    const std::string& text,
    const egaColor color,
    const int16_t offsetX,
    const int16_t offsetY)
{
    uint8_t numberOfLines = 0;

    if (text.length() == 0)
    {
        numberOfLines = 1;
    }
    else
    {
        const uint16_t maxWidth = 600;
        uint16_t chari = 0;
        uint16_t startLine = 0;
        char dest[200];
        while (chari < text.length())
        {
            uint16_t posLastSpaceBeforeMaxWidth = 0;
            uint16_t totalWidth = 0;
            chari = startLine;
            while (totalWidth < maxWidth && chari < text.length())
            {
                const uint8_t charIndex = text[chari];
                if (charIndex == ' ')
                {
                    posLastSpaceBeforeMaxWidth = chari;
                }
                totalWidth += m_font.GetCharacterWidth(charIndex);
                chari++;
            }
            if (chari == text.length() && totalWidth < maxWidth)
            {
                posLastSpaceBeforeMaxWidth = chari;
            }

            memset(dest, 0, 200);
            std::strncpy(dest, text.c_str() + startLine, posLastSpaceBeforeMaxWidth - startLine);
            LeftAligned(dest, color, offsetX, offsetY + (9 * numberOfLines));

            startLine = posLastSpaceBeforeMaxWidth + 1;
            numberOfLines++;
        }
    }

    return numberOfLines;
}

void RenderableText::Centered(
    const std::string& text,
    const egaColor color,
    const int16_t offsetX,
    const int16_t offsetY)
{
    if (text.empty())
    {
        // Nothing to render
        return;
    }

    uint16_t totalWidth = 0;
    for (uint16_t chari = 0; chari < text.length(); chari++)
    {
        const uint8_t charIndex = text[chari];
        totalWidth += m_font.GetCharacterWidth(charIndex);
    }

    const uint16_t halfTotalWidth = totalWidth / 2;
    const uint16_t leftAlignedOffsetX = offsetX - halfTotalWidth;
    LeftAligned(text, color, leftAlignedOffsetX, offsetY);
}

void RenderableText::Number(
    const uint16_t value,
    const uint8_t maxDigits,
    const egaColor color,
    const int16_t offsetX,
    const int16_t offsetY)
{
    char str[10];

    snprintf(str, 10, "%d", value);

    const uint16_t widthOfBlank = m_font.GetCharacterWidth('0');
    const uint16_t widthOfBlanks = widthOfBlank * (maxDigits - (uint16_t)strlen(str));

    LeftAligned(str, color, offsetX + widthOfBlanks, offsetY);
}

uint16_t RenderableText::GetWidthInPixels(const std::string& text) const
{
    uint16_t width = 0;
    for (size_t i = 0; i < text.size(); i++)
    {
        width += m_font.GetCharacterWidth(text.at(i));
    }

    return width;
}

uint16_t RenderableText::SplitTextInTwo(const std::string& text, std::vector<std::string>& subStrings) const
{
    // Initial result is without split
    std::string bestSplitString1 = text;
    std::string bestSplitString2 = "";
    uint16_t bestSplitWidthInPixels = GetWidthInPixels(text);

    const char separator = ' ';
    size_t separatorPos = text.find(separator);

    // Find optimal split
    while (separatorPos != std::string::npos)
    {
        const std::string str1 = text.substr(0, separatorPos);
        const size_t strLength2 = (separatorPos + 1 < text.size()) ? text.size() - separatorPos - 1 : 0;
        const std::string str2 = text.substr(separatorPos + 1, strLength2);
        const uint16_t str1WidthInPixels = GetWidthInPixels(str1);
        const uint16_t str2WidthInPixels = GetWidthInPixels(str2);
        if (str1WidthInPixels >= str2WidthInPixels && str1WidthInPixels < bestSplitWidthInPixels)
        {
            bestSplitString1 = str1;
            bestSplitString2 = str2;
            bestSplitWidthInPixels = str1WidthInPixels;
        }
        else if (str2WidthInPixels > str1WidthInPixels&& str2WidthInPixels < bestSplitWidthInPixels)
        {
            bestSplitString1 = str1;
            bestSplitString2 = str2;
            bestSplitWidthInPixels = str2WidthInPixels;
        }
        separatorPos = text.find(separator, separatorPos + 1);
    }

    // Compose result
    subStrings.clear();
    subStrings.push_back(bestSplitString1);
    if (!bestSplitString2.empty())
    {
        subStrings.push_back(bestSplitString2);
    }

    return bestSplitWidthInPixels;
}

uint16_t RenderableText::SplitTextInThree(const std::string& text, std::vector<std::string>& subStrings) const
{
    // Initial result is without split
    std::string bestSplitString1 = text;
    std::string bestSplitString2 = "";
    std::string bestSplitString3 = "";
    uint16_t bestSplitWidthInPixels = GetWidthInPixels(text);

    const char separator = ' ';
    size_t separator1Pos = text.find(separator);

    // Find optimal split
    while (separator1Pos != std::string::npos)
    {
        const std::string str1 = text.substr(0, separator1Pos);
        const uint16_t str1WidthInPixels = GetWidthInPixels(str1);

        size_t separator2Pos = text.find(separator, separator1Pos + 1);

        if (separator2Pos == std::string::npos)
        {
            // Unable to split remaining text in two
            const std::string str2 = text.substr(separator1Pos + 1, text.size() - separator1Pos - 1);
            const uint16_t str2WidthInPixels = GetWidthInPixels(str2);

            if (str1WidthInPixels >= str2WidthInPixels && str1WidthInPixels < bestSplitWidthInPixels)
            {
                bestSplitString1 = str1;
                bestSplitString2 = str2;
                bestSplitString3 = "";
                bestSplitWidthInPixels = str1WidthInPixels;
            }
            else if (str2WidthInPixels > str1WidthInPixels&& str2WidthInPixels < bestSplitWidthInPixels)
            {
                bestSplitString1 = str1;
                bestSplitString2 = str2;
                bestSplitString3 = "";
                bestSplitWidthInPixels = str2WidthInPixels;
            }
        }
        else
        {
            while (separator2Pos != std::string::npos)
            {
                const std::string str2 = text.substr(separator1Pos + 1, separator2Pos - separator1Pos);
                const uint16_t str2WidthInPixels = GetWidthInPixels(str2);

                const size_t strLength3 = (separator2Pos + 1 < text.size()) ? text.size() - separator2Pos - 1 : 0;
                const std::string str3 = text.substr(separator2Pos + 1, strLength3);

                const uint16_t str3WidthInPixels = GetWidthInPixels(str3);
                if (str1WidthInPixels >= str2WidthInPixels && str1WidthInPixels >= str3WidthInPixels && str1WidthInPixels < bestSplitWidthInPixels)
                {
                    bestSplitString1 = str1;
                    bestSplitString2 = str2;
                    bestSplitString3 = str3;
                    bestSplitWidthInPixels = str1WidthInPixels;
                }
                else if (str2WidthInPixels >= str1WidthInPixels && str2WidthInPixels >= str3WidthInPixels && str2WidthInPixels < bestSplitWidthInPixels)
                {
                    bestSplitString1 = str1;
                    bestSplitString2 = str2;
                    bestSplitString3 = str3;
                    bestSplitWidthInPixels = str2WidthInPixels;
                }
                else if (str3WidthInPixels >= str1WidthInPixels && str3WidthInPixels >= str2WidthInPixels && str3WidthInPixels < bestSplitWidthInPixels)
                {
                    bestSplitString1 = str1;
                    bestSplitString2 = str2;
                    bestSplitString3 = str3;
                    bestSplitWidthInPixels = str3WidthInPixels;
                }
                separator2Pos = text.find(separator, separator2Pos + 1);
            }
        }
        separator1Pos = text.find(separator, separator1Pos + 1);
    }

    // Compose result
    subStrings.clear();
    subStrings.push_back(bestSplitString1);
    if (!bestSplitString2.empty())
    {
        subStrings.push_back(bestSplitString2);

        if (!bestSplitString3.empty())
        {
            subStrings.push_back(bestSplitString3);
        }
    }

    return bestSplitWidthInPixels;
}
