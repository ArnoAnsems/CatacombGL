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

#include <gtest/gtest.h>
#include "../Engine/ConsoleVariableEnum.h"

class ConsoleVariableEnum_Test : public ::testing::Test
{
protected:
    static constexpr uint8_t m_itemIdFirst = 0u;
    static constexpr uint8_t m_itemIdSecond = 1u;
    static constexpr uint8_t m_itemIdThird = 2u;

    const std::vector< ConsoleVariableEnum::EnumItem> m_testItems =
    {
        {"FirstItemNameInMenu", "FirstItemNameInConfig", "FirstItemNameInConfigAlt"},
        {"SecondItemNameInMenu", "SecondItemNameInConfig", "SecondItemNameInConfigAlt"},
        {"ThirdItemNameInMenu", "ThirdItemNameInConfig", "ThirdItemNameInConfigAlt"},
    };
};

TEST_F(ConsoleVariableEnum_Test, CheckNaming)
{
    const ConsoleVariableEnum var("MenuName", "ConfigFileName", m_testItems, m_itemIdThird);
    EXPECT_EQ("MenuName", var.GetNameInMenu());
    EXPECT_EQ("ConfigFileName", var.GetNameInConfigFile());
    EXPECT_EQ(m_testItems.at(m_itemIdThird).itemNameInMenu, var.GetValueInMenu());
}

TEST_F(ConsoleVariableEnum_Test, CheckAssignment)
{
    ConsoleVariableEnum var("MenuName", "ConfigFileName", m_testItems, m_itemIdThird);
    var.Next();
    EXPECT_EQ(m_itemIdFirst, var.GetItemIndex());
    EXPECT_EQ(m_testItems.at(m_itemIdFirst).itemNameInMenu, var.GetValueInMenu());
    var.Next();
    EXPECT_EQ(m_itemIdSecond, var.GetItemIndex());
    EXPECT_EQ(m_testItems.at(m_itemIdSecond).itemNameInMenu, var.GetValueInMenu());
    var.SetItemIndex(m_itemIdThird);
    EXPECT_EQ(m_itemIdThird, var.GetItemIndex());
    EXPECT_EQ(m_testItems.at(m_itemIdThird).itemNameInMenu, var.GetValueInMenu());
    var.SetItemIndex(m_itemIdThird + 1);
    EXPECT_EQ(m_itemIdThird, var.GetItemIndex()); // Unchanged, due to provided index out-of-range.
    var.SetItemIndex(m_itemIdFirst);
    EXPECT_EQ(m_itemIdFirst, var.GetItemIndex());
    EXPECT_EQ(m_testItems.at(m_itemIdFirst).itemNameInMenu, var.GetValueInMenu());
    var.SetToDefault();
    EXPECT_EQ(m_itemIdThird, var.GetItemIndex());
    EXPECT_EQ(m_testItems.at(m_itemIdThird).itemNameInMenu, var.GetValueInMenu());
}

TEST_F(ConsoleVariableEnum_Test, CheckSerialization)
{
    ConsoleVariableEnum var("MenuName", "ConfigFileName", m_testItems, m_itemIdThird);
    EXPECT_EQ(m_testItems.at(m_itemIdThird).itemNameInConfig, var.Serialize());
    var.SetItemIndex(m_itemIdFirst);
    EXPECT_EQ(m_testItems.at(m_itemIdFirst).itemNameInConfig, var.Serialize());
}

TEST_F(ConsoleVariableEnum_Test, CheckDeserialization)
{
    ConsoleVariableEnum var("MenuName", "ConfigFileName", m_testItems, m_itemIdThird);
    var.Deserialize(m_testItems.at(m_itemIdFirst).itemNameInConfig);
    EXPECT_EQ(m_itemIdFirst, var.GetItemIndex());
    var.Deserialize(m_testItems.at(m_itemIdThird).itemNameInConfigAlt);
    EXPECT_EQ(m_itemIdThird, var.GetItemIndex());
    var.Deserialize("Garbage");
    EXPECT_EQ(m_itemIdThird, var.GetItemIndex()); // Unchanged, due to unrecognized input
}
