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

#include "SavedGameInDosFormat.h"

SavedGameInDosFormat::SavedGameInDosFormat(const FileChunk* fileChunk) :
    m_fileChunk(fileChunk)
{

}

SavedGameInDosFormat::~SavedGameInDosFormat()
{

}

bool SavedGameInDosFormat::Load()
{
    char tempSignature[4];
    std::memcpy(tempSignature, m_fileChunk->GetChunk(), sizeof(tempSignature));
    m_signature = std::string(tempSignature);

    m_present = (ReadInt(6) == 0);

    char tempName[33];
    std::memcpy(tempName, m_fileChunk->GetChunk() + 8, sizeof(tempName));
    m_name = std::string(tempName);

    m_difficulty = ReadInt(41);
    m_mapOn = ReadInt(43);
    m_bolts = ReadInt(45);
    m_nukes = ReadInt(47);
    m_potions = ReadInt(49);
    for (uint8_t i = 0; i < 4; i++)
    {
        m_keys[i] = ReadInt(51 + (i * 2));
    }
    for (uint8_t i = 0; i < 8; i++)
    {
        m_scrolls[i] = ReadInt(59 + (i * 2));
    }
    m_score = ReadLong(75);
    m_body = ReadInt(79);
    m_shotpower = ReadInt(81);

    return true;
}

const std::string& SavedGameInDosFormat::GetSignature() const
{
    return m_signature;
}

bool SavedGameInDosFormat::GetPresent() const
{
    return m_present;
}

const std::string& SavedGameInDosFormat::GetName() const
{
    return m_name;
}

int16_t SavedGameInDosFormat::GetDifficulty() const
{
    return m_difficulty;
}

int16_t SavedGameInDosFormat::GetMapOn() const
{
    return m_mapOn;
}

int16_t SavedGameInDosFormat::GetBolts() const
{
    return m_bolts;
}

int16_t SavedGameInDosFormat::GetNukes() const
{
    return m_nukes;
}

int16_t SavedGameInDosFormat::GetPotions() const
{
    return m_potions;
}

int16_t SavedGameInDosFormat::GetKeys(uint8_t index) const
{
    return (index < 4) ? m_keys[index] : 0;
}

int16_t SavedGameInDosFormat::GetScrolls(uint8_t index) const
{
    return (index < 8) ? m_scrolls[8] : 0;
}

int32_t SavedGameInDosFormat::GetScore() const
{
    return m_score;
}

int16_t SavedGameInDosFormat::GetBody() const
{
    return m_body;
}

int16_t SavedGameInDosFormat::GetShotpower() const
{
    return m_shotpower;
}

int16_t SavedGameInDosFormat::ReadInt(const uint32_t offset)
{
    return ((int16_t)(m_fileChunk->GetChunk()[offset]) << 8) + m_fileChunk->GetChunk()[offset + 1];
}

int32_t SavedGameInDosFormat::ReadLong(const uint32_t offset)
{
    return ((int32_t)(m_fileChunk->GetChunk()[offset + 2]) << 8) + m_fileChunk->GetChunk()[offset + 1];
}