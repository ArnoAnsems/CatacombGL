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

#include "ConsoleVariableBool.h"

static const std::string enabledStr = "Enabled";
static const std::string disabledStr = "Disabled";

ConsoleVariableBool::ConsoleVariableBool(const std::string& nameInMenu, const std::string& nameInConfigFile, const bool defaultValue) :
    ConsoleVariable(nameInMenu, nameInConfigFile),
    m_enabled(defaultValue),
    m_defaultValue(defaultValue)
{

}

ConsoleVariableBool::~ConsoleVariableBool()
{

}

const std::string ConsoleVariableBool::Serialize() const
{
    return (m_enabled) ? "Enabled" : "Disabled";
}

void ConsoleVariableBool::Deserialize(const std::string& str)
{
    m_enabled = (str.compare("true") == 0) || (str.compare("Enabled") == 0);
}

bool ConsoleVariableBool::IsEnabled() const
{
    return m_enabled;
}

void ConsoleVariableBool::SetEnabled(bool enabled)
{
    m_enabled = enabled;
}

void ConsoleVariableBool::Toggle()
{
    m_enabled = !m_enabled;
}

const std::string& ConsoleVariableBool::GetValueInMenu() const
{
    return (m_enabled) ? enabledStr : disabledStr;
}

void ConsoleVariableBool::SetToDefault()
{
    m_enabled = m_defaultValue;
}