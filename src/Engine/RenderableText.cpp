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

        m_text.push_back({ offsetX + combinedWidth, offsetY, charIndex, color });

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
        strcpy_s(truncatedText, text.c_str());
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
        strcpy_s(truncatedText, "...");
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
            strncpy_s(dest, text.c_str() + startLine, posLastSpaceBeforeMaxWidth - startLine);
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

    _itoa_s(value, str, 10);

    const uint16_t widthOfBlank = m_font.GetCharacterWidth('0');
    const uint16_t widthOfBlanks = widthOfBlank * (maxDigits - (uint16_t)strlen(str));

    LeftAligned(str, color, offsetX + widthOfBlanks, offsetY);
}