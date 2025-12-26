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

#include "StringUtility.h"

std::string StringUtility::IntToHexadecimal(unsigned int number)
{
    // This function is based on an example from https://www.tutorialspoint.com/convert-an-integer-to-a-hex-string-in-cplusplus#4
    std::string result;
    if (number == 0)
    {
        result = "0";
    }
    else
    {
        const std::string hexChars = "0123456789ABCDEF";
        while (number > 0)
        {
            result = hexChars[number % 16] + result;
            number /= 16;
        }
    }
    return "0x" + result;
}