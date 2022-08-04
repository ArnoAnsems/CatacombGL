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

#include "PlayerInventory.h"
#include "IGame.h"

PlayerInventory::PlayerInventory(IGame& game) :
    m_game(game),
    m_bolts(0),
    m_nukes(0),
    m_potions(0),
    m_boltsInChest(0),
    m_nukesInChest(0),
    m_potionsInChest(0)
{
    for (uint8_t key = 0; key < maxKeys; key++)
    {
        m_keys[key] = 0;
    }

    for (uint8_t scroll = 0; scroll < maxScrolls; scroll++)
    {
        m_scrolls[scroll] = false;
    }

    for (uint8_t gem = 0; gem < maxGems; gem++)
    {
        m_gems[gem] = false;
    }
}

PlayerInventory::~PlayerInventory()
{

}

uint8_t PlayerInventory::GetBolts() const
{
    return m_bolts;
}

uint8_t PlayerInventory::GetNukes() const
{
    return m_nukes;
}

uint8_t PlayerInventory::GetPotions() const
{
    return m_potions;
}

uint8_t PlayerInventory::GetKeys(const uint8_t key) const
{
    return (key < maxKeys) ? m_keys[key] : 0;
}

bool PlayerInventory::GetScroll(const uint8_t scroll) const
{
    return (scroll < maxScrolls) ? m_scrolls[scroll] : false;
}

bool PlayerInventory::GetGem(const uint8_t gem) const
{
    return (gem < maxGems) ? m_gems[gem] : false;
}

void PlayerInventory::GivePotion()
{
    if (m_potions == 99)
    {
        return;
    }

    m_game.PlaySoundGetPotion();
    m_potions++;
}

bool PlayerInventory::UsePotion()
{
    if (m_potions > 0)
    {
        m_game.PlaySoundUsePotion();
        m_potions--;
        return true;
    }
    else
    {
        m_game.PlaySoundNoItem();
        return false;
    }
}

void PlayerInventory::GiveBolt()
{
    if (m_bolts == 99)
        return;

    m_game.PlaySoundGetBolt();
    m_bolts++;
}

void PlayerInventory::GiveChest()
{
    m_boltsInChest = rand() % 5;
    m_nukesInChest = rand() % 4;
    m_potionsInChest = rand() % 3;
}

bool PlayerInventory::HasItemsInChest() const
{
    return (m_boltsInChest != 0 || m_nukesInChest != 0 || m_potionsInChest != 0);
}

void PlayerInventory::GiveNextItemInChest()
{
    if (m_boltsInChest > 0)
    {
        GiveBolt();
        m_boltsInChest--;
    }
    else if (m_nukesInChest > 0)
    {
        GiveNuke();
        m_nukesInChest--;
    }
    else if (m_potionsInChest > 0)
    {
        GivePotion();
        m_potionsInChest--;
    }
}

void PlayerInventory::GiveNuke()
{
    if (m_nukes == 99)
        return;

    m_game.PlaySoundGetNuke();
    m_nukes++;
}

void PlayerInventory::GiveScroll(const uint8_t scrollId)
{
    if (scrollId >= maxScrolls)
    {
        return;
    }

    m_game.PlaySoundGetScroll();

    m_scrolls[scrollId] = true;
}

void PlayerInventory::GiveGem(const uint8_t gemId)
{
    if (gemId >= maxGems)
    {
        return;
    }

    m_game.PlaySoundGetGem();

    m_gems[gemId] = true;
}



void PlayerInventory::GiveKey(const uint8_t keyId)
{
    if (keyId == RedKey2)
    {
        m_keys[RedKey]++;
    }
    else if (keyId < maxKeys)
    {
        m_keys[keyId]++;
    }

    m_game.PlaySoundGetKey();
}

void PlayerInventory::TakeKey(const uint8_t keyId)
{
    if (keyId < maxKeys)
    {
        if (m_keys[keyId] > 0)
        {
            m_keys[keyId]--;
        }
    }
}

bool PlayerInventory::TakeBolt()
{
    if (m_bolts > 0)
    {
        m_bolts--;
        return true;
    }

    return false;
}

bool PlayerInventory::TakeNuke()
{
    if (m_nukes > 0)
    {
        m_nukes--;
        return true;
    }

    return false;
}

void PlayerInventory::ResetForNewGame()
{
    m_bolts = 0;
    m_nukes = 0;
    m_potions = 0;
    for (uint8_t key = 0; key < maxKeys; key++)
    {
        m_keys[key] = 0;
    }

    for (uint8_t scroll = 0; scroll < maxScrolls; scroll++)
    {
        m_scrolls[scroll] = false;
    }

    for (uint8_t gem = 0; gem < maxGems; gem++)
    {
        m_gems[gem] = false;
    }
}

void PlayerInventory::StoreToFile(std::ofstream& file) const
{
    file.write((const char*)&m_bolts, sizeof(m_bolts));
    file.write((const char*)&m_nukes, sizeof(m_nukes));
    file.write((const char*)&m_potions, sizeof(m_potions));
    file.write((const char*)&m_keys, sizeof(m_keys));
    file.write((const char*)&m_scrolls, sizeof(m_scrolls));
    file.write((const char*)&m_gems, sizeof(m_gems));
}

bool PlayerInventory::LoadFromFile(std::ifstream& file)
{
    file.read((char*)&m_bolts, sizeof(m_bolts));
    file.read((char*)&m_nukes, sizeof(m_nukes));
    file.read((char*)&m_potions, sizeof(m_potions));
    file.read((char*)&m_keys, sizeof(m_keys));
    file.read((char*)&m_scrolls, sizeof(m_scrolls));
    file.read((char*)&m_gems, sizeof(m_gems));

    return true;
}

void PlayerInventory::LoadFromDosGame(const SavedGameInDosFormat& savedGameInDosFormat)
{
    m_bolts = (uint8_t)savedGameInDosFormat.GetBolts();
    m_nukes = (uint8_t)savedGameInDosFormat.GetNukes();
    m_potions = (uint8_t)savedGameInDosFormat.GetPotions();
    for (uint8_t i = 0; i < maxKeys; i++)
    {
        m_keys[i] = (uint8_t)savedGameInDosFormat.GetKeys(i);
    }
    for (uint8_t i = 0; i < maxScrolls; i++)
    {
        m_scrolls[i] = (savedGameInDosFormat.GetScrolls(i) != 0);
    }
    for (uint8_t i = 0; i < maxGems; i++)
    {
        m_gems[i] = (savedGameInDosFormat.GetGems(i) != 0);
    }
}