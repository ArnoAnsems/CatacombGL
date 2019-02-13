// Copyright (C) 2019 Arno Ansems
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

#include "HelpPages.h"

static const char endOfPageMarker = '@';
static const char centeredLineMarker = '^';
static const char endOfLineMarker = '\n';

HelpPages::HelpPages(const std::string& helpText)
{
	//uint16_t lineIndex = 0;
	HelpLine currentLine;
	HelpPage currentPage;
	uint16_t charIndex = 0;
	while (charIndex < helpText.size())
	{
		if (helpText.at(charIndex) == endOfPageMarker)
		{
			if (!currentPage.empty() && (currentPage.size() != 1 || !currentPage.at(0).line.empty()))
			{
				m_pages.push_back(currentPage);
			}
			currentPage.clear();
		}
		else if (helpText.at(charIndex) == endOfLineMarker)
		{
			currentPage.push_back(currentLine);
			currentLine.line.clear();
			currentLine.centered = false;
		}
		else
		{
			currentLine.line += helpText.at(charIndex);
		}
		charIndex++;
	}
}

uint8_t HelpPages::GetNumberOfPages() const
{
	return static_cast<uint8_t>(m_pages.size());
}

const HelpPage& HelpPages::GetPage(const uint8_t pageIndex) const
{
	const uint8_t index = (pageIndex < m_pages.size()) ? pageIndex : 0;
	return m_pages.at(index);
}