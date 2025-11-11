// Copyright (C) 2024 Arno Ansems
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

#pragma once

#include <map>
#include <list>
#include <cstdint>

enum class GameId
{
    CatacombAbyssv113 = 0,
    CatacombAbyssv124,
    CatacombArmageddonv102,
    CatacombApocalypsev101,
    Catacomb3Dv100,
    Catacomb3Dv122,
    NotDetected
};

inline uint8_t getPersistentGameId(const GameId gameId)
{
    constexpr uint8_t persistentGameIdUnknown = 0u;
    constexpr uint8_t persistentGameIdCatacombAbyssv113 = 1u;
    constexpr uint8_t persistentGameIdCatacombAbyssv124 = 2u;
    constexpr uint8_t persistentGameIdCatacombArmageddonv102 = 3u;
    constexpr uint8_t persistentGameIdCatacombApocalypsev101 = 4u;
    constexpr uint8_t persistentGameIdCatacomb3Dv122 = 5u;
    constexpr uint8_t persistentGameIdCatacomb3Dv100 = 6u;

    static const std::map<GameId, uint8_t> gameIdToIntMap =
    {
        {GameId::CatacombAbyssv113, persistentGameIdCatacombAbyssv113},
        {GameId::CatacombAbyssv124, persistentGameIdCatacombAbyssv124},
        {GameId::CatacombArmageddonv102, persistentGameIdCatacombArmageddonv102},
        {GameId::CatacombApocalypsev101, persistentGameIdCatacombApocalypsev101},
        {GameId::Catacomb3Dv100, persistentGameIdCatacomb3Dv100},
        {GameId::Catacomb3Dv122, persistentGameIdCatacomb3Dv122}
    };

    const auto it = gameIdToIntMap.find(gameId);
    return (it != gameIdToIntMap.end()) ? it->second : persistentGameIdUnknown;
}

inline const std::list<GameId>& getAllGameIds()
{
    static const std::list<GameId> allGameIds
    {
        GameId::CatacombAbyssv113,
        GameId::CatacombAbyssv124,
        GameId::CatacombArmageddonv102,
        GameId::CatacombApocalypsev101,
        GameId::Catacomb3Dv100,
        GameId::Catacomb3Dv122
    };
    return allGameIds;
}
