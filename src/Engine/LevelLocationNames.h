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
// LevelLocationNames
//
// The names of locations in a level, as read from a chunk of the EGAGRAPH repository file and
// displayed in the statusbar.
//
#pragma once

#include "FileChunk.h"
#include <string>
#include <vector>

class LevelLocationNames
{
public:
    LevelLocationNames(const FileChunk* decompressedChunk);
    ~LevelLocationNames();

    const std::string& GetLocationName(const uint8_t index) const;
    const bool BorderShouldFlash(const uint8_t index) const;

private:
    typedef struct
    {
        std::string name;
        bool borderShouldFlash;
    } LocationName;

    std::vector<LocationName> m_LocationNames;
};

