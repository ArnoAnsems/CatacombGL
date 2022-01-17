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
#include "Decompressor.h"

SavedGameInDosFormat::SavedGameInDosFormat(const FileChunk* fileChunk, const DosFormatConfig& config) :
    m_fileChunk(fileChunk),
    m_config(config),
    m_plane0(nullptr),
    m_plane2(nullptr),
    m_objects(nullptr),
    m_dataIsValid(false)
{

}

SavedGameInDosFormat::~SavedGameInDosFormat()
{
    delete m_plane0;
    delete m_plane2;
    delete[] m_objects;
}

bool SavedGameInDosFormat::Load()
{
    m_dataIsValid = true;

    if (m_fileChunk == nullptr)
    {
        m_errorMessage = "data is null";
        m_dataIsValid = false;
    }

    if (m_dataIsValid && m_fileChunk->GetSize() < 88)
    {
        m_errorMessage = "too small to contain header";
        m_dataIsValid = false;
    }

    uint32_t offset = 0;
    if (m_dataIsValid)
    {
        ReadSignature(offset);
        ReadOldTest(offset);
        ReadPresent(offset);
        ReadName(offset);
        ReadDifficulty(offset);
        ReadMapOn(offset);
        ReadBolts(offset);
        ReadNukes(offset);
        ReadPotions(offset);
        ReadKeys(offset);
        ReadScrolls(offset);
        ReadScore(offset);
        ReadBody(offset);
        ReadShotpower(offset);
    }

    if (m_dataIsValid)
    {
        ReadPlane0(offset);
    }
        
    if (m_dataIsValid)
    {
        ReadPlane2(offset);
    }

    const uint16_t offsetToFirstObject = offset;
    const uint16_t sizeOfSingleObject = 68u;
    if (m_dataIsValid)
    {
        m_numberOfObjects = (m_fileChunk->GetSize() - offsetToFirstObject) / sizeOfSingleObject;

        if (m_numberOfObjects == 0)
        {
            m_errorMessage = "no objects found";
            m_dataIsValid = false;
        }
    }

    if (m_dataIsValid)
    {
        if (m_objects)
        {
            delete[] m_objects;
        }
        m_objects = new ObjectInDosFormat[m_numberOfObjects];
        for (uint16_t i = 0; i < m_numberOfObjects; i++)
        {
            const uint8_t* pointerToObject = m_fileChunk->GetChunk() + offsetToFirstObject + (i * sizeOfSingleObject);
            // The ObjectInDosFormat struct has to be filled element by element, due to the different alignment under Windows.
            std::memcpy(&m_objects[i].active, pointerToObject, 2);
            std::memcpy(&m_objects[i].ticcount, pointerToObject + 2, 2);
            std::memcpy(&m_objects[i].obclass, pointerToObject + 4, 2);
            std::memcpy(&m_objects[i].stateOffset, pointerToObject + 6, 2);
            std::memcpy(&m_objects[i].shootable, pointerToObject + 8, 2);
            std::memcpy(&m_objects[i].tileObject, pointerToObject + 10, 2);
            std::memcpy(&m_objects[i].distance, pointerToObject + 12, 4);
            std::memcpy(&m_objects[i].dir, pointerToObject + 16, 2);
            std::memcpy(&m_objects[i].x, pointerToObject + 18, 4);
            std::memcpy(&m_objects[i].y, pointerToObject + 22, 4);
            std::memcpy(&m_objects[i].tilex, pointerToObject + 26, 2);
            std::memcpy(&m_objects[i].tiley, pointerToObject + 28, 2);
            std::memcpy(&m_objects[i].viewx, pointerToObject + 30, 2);
            std::memcpy(&m_objects[i].viewheight, pointerToObject + 32, 2);
            std::memcpy(&m_objects[i].angle, pointerToObject + 34, 2);
            std::memcpy(&m_objects[i].hitpoints, pointerToObject + 36, 2);
            std::memcpy(&m_objects[i].speed, pointerToObject + 38, 4);
            std::memcpy(&m_objects[i].size, pointerToObject + 42, 2);
            std::memcpy(&m_objects[i].xl, pointerToObject + 44, 4);
            std::memcpy(&m_objects[i].xh, pointerToObject + 48, 4);
            std::memcpy(&m_objects[i].yl, pointerToObject + 52, 4);
            std::memcpy(&m_objects[i].yh, pointerToObject + 56, 4);
            std::memcpy(&m_objects[i].temp1, pointerToObject + 60, 2);
            std::memcpy(&m_objects[i].temp2, pointerToObject + 62, 2);
            std::memcpy(&m_objects[i].next, pointerToObject + 64, 2);
            std::memcpy(&m_objects[i].prev, pointerToObject + 66, 2);
        }
    }

    return m_dataIsValid;
}

