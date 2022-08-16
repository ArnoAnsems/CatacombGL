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
#include "SavedGameInDosFormatConfig.h"
#include "Decompressor.h"

SavedGameInDosFormat::SavedGameInDosFormat(const FileChunk* fileChunk, const SavedGameInDosFormatConfig& config) :
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
    const uint16_t sizeOfSingleObject = GetSizeOfObject();
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

uint16_t SavedGameInDosFormat::GetSkyColor() const
{
    return m_skycolor;
}

uint16_t SavedGameInDosFormat::GetGroundColor() const
{
    return m_groundcolor;
}

int16_t SavedGameInDosFormat::GetMapHeight() const
{
    return m_mapHeight;
}

int16_t SavedGameInDosFormat::GetMapWidth() const
{
    return m_mapWidth;
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

uint16_t SavedGameInDosFormat::ReadUInt(const uint32_t offset)
{
    uint16_t dest;
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

void SavedGameInDosFormat::ReadSkyColor(uint32_t& offset)
{
    m_skycolor = ReadUInt(offset);
    offset += sizeof(m_skycolor);
}

void SavedGameInDosFormat::ReadGroundColor(uint32_t& offset)
{
    m_groundcolor = ReadUInt(offset);
    offset += sizeof(m_groundcolor);
}

void SavedGameInDosFormat::ReadMapWidth(uint32_t& offset)
{
    m_mapWidth = ReadInt(offset);
    offset += sizeof(m_mapWidth);
}
void SavedGameInDosFormat::ReadMapHeight(uint32_t& offset)
{
    m_mapHeight = ReadInt(offset);
    offset += sizeof(m_mapHeight);
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
    ObjectInDosFormat* const targetObject = &m_objects[objectIndex];
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
    case HeaderTtemSkyColor:
        ReadSkyColor(offset);
        break;
    case HeaderItemGroundColor:
        ReadGroundColor(offset);
        break;
    case HeaderItemMapHeight:
        ReadMapHeight(offset);
        break;
    case HeaderItemMapWidth:
        ReadMapWidth(offset);
        break;
    default:
        break;
    }
}

void SavedGameInDosFormat::ReadObjectItem(ObjectInDosFormat* const object, const ObjectItem item, uint32_t& offset)
{
    const uint8_t* pointerToItem = m_fileChunk->GetChunk() + offset;
    const uint32_t itemSize = GetSizeOfObjectItem(item);

    switch (item)
    {
    case ObjectActive:
    {
        std::memcpy(&(object->active), pointerToItem, itemSize);
        break;
    }
    case ObjectTiccount:
    {
        std::memcpy(&(object->ticcount), pointerToItem, itemSize);
        break;
    }
    case ObjectObclass:
    {
        std::memcpy(&(object->obclass), pointerToItem, itemSize);
        break;
    }
    case ObjectState16:
    {
        std::memcpy(&(object->state16), pointerToItem, itemSize);
        break;
    }
    case ObjectState32:
    {
        std::memcpy(&(object->state32), pointerToItem, itemSize);
        break;
    }
    case ObjectShootable:
    {
        std::memcpy(&(object->shootable), pointerToItem, itemSize);
        break;
    }
    case ObjectFlags:
    {
        std::memcpy(&(object->flags), pointerToItem, itemSize);
        break;
    }
    case ObjectTileObject:
    {
        std::memcpy(&(object->tileObject), pointerToItem, itemSize);
        break;
    }
    case ObjectDistance:
    {
        std::memcpy(&(object->distance), pointerToItem, itemSize);
        break;
    }
    case ObjectDir:
    {
        std::memcpy(&(object->dir), pointerToItem, itemSize);
        break;
    }
    case ObjectX:
    {
        std::memcpy(&(object->x), pointerToItem, itemSize);
        break;
    }
    case ObjectY:
    {
        std::memcpy(&(object->y), pointerToItem, itemSize);
        break;
    }
    case ObjectTileX:
    {
        std::memcpy(&(object->tilex), pointerToItem, itemSize);
        break;
    }
    case ObjectTileY:
    {
        std::memcpy(&(object->tiley), pointerToItem, itemSize);
        break;
    }
    case ObjectViewX:
    {
        std::memcpy(&(object->viewx), pointerToItem, itemSize);
        break;
    }
    case ObjectViewHeight:
    {
        std::memcpy(&(object->viewheight), pointerToItem, itemSize);
        break;
    }
    case ObjectAngle:
    {
        std::memcpy(&(object->angle), pointerToItem, itemSize);
        break;
    }
    case ObjectHitpoints:
    {
        std::memcpy(&(object->hitpoints), pointerToItem, itemSize);
        break;
    }
    case ObjectSpeed:
    {
        std::memcpy(&(object->speed), pointerToItem, itemSize);
        break;
    }
    case ObjectSize:
    {
        std::memcpy(&(object->size), pointerToItem, itemSize);
        break;
    }
    case ObjectXL:
    {
        std::memcpy(&(object->xl), pointerToItem, itemSize);
        break;
    }
    case ObjectXH:
    {
        std::memcpy(&(object->xh), pointerToItem, itemSize);
        break;
    }
    case ObjectYL:
    {
        std::memcpy(&(object->yl), pointerToItem, itemSize);
        break;
    }
    case ObjectYH:
    {
        std::memcpy(&(object->yh), pointerToItem, itemSize);
        break;
    }
    case ObjectTemp1:
    {
        std::memcpy(&(object->temp1), pointerToItem, itemSize);
        break;
    }
    case ObjectTemp2:
    {
        std::memcpy(&(object->temp2), pointerToItem, itemSize);
        break;
    }
    case ObjectNext:
    {
        std::memcpy(&(object->next), pointerToItem, itemSize);
        break;
    }
    case ObjectPrev:
    {
        std::memcpy(&(object->prev), pointerToItem, itemSize);
        break;
    }
    default:
    {
        break;
    }
    }

    offset += itemSize;
}

uint32_t SavedGameInDosFormat::GetSizeOfObjectItem(const ObjectItem item) const
{
    uint32_t itemSize = 0;

    switch (item)
    {
    case ObjectActive:
    {
        itemSize = sizeof(ObjectInDosFormat::active);
        break;
    }
    case ObjectTiccount:
    {
        itemSize = sizeof(ObjectInDosFormat::ticcount);
        break;
    }
    case ObjectObclass:
    {
        itemSize = sizeof(ObjectInDosFormat::obclass);
        break;
    }
    case ObjectState16:
    {
        itemSize = sizeof(ObjectInDosFormat::state16);
        break;
    }
    case ObjectState32:
    {
        itemSize = sizeof(ObjectInDosFormat::state32);
        break;
    }
    case ObjectShootable:
    {
        itemSize = sizeof(ObjectInDosFormat::shootable);
        break;
    }
    case ObjectFlags:
    {
        itemSize = sizeof(ObjectInDosFormat::flags);
        break;
    }
    case ObjectTileObject:
    {
        itemSize = sizeof(ObjectInDosFormat::tileObject);
        break;
    }
    case ObjectDistance:
    {
        itemSize = sizeof(ObjectInDosFormat::distance);
        break;
    }
    case ObjectDir:
    {
        itemSize = sizeof(ObjectInDosFormat::dir);
        break;
    }
    case ObjectX:
    {
        itemSize = sizeof(ObjectInDosFormat::x);
        break;
    }
    case ObjectY:
    {
        itemSize = sizeof(ObjectInDosFormat::y);
        break;
    }
    case ObjectTileX:
    {
        itemSize = sizeof(ObjectInDosFormat::tilex);
        break;
    }
    case ObjectTileY:
    {
        itemSize = sizeof(ObjectInDosFormat::tiley);
        break;
    }
    case ObjectViewX:
    {
        itemSize = sizeof(ObjectInDosFormat::viewx);
        break;
    }
    case ObjectViewHeight:
    {
        itemSize = sizeof(ObjectInDosFormat::viewheight);
        break;
    }
    case ObjectAngle:
    {
        itemSize = sizeof(ObjectInDosFormat::angle);
        break;
    }
    case ObjectHitpoints:
    {
        itemSize = sizeof(ObjectInDosFormat::hitpoints);
        break;
    }
    case ObjectSpeed:
    {
        itemSize = sizeof(ObjectInDosFormat::speed);
        break;
    }
    case ObjectSize:
    {
        itemSize = sizeof(ObjectInDosFormat::size);
        break;
    }
    case ObjectXL:
    {
        itemSize = sizeof(ObjectInDosFormat::xl);
        break;
    }
    case ObjectXH:
    {
        itemSize = sizeof(ObjectInDosFormat::xh);
        break;
    }
    case ObjectYL:
    {
        itemSize = sizeof(ObjectInDosFormat::yl);
        break;
    }
    case ObjectYH:
    {
        itemSize = sizeof(ObjectInDosFormat::yh);
        break;
    }
    case ObjectTemp1:
    {
        itemSize = sizeof(ObjectInDosFormat::temp1);
        break;
    }
    case ObjectTemp2:
    {
        itemSize = sizeof(ObjectInDosFormat::temp2);
        break;
    }
    case ObjectNext:
    {
        itemSize = sizeof(ObjectInDosFormat::next);
        break;
    }
    case ObjectPrev:
    {
        itemSize = sizeof(ObjectInDosFormat::prev);
        break;
    }
    default:
    {
        break;
    }
    }

    return itemSize;
}

uint32_t SavedGameInDosFormat::GetSizeOfObject() const
{
    uint32_t objectSize = 0;
    for (const ObjectItem item : m_config.objectItems)
    {
        objectSize += GetSizeOfObjectItem(item);
    }
    return objectSize;
}