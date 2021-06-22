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

#include "ConsoleVariableString_Test.h"
#include "../Engine/ConsoleVariableString.h"

ConsoleVariableString_Test::ConsoleVariableString_Test()
{

}

ConsoleVariableString_Test::~ConsoleVariableString_Test()
{

}

TEST(ConsoleVariableString_Test, CheckNaming)
{
    ConsoleVariableString var("MenuName", "ConfigFileName", "DefaultValue");
    EXPECT_EQ("MenuName", var.GetNameInMenu());
    EXPECT_EQ("ConfigFileName", var.GetNameInConfigFile());
    EXPECT_EQ("DefaultValue", var.Get());
}

TEST(ConsoleVariableString_Test, CheckAssignment)
{
    ConsoleVariableString var("MenuName", "ConfigFileName", "DefaultValue");
    EXPECT_EQ("DefaultValue", var.Get());
    const std::string newValue = "New value with \\ and spaces";
    var.Set(newValue);
    EXPECT_EQ(newValue, var.Get());
    var.SetToDefault();
    EXPECT_EQ("DefaultValue", var.Get());
}

TEST(ConsoleVariableString_Test, CheckSerialization)
{
    ConsoleVariableString var("MenuName", "ConfigFileName", "DefaultValue");
    EXPECT_EQ("DefaultValue", var.Serialize());
}

TEST(ConsoleVariableString_Test, CheckDeserialization)
{
    ConsoleVariableString var("MenuName", "ConfigFileName", "DefaultValue");
    const std::string newValue = "New value with \\ and spaces";
    var.Deserialize(newValue);
    EXPECT_EQ(newValue, var.Get());
}