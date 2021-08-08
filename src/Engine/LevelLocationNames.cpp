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

#include "LevelLocationNames.h"
#include <string.h>

const std::string UnnamedLocation = "";

LevelLocationNames::LevelLocationNames(const FileChunk* decompressedChunk)
{
    m_LocationNames.clear();

    uint16_t charIndex = 0;
    uint16_t startOfName = 0;
    while (charIndex < decompressedChunk->GetSize())
    {
        const char currentChar = decompressedChunk->GetChunk()[charIndex];
        if (currentChar == '\r')
        {
            LocationName locationName = { UnnamedLocation, false };
            // In Apocalypse, some names start with '@' to indicate that the borders should flash.
            if (decompressedChunk->GetChunk()[startOfName] == '@')
            {
                locationName.borderShouldFlash = true;
                startOfName++;
            }
            const uint16_t nameLength = charIndex - startOfName;
            if (nameLength > 0)
            {
                char* name = new char[nameLength + 1];

                for (int i = 0; i < nameLength; i++)
                {
                    name[i] = decompressedChunk->GetChunk()[startOfName + i];
                }
                name[nameLength] = 0;
                locationName.name = name;
                m_LocationNames.push_back(locationName);

                delete[] name;
            }
            else
            {
                m_LocationNames.push_back(locationName);
            }
            startOfName = charIndex + 2;
        }
        charIndex++;
    }
}

LevelLocationNames::~LevelLocationNames()
{
    m_LocationNames.clear();
}


const std::string& LevelLocationNames::GetLocationName(const uint8_t index) const
{
    if (index < m_LocationNames.size())
    {
        return m_LocationNames.at(index).name;
    }

    return UnnamedLocation;
}

const bool LevelLocationNames::BorderShouldFlash(const uint8_t index) const
{
    if (index < m_LocationNames.size())
    {
        return m_LocationNames.at(index).borderShouldFlash;
    }

    return false;
}