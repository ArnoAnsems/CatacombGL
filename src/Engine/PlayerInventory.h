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
// PlayerInventory
//
// Keeps track of the player inventory, such as collected keys, scrolls, gems, etc.
//
#pragma once

#include <stdint.h>
#include <fstream>

enum KeyId
{
    RedKey,
    YellowKey,
    GreenKey,
    BlueKey,
    RedKey2,
    NoKey
};

class AudioPlayer;

class PlayerInventory
{
public:
    PlayerInventory(AudioPlayer* audioPlayer);
    ~PlayerInventory();

    uint8_t GetBolts() const;
    uint8_t GetNukes() const;
    uint8_t GetPotions() const;
    uint8_t GetKeys(const uint8_t key) const;
    bool GetScroll(const uint8_t scroll) const;
    bool GetGem(const uint8_t gem) const;
    void GivePotion();
    bool UsePotion();
    void GiveBolt();
    void GiveChest();
    void GiveNuke();
    void GiveScroll(const uint8_t scrollId);
    void GiveKey(const uint8_t keyId);
    void GiveGem(const uint8_t gemId);
    bool HasItemsInChest() const;
    void GiveNextItemInChest();

    void TakeKey(const uint8_t keyId);
    bool TakeBolt();
    bool TakeNuke();
    void ResetForNewGame();

    void StoreToFile(std::ofstream& file) const;
    bool LoadFromFile(std::ifstream& file);

private:
    static const uint8_t maxKeys = 4;
    static const uint8_t maxScrolls = 8;
    static const uint8_t maxGems = 5;

    uint8_t m_bolts;
    uint8_t m_nukes;
    uint8_t m_potions;
    uint8_t m_keys[maxKeys];
    bool m_scrolls[maxScrolls];
    bool m_gems[maxGems];

    // Contents of chest
    uint8_t m_boltsInChest;
    uint8_t m_nukesInChest;
    uint8_t m_potionsInChest;

    AudioPlayer* m_audioPlayer;
};

