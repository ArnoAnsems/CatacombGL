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

#include "ConsoleVariableEnum_Test.h"
#include "..\Engine\ConsoleVariableEnum.h"

const uint8_t ItemIdFirst = 0;
const uint8_t ItemIdSecond = 1;
const uint8_t ItemIdThird = 2;

const std::vector< ConsoleVariableEnum::EnumItem> testItems =
{
    {"FirstItemNameInMenu", "FirstItemNameInConfig", "FirstItemNameInConfigAlt"},
    {"SecondItemNameInMenu", "SecondItemNameInConfig", "SecondItemNameInConfigAlt"},
    {"ThirdItemNameInMenu", "ThirdItemNameInConfig", "ThirdItemNameInConfigAlt"},
};

ConsoleVariableEnum_Test::ConsoleVariableEnum_Test()
{

}

ConsoleVariableEnum_Test::~ConsoleVariableEnum_Test()
{

}

TEST(ConsoleVariableEnum_Test, CheckNaming)
{
    ConsoleVariableEnum var("MenuName", "ConfigFileName", testItems, ItemIdThird);
    EXPECT_EQ("MenuName", var.GetNameInMenu());
    EXPECT_EQ("ConfigFileName", var.GetNameInConfigFile());
    EXPECT_EQ(testItems.at(ItemIdThird).itemNameInMenu, var.GetValueInMenu());
}

TEST(ConsoleVariableEnum_Test, CheckAssignment)
{
    ConsoleVariableEnum var("MenuName", "ConfigFileName", testItems, ItemIdThird);
    var.Next();
    EXPECT_EQ(ItemIdFirst, var.GetItemIndex());
    EXPECT_EQ(testItems.at(ItemIdFirst).itemNameInMenu, var.GetValueInMenu());
    var.Next();
    EXPECT_EQ(ItemIdSecond, var.GetItemIndex());
    EXPECT_EQ(testItems.at(ItemIdSecond).itemNameInMenu, var.GetValueInMenu());
    var.SetItemIndex(ItemIdThird);
    EXPECT_EQ(ItemIdThird, var.GetItemIndex());
    EXPECT_EQ(testItems.at(ItemIdThird).itemNameInMenu, var.GetValueInMenu());
    var.SetItemIndex(ItemIdThird + 1);
    EXPECT_EQ(ItemIdThird, var.GetItemIndex()); // Unchanged, due to provided index out-of-range.
    var.SetItemIndex(ItemIdFirst);
    EXPECT_EQ(ItemIdFirst, var.GetItemIndex());
    EXPECT_EQ(testItems.at(ItemIdFirst).itemNameInMenu, var.GetValueInMenu());
    var.SetToDefault();
    EXPECT_EQ(ItemIdThird, var.GetItemIndex());
    EXPECT_EQ(testItems.at(ItemIdThird).itemNameInMenu, var.GetValueInMenu());
}

TEST(ConsoleVariableEnum_Test, CheckSerialization)
{
    ConsoleVariableEnum var("MenuName", "ConfigFileName", testItems, ItemIdThird);
    EXPECT_EQ(testItems.at(ItemIdThird).itemNameInConfig, var.Serialize());
    var.SetItemIndex(ItemIdFirst);
    EXPECT_EQ(testItems.at(ItemIdFirst).itemNameInConfig, var.Serialize());
}

TEST(ConsoleVariableEnum_Test, CheckDeserialization)
{
    ConsoleVariableEnum var("MenuName", "ConfigFileName", testItems, ItemIdThird);
    var.Deserialize(testItems.at(ItemIdFirst).itemNameInConfig);
    EXPECT_EQ(ItemIdFirst, var.GetItemIndex());
    var.Deserialize(testItems.at(ItemIdThird).itemNameInConfigAlt);
    EXPECT_EQ(ItemIdThird, var.GetItemIndex());
    var.Deserialize("Garbage");
    EXPECT_EQ(ItemIdThird, var.GetItemIndex()); // Unchanged, due to unrecognized input
}
