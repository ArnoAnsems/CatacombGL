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
        if (!m_config.gameName.empty())
        {
            // Skip the gameName
            offset += (m_config.gameName.length() + 1);
        }
        if (!m_config.saveVersion.empty())
        {
            // Skip the saveVersion
            offset += (m_config.saveVersion.length() + 1);
        }
        for (HeaderItem item : m_config.headerItems)
        {
            ReadHeaderItem(item, offset);
        }
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
            ReadObject(i, offset);
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

int16_t SavedGameInDosFormat::GetFreezeTime() const
{
    return m_freezeTime;
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
    return (index < 8) ? m_scrolls[index] : 0;
}

int16_t SavedGameInDosFormat::GetGems(uint8_t index) const
{
    return (index < 5) ? m_gems[index] : 0;
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

bool SavedGameInDosFormat::GetEasyModeOn() const
{
    return m_easyModeOn;
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
    char tempSignature[signatureSize + 1];
    std::memcpy(tempSignature, m_fileChunk->GetChunk() + offset, sizeof(tempSignature));
    tempSignature[signatureSize] = 0;
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

void SavedGameInDosFormat::ReadFreezeTime(uint32_t& offset)
{
    m_freezeTime = ReadInt(offset);
    offset += sizeof(m_freezeTime);
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

void SavedGameInDosFormat::ReadGems(uint32_t& offset)
{
    const uint32_t gemSize = sizeof(m_gems[0]);
    constexpr uint8_t numberOfGems = 5;
    for (uint8_t i = 0; i < numberOfGems; i++)
    {
        m_gems[i] = ReadInt(offset);
        offset += gemSize;
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

void SavedGameInDosFormat::ReadEasyModeOn(uint32_t& offset)
{
    const int16_t easyModeOnAsInt = ReadInt(offset);
    m_easyModeOn = (easyModeOnAsInt == 0);
    offset += sizeof(easyModeOnAsInt);
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

void SavedGameInDosFormat::ReadObject(uint16_t objectIndex, uint32_t& offset)
{
    ObjectInDosFormat& targetObject = m_objects[objectIndex];
    for (const ObjectItem item : m_config.objectItems)
    {
        ReadObjectItem(targetObject, item, offset);
    }
}

void SavedGameInDosFormat::ReadHeaderItem(const HeaderItem item, uint32_t& offset)
{
    switch (item)
    {
    case HeaderItemSignature:
        ReadSignature(offset);
        break;
    case HeaderItemOldTest:
        ReadOldTest(offset);
        break;
    case HeaderItemPresent:
        ReadPresent(offset);
        break;
    case HeaderItemName:
        ReadName(offset);
        break;
    case HeaderItemFreezeTime:
        ReadFreezeTime(offset);
        break;
    case HeaderItemDifficulty:
        ReadDifficulty(offset);
        break;
    case HeaderItemMapOn:
        ReadMapOn(offset);
        break;
    case HeaderItemBolts:
        ReadBolts(offset);
        break;
    case HeaderItemNukes:
        ReadNukes(offset);
        break;
    case HeaderItemPotions:
        ReadPotions(offset);
        break;
    case HeaderItemKeys:
        ReadKeys(offset);
        break;
    case HeaderItemScrolls:
        ReadScrolls(offset);
        break;
    case HeaderItemGems:
        ReadGems(offset);
        break;
    case HeaderItemScore:
        ReadScore(offset);
        break;
    case HeaderItemBody:
        ReadBody(offset);
        break;
    case HeaderItemShotpower:
        ReadShotpower(offset);
        break;
    case HeaderItemEasyModeOn:
        ReadEasyModeOn(offset);
        break;
    default:
        break;
    }
}

void SavedGameInDosFormat::ReadObjectItem(ObjectInDosFormat& object, const ObjectItem item, uint32_t& offset)
{
    const uint8_t* pointerToItem = m_fileChunk->GetChunk() + offset;
    switch (item)
    {
    case ObjectActive:
    {
        const uint32_t itemSize = sizeof(object.active);
        std::memcpy(&object.active, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectTiccount:
    {
        const uint32_t itemSize = sizeof(object.ticcount);
        std::memcpy(&object.ticcount, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectObclass:
    {
        const uint32_t itemSize = sizeof(object.obclass);
        std::memcpy(&object.obclass, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectState:
    {
        const uint32_t itemSize = sizeof(object.stateOffset);
        std::memcpy(&object.stateOffset, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectShootable:
    {
        const uint32_t itemSize = sizeof(object.shootable);
        std::memcpy(&object.shootable, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectFlags:
    {
        const uint32_t itemSize = sizeof(object.flags);
        std::memcpy(&object.flags, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectTileObject:
    {
        const uint32_t itemSize = sizeof(object.tileObject);
        std::memcpy(&object.tileObject, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectDistance:
    {
        const uint32_t itemSize = sizeof(object.distance);
        std::memcpy(&object.distance, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectDir:
    {
        const uint32_t itemSize = sizeof(object.dir);
        std::memcpy(&object.dir, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectX:
    {
        const uint32_t itemSize = sizeof(object.x);
        std::memcpy(&object.x, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectY:
    {
        const uint32_t itemSize = sizeof(object.y);
        std::memcpy(&object.y, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectTileX:
    {
        const uint32_t itemSize = sizeof(object.tilex);
        std::memcpy(&object.tilex, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectTileY:
    {
        const uint32_t itemSize = sizeof(object.tiley);
        std::memcpy(&object.tiley, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectViewX:
    {
        const uint32_t itemSize = sizeof(object.viewx);
        std::memcpy(&object.viewx, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectViewHeight:
    {
        const uint32_t itemSize = sizeof(object.viewheight);
        std::memcpy(&object.viewheight, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectAngle:
    {
        const uint32_t itemSize = sizeof(object.angle);
        std::memcpy(&object.angle, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectHitpoints:
    {
        const uint32_t itemSize = sizeof(object.hitpoints);
        std::memcpy(&object.hitpoints, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectSpeed:
    {
        const uint32_t itemSize = sizeof(object.speed);
        std::memcpy(&object.speed, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectSize:
    {
        const uint32_t itemSize = sizeof(object.size);
        std::memcpy(&object.size, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectXL:
    {
        const uint32_t itemSize = sizeof(object.xl);
        std::memcpy(&object.xl, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectXH:
    {
        const uint32_t itemSize = sizeof(object.xh);
        std::memcpy(&object.xh, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectYL:
    {
        const uint32_t itemSize = sizeof(object.yl);
        std::memcpy(&object.yl, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectYH:
    {
        const uint32_t itemSize = sizeof(object.yh);
        std::memcpy(&object.yh, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectTemp1:
    {
        const uint32_t itemSize = sizeof(object.temp1);
        std::memcpy(&object.temp1, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectTemp2:
    {
        const uint32_t itemSize = sizeof(object.temp2);
        std::memcpy(&object.temp2, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectNext:
    {
        const uint32_t itemSize = sizeof(object.next);
        std::memcpy(&object.next, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    case ObjectPrev:
    {
        const uint32_t itemSize = sizeof(object.prev);
        std::memcpy(&object.prev, pointerToItem, itemSize);
        offset += itemSize;
        break;
    }
    default:
    {
        break;
    }
    }
}