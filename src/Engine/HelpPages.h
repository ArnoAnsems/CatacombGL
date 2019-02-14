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

//
// HelpPages
//
// Class for parsing the contents of the HELP.TXT file.
//
#pragma once

#include <string>
#include <vector>

struct HelpLine
{
	std::string line;
	bool centered;
};

typedef std::vector<HelpLine> HelpPage;

class HelpPages
{
public:
	HelpPages();
	HelpPages(const std::string& helpText);

	uint8_t GetNumberOfPages() const;
	const HelpPage& GetPage(const uint8_t pageIndex) const;

private:
	std::vector<HelpPage> m_pages;
};
