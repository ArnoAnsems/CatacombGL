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
#include "Logging.h"
#include "StringUtility.h"

namespace
{
    const actorDirection DosDirToDirection(const uint16_t dir)
    {
        return
            (dir == 0u) ? northwest :
            (dir == 1u) ? north :
            (dir == 2u) ? northeast :
            (dir == 3u) ? west :
            (dir == 4u) ? nodir :
            (dir == 5u) ? east :
            (dir == 6u) ? southwest :
            (dir == 7u) ? south :
            (dir == 8u) ? southeast :
            nodir;
    }

};

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
    playerActor->SetAngle(DosToGLAngle(playerObject.angle));
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
        const uint16_t actorId = m_savedGameConverter.GetActorId(dosObject);
        const auto it = m_decorateActors.find(actorId);
        if (it != m_decorateActors.end())
        {
            const DecorateActor& decorateActor = it->second;
            const float x = DosToGLCoordinate(dosObject.x);
            const float y = DosToGLCoordinate(dosObject.y);
            Actor* actor = new Actor(x, y, 0, decorateActor);
            actor->SetTile((uint8_t)dosObject.tilex, (uint8_t)dosObject.tiley);
            actor->SetAngle(DosToGLAngle(dosObject.angle));
            actor->SetActive(dosObject.active != 0);
            actor->SetHealth(dosObject.hitpoints);
            actor->SetTemp1(dosObject.temp1);
            actor->SetTemp2(dosObject.temp2);
            actor->SetDirection(DosDirToDirection(dosObject.dir));
            actor->SetState(m_savedGameConverter.GetDecorateStateId(dosObject), 0);
            actor->SetAnimationFrame(m_savedGameConverter.GetAnimationFrame(dosObject));
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
        else
        {
            const std::string warningMessage =
                "WARNING: Unidentified actor at (" + std::to_string(dosObject.tilex) + "," + std::to_string(dosObject.tiley) + ") "
                "with state16=" + StringUtility::IntToHexadecimal(dosObject.state16) +
                " and state32=" + StringUtility::IntToHexadecimal(dosObject.state32);
            Logging::Instance().AddLogMessage(warningMessage);
        }
    }
}

uint32_t SavedGameInDosFormatLoader::GetPlayerState32() const
{
    const SavedGameInDosFormat::ObjectInDosFormat& playerObject = m_savedGameInDosFormat.GetObject(0);
    return playerObject.state32;
}

constexpr float SavedGameInDosFormatLoader::DosToGLCoordinate(const int32_t dosCoordinate)
{
    const float dosToGLScaleFactor = 65536.0f;
    return (float)dosCoordinate / dosToGLScaleFactor;
}

constexpr float SavedGameInDosFormatLoader::DosToGLAngle(const int16_t dosAngle)
{
    return static_cast<float>((360u + 90u - dosAngle) % 360u);
}
