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

#include "ConsoleVariableInt.h"
#include <stdexcept>

ConsoleVariableInt::ConsoleVariableInt(
    const std::string& nameInMenu,
    const std::string& nameInConfigFile,
    const int32_t minValue,
    const int32_t maxValue,
    const int32_t defaultValue) :
    ConsoleVariable(nameInMenu, nameInConfigFile),
    m_minValue(minValue),
    m_maxValue(maxValue),
    m_value(defaultValue)
{

}

ConsoleVariableInt::~ConsoleVariableInt()
{

}

const std::string ConsoleVariableInt::Serialize() const
{
    return std::to_string(m_value);
}

void ConsoleVariableInt::Deserialize(const std::string& str)
{
    try
    {
        m_value = std::stoi(str);
    }
    catch (std::invalid_argument)
    {
        m_value = 0;
    }
    catch (std::out_of_range)
    {
        m_value = 0;
    }
    ClipValueToRange();
}

int32_t ConsoleVariableInt::GetValue() const
{
    return m_value;
}

void ConsoleVariableInt::SetValue(const int32_t value)
{
    m_value = value;
    ClipValueToRange();
}

void ConsoleVariableInt::Increase()
{
    m_value++;
    ClipValueToRange();
}

void ConsoleVariableInt::Decrease()
{
    m_value--;
    ClipValueToRange();
}

void ConsoleVariableInt::ClipValueToRange()
{
    if (m_value < m_minValue)
    {
        m_value = m_minValue;
    }

    if (m_value > m_maxValue)
    {
        m_value = m_maxValue;
    }
}