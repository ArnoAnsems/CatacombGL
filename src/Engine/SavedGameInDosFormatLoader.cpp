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

#include "SavedGameInDosFormatLoader.h"
#include "SavedGamesInDosFormat.h"
#include "ISavedGameConverter.h"
#include "Actor.h"

SavedGameInDosFormatLoader::SavedGameInDosFormatLoader(
    const SavedGameInDosFormat& savedGameInDosFormat,
    const ISavedGameConverter& savedGameConverter,
    const std::map<uint16_t, const DecorateActor>& decorateActors) :
    m_savedGameInDosFormat(savedGameInDosFormat),
    m_savedGameConverter(savedGameConverter),
    m_decorateActors(decorateActors)
{
}

Actor* SavedGameInDosFormatLoader::LoadPlayerActor() const
{
    const SavedGameInDosFormat::ObjectInDosFormat& playerObject = m_savedGameInDosFormat.GetObject(0);
    const float playerX = (float)playerObject.x / 65536.0f;
    const float playerY = (float)playerObject.y / 65536.0f;
    Actor* playerActor = new Actor(playerX, playerY, 0, m_decorateActors.at(11));
    playerActor->SetAngle(playerObject.angle);
    playerActor->SetHealth(m_savedGameInDosFormat.GetBody());
    playerActor->SetTile(playerObject.tilex, playerObject.tiley);

    return playerActor;
}