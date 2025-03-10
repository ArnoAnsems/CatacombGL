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
#include "SavedGameInDosFormatConfig.h"

class SavedGameInDosFormat
{
public:
    typedef struct
    {
        uint16_t active;
        int16_t ticcount;
        uint16_t obclass;
        uint16_t state16;
        uint32_t state32;
        uint16_t shootable;
        uint16_t flags;
        uint16_t tileObject;
        int32_t distance;
        uint16_t dir;
        int32_t x;
        int32_t y;
        uint16_t tilex;
        uint16_t tiley;
        int16_t viewx;
        uint16_t viewheight;
        int16_t angle;
        int16_t hitpoints;
        int32_t speed;
        uint16_t size;
        int32_t xl;
        int32_t xh;
        int32_t yl;
        int32_t yh;
        int16_t temp1;
        int16_t temp2;
        int16_t next;
        int16_t prev;
    } ObjectInDosFormat;

    SavedGameInDosFormat(const FileChunk* fileChunk, const SavedGameInDosFormatConfig& config);
    SavedGameInDosFormat(const FileChunk* fileChunk, const SavedGameInDosFormatConfig& config, const std::string& name);
    ~SavedGameInDosFormat();

    bool Load();
    const std::string& GetErrorMessage() const;

    const std::string& GetSignature() const;
    bool GetPresent() const;
    const std::string& GetName() const;
    int16_t GetFreezeTime() const;
    int16_t GetDifficulty() const;
    int16_t GetMapOn() const;
    int16_t GetBolts() const;
    int16_t GetNukes() const;
    int16_t GetPotions() const;
    int16_t GetKeys(uint8_t index) const;
    int16_t GetScrolls(uint8_t index) const;
    int16_t GetGems(uint8_t index) const;
    int32_t GetScore() const;
    int16_t GetBody() const;
    int16_t GetShotpower() const;
    bool GetEasyModeOn() const;
    uint16_t GetSkyColor() const;
    uint16_t GetGroundColor() const;
    int16_t GetMapHeight() const;
    int16_t GetMapWidth() const;
    FileChunk* GetPlane0() const;
    FileChunk* GetPlane2() const;
    uint16_t GetNumberOfObjects() const;
    ObjectInDosFormat& GetObject(const uint16_t objectIndex) const;

private:
    int16_t ReadInt(const uint32_t offset);
    uint16_t ReadUInt(const uint32_t offset);
    int32_t ReadLong(const uint32_t offset);
    void ReadSignature(uint32_t& offset);
    void ReadOldTest(uint32_t& offset);
    void ReadPresent(uint32_t& offset);
    void ReadName(uint32_t& offset);
    void ReadFreezeTime(uint32_t& offset);
    void ReadDifficulty(uint32_t& offset);
    void ReadMapOn(uint32_t& offset);
    void ReadBolts(uint32_t& offset);
    void ReadNukes(uint32_t& offset);
    void ReadPotions(uint32_t& offset);
    void ReadKeys(uint32_t& offset);
    void ReadScrolls(uint32_t& offset);
    void ReadGems(uint32_t& offset);
    void ReadScore(uint32_t& offset);
    void ReadBody(uint32_t& offset);
    void ReadShotpower(uint32_t& offset);
    void ReadEasyModeOn(uint32_t& offset);
    void ReadSkyColor(uint32_t& offset);
    void ReadGroundColor(uint32_t& offset);
    void ReadMapWidth(uint32_t& offset);
    void ReadMapHeight(uint32_t& offset);
    void ReadPlane0(uint32_t& offset);
    void ReadPlane2(uint32_t& offset);
    void ReadObject(uint16_t objectIndex, uint32_t& offset);
    void ReadHeaderItem(const HeaderItem item, uint32_t& offset);
    void ReadObjectItem(ObjectInDosFormat* const object, const ObjectItem item, uint32_t& offset);
    uint32_t GetSizeOfObjectItem(const ObjectItem item) const;
    uint32_t GetSizeOfObject() const;
    const FileChunk* m_fileChunk;
    const SavedGameInDosFormatConfig& m_config;
    std::string m_errorMessage;
    std::string m_signature;
    int16_t m_oldTest;
    bool m_present;
    std::string m_name;
    int16_t m_freezeTime;
    int16_t m_difficulty;
    int16_t m_mapOn;
    int16_t m_bolts;
    int16_t m_nukes;
    int16_t m_potions;
    int16_t m_keys[4];
    int16_t m_scrolls[8];
    int16_t m_gems[5];
    int32_t m_score;
    int16_t m_body;
    int16_t m_shotpower;
    bool m_easyModeOn;
    uint16_t m_skycolor;
    uint16_t m_groundcolor;
    int16_t m_mapWidth;
    int16_t m_mapHeight;
    FileChunk* m_plane0;
    FileChunk* m_plane2;
    uint16_t m_numberOfObjects;
    ObjectInDosFormat* m_objects;
    bool m_dataIsValid;
};
