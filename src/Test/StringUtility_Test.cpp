// Copyright (C) 2025 Arno Ansems
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

#include <gtest/gtest.h>
#include "../Engine/StringUtility.h"

class StringUtility_Test : public ::testing::Test
{
};

TEST(StringUtility_Test, IntToHexadecimal)
{
	EXPECT_EQ("0x0", StringUtility::IntToHexadecimal(0u));
	EXPECT_EQ("0xA", StringUtility::IntToHexadecimal(10u));
	EXPECT_EQ("0xFFFF", StringUtility::IntToHexadecimal(65535u));
	EXPECT_EQ("0x10000", StringUtility::IntToHexadecimal(65536u));
}