const std::string& SavedGameInDosFormat::GetErrorMessage() const
{
    return m_errorMessage;
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

FileChunk* SavedGameInDosFormat::GetPlane0() const
{
    return m_plane0;
}

FileChunk* SavedGameInDosFormat::GetPlane2() const
{
    return m_plane2;
}

uint16_t SavedGameInDosFormat::GetNumberOfObjects() const
{
    return m_numberOfObjects;
}

int16_t SavedGameInDosFormat::ReadInt(const uint32_t offset)
{
    int16_t dest;
    std::memcpy(&dest, m_fileChunk->GetChunk() + offset, 2);
    return dest;
}

int32_t SavedGameInDosFormat::ReadLong(const uint32_t offset)
{
    int32_t dest;
    std::memcpy(&dest, m_fileChunk->GetChunk() + offset, 4);
    return dest;
}

SavedGameInDosFormat::ObjectInDosFormat& SavedGameInDosFormat::GetObject(const uint16_t objectIndex) const
{
    return m_objects[objectIndex];
}

void SavedGameInDosFormat::ReadSignature(uint32_t& offset)
{
    constexpr uint32_t signatureSize = 4;
    char tempSignature[signatureSize];
    std::memcpy(tempSignature, m_fileChunk->GetChunk() + offset, sizeof(tempSignature));
    m_signature = std::string(tempSignature);
    offset += signatureSize;
}

void SavedGameInDosFormat::ReadOldTest(uint32_t& offset)
{
    m_oldTest = ReadInt(offset);
    offset += sizeof(m_oldTest);
}

void SavedGameInDosFormat::ReadPresent(uint32_t& offset)
{
    const int16_t presentAsInt = ReadInt(offset);
    m_present = (presentAsInt == 0);
    offset += sizeof(presentAsInt);
}

void SavedGameInDosFormat::ReadName(uint32_t& offset)
{
    constexpr uint32_t nameSize = 33;
    char tempName[nameSize];
    std::memcpy(tempName, m_fileChunk->GetChunk() + offset, sizeof(tempName));
    m_name = std::string(tempName);
    // +1 due to byte alignment
    offset += (nameSize + 1);
}

void SavedGameInDosFormat::ReadDifficulty(uint32_t& offset)
{
    m_difficulty = ReadInt(offset);
    offset += sizeof(m_difficulty);
}

void SavedGameInDosFormat::ReadMapOn(uint32_t& offset)
{
    m_mapOn = ReadInt(offset);
    offset += sizeof(m_mapOn);
}

void SavedGameInDosFormat::ReadBolts(uint32_t& offset)
{
    m_bolts = ReadInt(offset);
    offset += sizeof(m_bolts);
}

void SavedGameInDosFormat::ReadNukes(uint32_t& offset)
{
    m_nukes = ReadInt(offset);
    offset += sizeof(m_nukes);
}

void SavedGameInDosFormat::ReadPotions(uint32_t& offset)
{
    m_potions = ReadInt(offset);
    offset += sizeof(m_potions);
}

void SavedGameInDosFormat::ReadKeys(uint32_t& offset)
{
    const uint32_t keySize = sizeof(m_keys[0]);
    constexpr uint8_t numberOfKeys = 4;
    for (uint8_t i = 0; i < numberOfKeys; i++)
    {
        m_keys[i] = ReadInt(offset);
        offset += keySize;
    }
}

void SavedGameInDosFormat::ReadScrolls(uint32_t& offset)
{
    const uint32_t scrollSize = sizeof(m_scrolls[0]);
    constexpr uint8_t numberOfScrolls = 8;
    for (uint8_t i = 0; i < numberOfScrolls; i++)
    {
        m_scrolls[i] = ReadInt(offset);
        offset += scrollSize;
    }
}

void SavedGameInDosFormat::ReadScore(uint32_t& offset)
{
    m_score = ReadLong(offset);
    offset += sizeof(m_score);
}

void SavedGameInDosFormat::ReadBody(uint32_t& offset)
{
    m_body = ReadInt(offset);
    offset += sizeof(m_body);
}

void SavedGameInDosFormat::ReadShotpower(uint32_t& offset)
{
    m_shotpower = ReadInt(offset);
    offset += sizeof(m_shotpower);
}

void SavedGameInDosFormat::ReadPlane0(uint32_t& offset)
{
    uint16_t plane0CompressedSize = 0;
    const uint16_t plane0MaxCompressedSize = m_fileChunk->GetSize() - offset - sizeof(plane0CompressedSize);
    m_plane0 = Decompressor::RLEW_DecompressFromSavedGame(
        &(m_fileChunk->GetChunk()[offset]),
        0xABCD,
        plane0MaxCompressedSize,
        plane0CompressedSize);

    if (m_plane0 == nullptr)
    {
        m_errorMessage = "unable to decompress plane 0";
        m_dataIsValid = false;
    }
    else
    {
        offset += (plane0CompressedSize + sizeof(plane0CompressedSize));
    }
}

void SavedGameInDosFormat::ReadPlane2(uint32_t& offset)
{
    uint16_t plane2CompressedSize = 0;
    const uint16_t plane2MaxCompressedSize = m_fileChunk->GetSize() - offset - sizeof(plane2CompressedSize);
    m_plane2 = Decompressor::RLEW_DecompressFromSavedGame(
        &(m_fileChunk->GetChunk()[offset]),
        0xABCD,
        plane2MaxCompressedSize,
        plane2CompressedSize);

    if (m_plane2 == nullptr)
    {
        m_errorMessage = "unable to decompress plane 2";
        m_dataIsValid = false;
    }
    else
    {
        offset += (plane2CompressedSize + sizeof(plane2CompressedSize));
    }
}