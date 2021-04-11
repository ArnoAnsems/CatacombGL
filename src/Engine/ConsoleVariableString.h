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

class ConsoleVariableString : public ConsoleVariable
{
public:
    ConsoleVariableString(const std::string& nameInMenu, const std::string& nameInConfigFile, const std::string& defaultValue);
    ~ConsoleVariableString();

    const std::string Serialize() const override;
    void Deserialize(const std::string& str) override;
    void SetToDefault() override;

    const std::string& Get() const;
    void Set(const std::string& value);

private:
    std::string m_value;
    const std::string m_defaultValue;
};

