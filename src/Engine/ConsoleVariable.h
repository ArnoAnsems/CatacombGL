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

#include <string>

class ConsoleVariable
{
public:
    ConsoleVariable();
    ConsoleVariable(const std::string& nameInMenu, const std::string& nameInConfigFile);
    ~ConsoleVariable();

    const std::string& GetNameInMenu() const;
    const std::string& GetNameInConfigFile() const;

    virtual const std::string Serialize() const;
    virtual void Deserialize(const std::string& str);

protected:
    const std::string m_nameInMenu;
    const std::string m_nameInConfigFile;
};
