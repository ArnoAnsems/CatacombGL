// Copyright (C) 2018 Arno Ansems
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

//
// ISystem
//
// Abstract interface towards OS-dependent functionality.
//
#pragma once

#include <string>
#include <vector>

class ISystem
{
public:
    virtual ~ISystem() {};
    virtual const std::string& GetConfigurationFilePath() const = 0;
    virtual void GetSavedGameNamesFromFolder(const std::string& path, std::vector<std::string>& filesFound) const = 0;
    virtual bool CreatePath(const std::string& path) const = 0;
    virtual std::string GetOSVersion() const = 0;
    virtual bool isBuiltIn64Bit() const = 0;
};