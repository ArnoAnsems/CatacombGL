// Copyright (C) 2022 Arno Ansems
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

#include <cstdint>
#include <map>

class SavedGameInDosFormat;
class ISavedGameConverter;
class Actor;
struct DecorateActor;

class SavedGameInDosFormatLoader
{
public:
    SavedGameInDosFormatLoader(
        const SavedGameInDosFormat& savedGameInDosFormat,
        const ISavedGameConverter& savedGameConverter,
        const std::map<uint16_t, const DecorateActor>& decorateActors);
    ~SavedGameInDosFormatLoader() = default;

    Actor* LoadPlayerActor() const;
    void LoadActors(
        Actor** blockingActors,
        Actor** nonBlockingActors,
        const uint16_t levelWidth,
        const uint16_t levelHeight);
    uint32_t GetPlayerState32() const;

private:
    static constexpr float DosToGLCoordinate(const int32_t dosCoordinate);
    static constexpr float DosToGLAngle(const int16_t dosAngle);

    const SavedGameInDosFormat& m_savedGameInDosFormat;
    const ISavedGameConverter& m_savedGameConverter;
    const std::map<uint16_t, const DecorateActor>& m_decorateActors;
};
