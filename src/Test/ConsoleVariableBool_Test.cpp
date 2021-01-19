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

#include "ConsoleVariableBool_Test.h"
#include "..\Engine\ConsoleVariableBool.h"

ConsoleVariableBool_Test::ConsoleVariableBool_Test()
{

}

ConsoleVariableBool_Test::~ConsoleVariableBool_Test()
{

}

TEST(ConsoleVariableBool_Test, CheckNaming)
{
    ConsoleVariableBool var("MenuName", "ConfigFileName", true);
    EXPECT_EQ("MenuName", var.GetNameInMenu());
    EXPECT_EQ("ConfigFileName", var.GetNameInConfigFile());
}

TEST(ConsoleVariableBool_Test, CheckAssignment)
{
    ConsoleVariableBool var("MenuName", "ConfigFileName", true);
    EXPECT_TRUE(var.IsEnabled());
    var.Toggle();
    EXPECT_FALSE(var.IsEnabled());
    var.Toggle();
    EXPECT_TRUE(var.IsEnabled());
    var.SetEnabled(false);
    EXPECT_FALSE(var.IsEnabled());
}

TEST(ConsoleVariableBool_Test, CheckSerialization)
{
    ConsoleVariableBool var("MenuName", "ConfigFileName", true);
    EXPECT_EQ("Enabled", var.Serialize());
    var.Toggle();
    EXPECT_EQ("Disabled", var.Serialize());
    var.Deserialize("Disabled");
    EXPECT_FALSE(var.IsEnabled());
}

TEST(ConsoleVariableBool_Test, CheckDeserialization)
{
    ConsoleVariableBool var("MenuName", "ConfigFileName", true);
    var.Deserialize("Disabled");
    EXPECT_FALSE(var.IsEnabled());
    var.Deserialize("false");
    EXPECT_FALSE(var.IsEnabled());
    var.Deserialize("Enabled");
    EXPECT_TRUE(var.IsEnabled());
    var.Deserialize("true");
    EXPECT_TRUE(var.IsEnabled());
}