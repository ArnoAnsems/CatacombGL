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

#include "ConsoleVariable.h"

class ConsoleVariableInt : public ConsoleVariable
{
public:
    ConsoleVariableInt(
        const std::string& nameInMenu,
        const std::string& nameInConfigFile,
        const int32_t minValue,
        const int32_t maxValue,
        const int32_t defaultValue);
    ~ConsoleVariableInt();

    const std::string Serialize() const override;
    void Deserialize(const std::string& str) override;

    int32_t GetValue() const;
    void SetValue(const int32_t value);
    void Increase();
    void Decrease();

private:
    void ClipValueToRange();

    const int32_t m_minValue;
    const int32_t m_maxValue;
    int32_t m_value;
};

