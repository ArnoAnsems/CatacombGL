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

//
// SavedGameInDosFormat
//
// Reads the content of a saved game that was stored in the original DOS format.
//
#pragma once

#include "FileChunk.h"
#include <string>

class SavedGameInDosFormat
{
public:
    SavedGameInDosFormat(const FileChunk* fileChunk);
    ~SavedGameInDosFormat();

    bool Load();

    const std::string& GetSignature() const;
    bool GetPresent() const;
    const std::string& GetName() const;
    int16_t GetDifficulty() const;
    int16_t GetMapOn() const;
    int16_t GetBolts() const;
    int16_t GetNukes() const;
    int16_t GetPotions() const;
    int16_t GetKeys(uint8_t index) const;
    int16_t GetScrolls(uint8_t index) const;
    int32_t GetScore() const;
    int16_t GetBody() const;
    int16_t GetShotpower() const;
    FileChunk* GetPlane0() const;
    FileChunk* GetPlane2() const;

private:
    int16_t ReadInt(const uint32_t offset);
    int32_t ReadLong(const uint32_t offset);

    const FileChunk* m_fileChunk;
    std::string m_signature;
    bool m_present;
    std::string m_name;
    int16_t m_difficulty;
    int16_t m_mapOn;
    int16_t m_bolts;
    int16_t m_nukes;
    int16_t m_potions;
    int16_t m_keys[4];
    int16_t m_scrolls[8];
    int32_t m_score;
    int16_t m_body;
    int16_t m_shotpower;
    FileChunk* m_plane0;
    FileChunk* m_plane2;
};
