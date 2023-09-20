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
#pragma once

#include <vector>
#include <cstdint>
#include "ConsoleVariable.h"

class ConsoleVariableEnum : public ConsoleVariable
{
public:
    typedef struct EnumItem
    {
        const std::string itemNameInMenu;
        const std::string itemNameInConfig;
        const std::string itemNameInConfigAlt;
    } EnumItem;

    ConsoleVariableEnum(
        const std::string& nameInMenu,
        const std::string& nameInConfigFile,
        const std::vector<EnumItem>& items,
        const uint8_t defaultItemIndex);
    ~ConsoleVariableEnum();

    const std::string Serialize() const override;
    void Deserialize(const std::string& str) override;
    void SetToDefault() override;

    uint8_t GetItemIndex() const;
    void SetItemIndex(const uint8_t index);
    void Next();

    const std::string& GetValueInMenu() const;

private:
    const std::vector<EnumItem> m_items;
    uint8_t m_itemIndex;
    const uint8_t m_defaultItemIndex;
};
