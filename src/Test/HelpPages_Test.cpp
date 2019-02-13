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

#include "HelpPages_Test.h"
#include "..\Engine\HelpPages.h"

HelpPages_Test::HelpPages_Test()
{

}

HelpPages_Test::~HelpPages_Test()
{

}

TEST(HelpPages_Test, ZeroPagesWhenEmptyString)
{
	HelpPages helpPages("");
	EXPECT_EQ(0, helpPages.GetNumberOfPages());
}

TEST(HelpPages_Test, TwoRegularPages)
{
	HelpPages helpPages("First line\n^Centered second line\n\nFourth and last line\n@First line of second page\nSecond and last line\n@\n@");
	EXPECT_EQ(2, helpPages.GetNumberOfPages());

	const HelpPage& page0 = helpPages.GetPage(0);
	EXPECT_EQ(4, page0.size());
	EXPECT_STREQ("First line", page0.at(0).line.c_str());
}
