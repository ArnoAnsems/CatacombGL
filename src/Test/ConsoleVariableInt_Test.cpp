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

#include "ConsoleVariableInt_Test.h"
#include "../Engine/ConsoleVariableInt.h"

ConsoleVariableInt_Test::ConsoleVariableInt_Test()
{

}

ConsoleVariableInt_Test::~ConsoleVariableInt_Test()
{

}

TEST(ConsoleVariableInt_Test, CheckNaming)
{
    ConsoleVariableInt var("MenuName", "ConfigFileName", -2, 6, 5);
    EXPECT_EQ("MenuName", var.GetNameInMenu());
    EXPECT_EQ("ConfigFileName", var.GetNameInConfigFile());
}

TEST(ConsoleVariableInt_Test, CheckAssignment)
{
    ConsoleVariableInt var("MenuName", "ConfigFileName", -2, 6, 5);
    EXPECT_EQ(5, var.GetValue());
    var.Increase();
    EXPECT_EQ(6, var.GetValue());
    var.Increase();
    EXPECT_EQ(6, var.GetValue());
    var.SetValue(-2);
    EXPECT_EQ(-2, var.GetValue());
    var.Decrease();
    EXPECT_EQ(-2, var.GetValue());
    var.SetValue(-3);
    EXPECT_EQ(-2, var.GetValue());
    var.SetValue(7);
    EXPECT_EQ(6, var.GetValue());
    var.SetToDefault();
    EXPECT_EQ(5, var.GetValue());
}

TEST(ConsoleVariableInt_Test, CheckSerialization)
{
    ConsoleVariableInt var("MenuName", "ConfigFileName", -30, 200, 5);
    EXPECT_EQ("5", var.Serialize());
    var.SetValue(-27);
    EXPECT_EQ("-27", var.Serialize());
    var.SetValue(189);
    EXPECT_EQ("189", var.Serialize());
}

TEST(ConsoleVariableInt_Test, CheckDeserialization)
{
    ConsoleVariableInt var("MenuName", "ConfigFileName", -2, 50, 5);
    var.Deserialize("-1");
    EXPECT_EQ(-1, var.GetValue());
    var.Deserialize("-3");   // Below minimum
    EXPECT_EQ(-2, var.GetValue());
    var.Deserialize("44");
    EXPECT_EQ(44, var.GetValue());
    var.Deserialize("51");   // Above maximum
    EXPECT_EQ(50, var.GetValue());
    var.Deserialize("abc");  // Invalid argument
    EXPECT_EQ(0, var.GetValue());
    var.Deserialize("10000000000000");  // Out of range
    EXPECT_EQ(0, var.GetValue());
}
