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

#include "ConsoleVariableString.h"

ConsoleVariableString::ConsoleVariableString(const std::string& nameInMenu, const std::string& nameInConfigFile, const std::string& defaultValue) :
    ConsoleVariable(nameInMenu, nameInConfigFile),
    m_value(defaultValue)
{

}

ConsoleVariableString::~ConsoleVariableString()
{

}

const std::string ConsoleVariableString::Serialize() const
{
    return m_value;
}

void ConsoleVariableString::Deserialize(const std::string& str)
{
    m_value = str;
}

const std::string& ConsoleVariableString::Get() const
{
    return m_value;
}

void ConsoleVariableString::Set(const std::string& value)
{
    m_value = value;
}
