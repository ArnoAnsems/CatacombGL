// Copyright (C) 2018 Arno Ansems
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

#include "LevelLocationNames_Test.h"
#include "../Engine/LevelLocationNames.h"

LevelLocationNames_Test::LevelLocationNames_Test()
{

}

LevelLocationNames_Test::~LevelLocationNames_Test()
{

}

TEST(LevelLocationNames_Test, EmptyChunk)
{
    FileChunk fileChunk(0);
    LevelLocationNames levelLocationNames(&fileChunk);
    EXPECT_STREQ("", levelLocationNames.GetLocationName(0).c_str());
}

TEST(LevelLocationNames_Test, MultipleLines)
{
    const char* names = "\r\nCemetery\r\n\r\nGarden\r\n";
    const uint32_t namesSize = (uint32_t)strlen(names);
    FileChunk fileChunk(namesSize);
    memcpy(fileChunk.GetChunk(), (void*)names, namesSize);
    LevelLocationNames levelLocationNames(&fileChunk);
    EXPECT_STREQ("", levelLocationNames.GetLocationName(0).c_str());
    EXPECT_STREQ("Cemetery", levelLocationNames.GetLocationName(1).c_str());
    EXPECT_STREQ("", levelLocationNames.GetLocationName(2).c_str());
    EXPECT_STREQ("Garden", levelLocationNames.GetLocationName(3).c_str());
    EXPECT_STREQ("", levelLocationNames.GetLocationName(4).c_str());
}

