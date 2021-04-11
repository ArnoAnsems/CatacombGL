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

#include "ConsoleVariableEnum.h"

ConsoleVariableEnum::ConsoleVariableEnum(
    const std::string& nameInMenu,
    const std::string& nameInConfigFile,
    const std::vector<EnumItem>& items,
    const uint8_t defaultItemIndex) :
    ConsoleVariable(nameInMenu, nameInConfigFile),
    m_items(items),
    m_itemIndex(defaultItemIndex),
    m_defaultItemIndex(defaultItemIndex)
{

}

ConsoleVariableEnum::~ConsoleVariableEnum()
{

}

const std::string ConsoleVariableEnum::Serialize() const
{
    return m_items.at(m_itemIndex).itemNameInConfig;
}

void ConsoleVariableEnum::Deserialize(const std::string& str)
{
    uint8_t newIndex = 0;
    bool itemFound = false;
    while (!itemFound && newIndex < m_items.size())
    {
        itemFound = (m_items.at(newIndex).itemNameInConfig == str || m_items.at(newIndex).itemNameInConfigAlt == str);
        if (!itemFound)
        {
            newIndex++;
        }
    }

    if (itemFound)
    {
        m_itemIndex = newIndex;
    }
}

uint8_t ConsoleVariableEnum::GetItemIndex() const
{
    return m_itemIndex;
}

void ConsoleVariableEnum::SetItemIndex(const uint8_t index)
{
    if (index < m_items.size())
    {
        m_itemIndex = index;
    }
}

void ConsoleVariableEnum::Next()
{
    m_itemIndex++;
    if (m_itemIndex >= m_items.size())
    {
        m_itemIndex = 0;
    }
}

const std::string& ConsoleVariableEnum::GetValueInMenu() const
{
    return m_items.at(m_itemIndex).itemNameInMenu;
}

void ConsoleVariableEnum::SetToDefault()
{
    m_itemIndex = m_defaultItemIndex;
}