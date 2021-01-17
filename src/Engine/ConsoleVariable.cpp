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

#include "ConsoleVariable.h"

ConsoleVariable::ConsoleVariable():
    m_nameInMenu(""),
    m_nameInConfigFile("")
{

}
ConsoleVariable::ConsoleVariable(const std::string& nameInMenu, const std::string& nameInConfigFile):
    m_nameInMenu(nameInMenu),
    m_nameInConfigFile(nameInConfigFile)
{

}

ConsoleVariable::~ConsoleVariable()
{

}

const std::string& ConsoleVariable::GetNameInMenu() const
{
    return m_nameInMenu;
}

const std::string& ConsoleVariable::GetNameInConfigFile() const
{
    return m_nameInConfigFile;
}

const std::string ConsoleVariable::Serialize() const
{
    return "";
}

void ConsoleVariable::Deserialize(const std::string& str)
{

}