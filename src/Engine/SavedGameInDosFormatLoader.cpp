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
    const float playerX = DosToGLCoordinate(playerObject.x);
    const float playerY = DosToGLCoordinate(playerObject.y);
    Actor* playerActor = new Actor(playerX, playerY, 0, m_decorateActors.at(11));
    playerActor->SetAngle(playerObject.angle);
    playerActor->SetHealth(m_savedGameInDosFormat.GetBody());
    playerActor->SetTile((const uint8_t)playerObject.tilex, (const uint8_t)playerObject.tiley);

    return playerActor;
}

void SavedGameInDosFormatLoader::LoadActors(
    Actor** blockingActors,
    Actor** nonBlockingActors,
    const uint16_t levelWidth,
    const uint16_t levelHeight)
{
    uint16_t nonBlockingIndex = 0;
    for (uint16_t i = 1; i < m_savedGameInDosFormat.GetNumberOfObjects(); i++)
    {
        const SavedGameInDosFormat::ObjectInDosFormat& dosObject = m_savedGameInDosFormat.GetObject(i);
        const uint16_t actorId = m_savedGameConverter.GetActorId(dosObject.obclass, dosObject.state16, dosObject.state32, dosObject.temp1);
        const auto it = m_decorateActors.find(actorId);
        if (it != m_decorateActors.end())
        {
            const DecorateActor& decorateActor = it->second;
            const float x = DosToGLCoordinate(dosObject.x);
            const float y = DosToGLCoordinate(dosObject.y);
            Actor* actor = new Actor(x, y, 0, decorateActor);
            actor->SetTile((uint8_t)dosObject.tilex, (uint8_t)dosObject.tiley);
            actor->SetAngle(dosObject.angle);
            actor->SetActive(dosObject.active != 0);
            actor->SetHealth(dosObject.hitpoints);
            actor->SetTemp1(dosObject.temp1);
            actor->SetTemp2(dosObject.temp2);
            actor->SetState(decorateActor.initialState, 0);
            actor->SetTimeToNextAction(0);
            if (decorateActor.initialState == StateIdProjectileFly || m_savedGameConverter.IsInertObject(dosObject.obclass))
            {
                nonBlockingActors[nonBlockingIndex] = actor;
                nonBlockingIndex++;
            }
            else
            {
                blockingActors[(dosObject.tiley * levelWidth) + dosObject.tilex] = actor;
            }
        }
    }
}

const float SavedGameInDosFormatLoader::DosToGLCoordinate(const int32_t dosCoordinate)
{
    const float dosToGLScaleFactor = 65536.0f;
    return (float)dosCoordinate / dosToGLScaleFactor;
}