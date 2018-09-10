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

#include "Actor.h"
#include "Math.h"
#include <fstream>

Actor::Actor(const float x, const float y, const uint32_t timestamp, const DecorateActor& decorateActor) :
    m_x(x),
    m_y(y),
    m_tileX(uint8_t(x)),
    m_tileY(uint8_t(y)),
    m_timestamp(timestamp),
    m_solid(false),
    m_direction(nodir),
    m_active(false),
    m_stateId(StateIdHidden),
    m_animationFrame(0),
    m_timeToNextAction(0),
    m_decorateActor(decorateActor),
    m_health(decorateActor.initialHealth),
    m_angle(0.0f),
    m_temp1(0),
    m_temp2(0),
    m_actionPerformed(false)

{
    SetState(decorateActor.initialState, m_timestamp);
    m_solid = (m_stateId != StateIdHidden && m_stateId != StateIdWaitForPickup);
}

Actor::Actor(std::ifstream& file, const std::map<uint16_t, const DecorateActor>& decorateActors) :
    m_decorateActor(GetDecorateActorFromFile(file, decorateActors))
{
    file.read((char*)&m_x, sizeof(m_x));
    file.read((char*)&m_y, sizeof(m_y));
    file.read((char*)&m_tileX, sizeof(m_tileX));
    file.read((char*)&m_tileY, sizeof(m_tileY));
    file.read((char*)&m_timestamp, sizeof(m_timestamp));
    file.read((char*)&m_solid, sizeof(m_solid));
    file.read((char*)&m_health, sizeof(m_health));
    file.read((char*)&m_temp1, sizeof(m_temp1));
    file.read((char*)&m_temp2, sizeof(m_temp2));
    uint8_t direction = 0;
    file.read((char*)&direction, sizeof(direction));
    m_direction = (actorDirection)direction;
    file.read((char*)&m_active, sizeof(m_active));
    uint8_t stateId = 0;
    file.read((char*)&stateId, sizeof(stateId));
    m_stateId = (DecorateStateId)stateId;
    file.read((char*)&m_animationFrame, sizeof(m_animationFrame));
    file.read((char*)&m_actionPerformed, sizeof(m_actionPerformed));
    file.read((char*)&m_timeToNextAction, sizeof(m_timeToNextAction));
    file.read((char*)&m_angle, sizeof(m_angle));
}

Actor::~Actor()
{

}

const DecorateActor& Actor::GetDecorateActorFromFile(std::ifstream& file, const std::map<uint16_t, const DecorateActor>& decorateActors) const
{
    uint16_t actorId = 0;
    file.read((char*)&actorId, sizeof(actorId));
    const auto idToActorPair = decorateActors.find(actorId);
    return (idToActorPair != decorateActors.end()) ? idToActorPair->second : decorateActors.begin()->second;
}

float Actor::GetX() const
{
    return m_x;
}

void Actor::SetX(const float x)
{
    m_x = x;
}

float Actor::GetY() const
{
    return m_y;
}

void Actor::SetY(const float y)
{
    m_y = y;
}

uint8_t Actor::GetTileX() const
{
    return m_tileX;
}

uint8_t Actor::GetTileY() const
{
    return m_tileY;
}

void Actor::SetTile(const uint8_t x, const uint8_t y)
{
    m_tileX = x;
    m_tileY = y;
}

float Actor::GetDistanceToTarget() const
{
    const float distanceX = abs(m_x - float(m_tileX) - 0.5f);
    const float distanceY = abs(m_y - float(m_tileY) - 0.5f);
    return (distanceX > distanceY) ? distanceX : distanceY;
}

bool Actor::TargetReached() const
{
    return GetDistanceToTarget() < 0.1f;
}

uint16_t Actor::GetPictureIndex() const
{

    const DecorateState& currentState = m_decorateActor.states.find(m_stateId)->second;
    return currentState.animation.at(m_animationFrame).pictureIndex;
}

void Actor::Think(const uint32_t currentTimestamp)
{
    const uint16_t deltaTicks = (uint16_t)(((currentTimestamp - m_timestamp) * 60) / 1000);
    const DecorateState& currentState = m_decorateActor.states.find(m_stateId)->second;
    const DecorateAnimationFrame& currentFrame = currentState.animation.at(m_animationFrame);
    if (deltaTicks > currentFrame.durationInTics)
    {
        // Goto next frame
        m_animationFrame++;
        m_timestamp = currentTimestamp;
        m_actionPerformed = false;
        if (currentState.animation.size() <= m_animationFrame)
        {
            // We're at the end of an animation; go to the first frame of the next state
            m_stateId = currentState.nextState;
            m_animationFrame = 0;
        }
    }
}

bool Actor::IsSolid() const
{
    return m_solid;
}

actorAction Actor::GetAction() const
{
    const DecorateState& currentState = m_decorateActor.states.find(m_stateId)->second;
    return currentState.animation.at(m_animationFrame).action;
}

bool Actor::WouldCollideWithActor(const float x, const float y, const float size) const
{
    return (m_solid && (abs(x - m_x) < size + m_decorateActor.size) && (abs(y - m_y) < size + m_decorateActor.size));
}

actorDirection Actor::GetDirection() const
{
    return m_direction;
}

void Actor::SetDirection(const actorDirection direction)
{
    m_direction = direction;
}

void Actor::Damage(const int16_t points)
{
    if (m_decorateActor.states.find(StateIdDying) != m_decorateActor.states.end())
    {
        m_health -= points;
        if (m_health < 0)
        {
            m_health = 0;
        }
    }
}

bool Actor::IsDead() const
{
    return (m_health <= 0);
}

bool Actor::IsActive() const
{
    return m_active;
}

void Actor::SetActive(const bool active)
{
    m_active = active;
}

int16_t Actor::GetTemp1() const
{
    return m_temp1;
}

int16_t Actor::GetTemp2() const
{
    return m_temp2;
}

void Actor::SetHealth(const int16_t health)
{
    m_health = health;
}

int16_t Actor::GetHealth() const
{
    return m_health;
}

const DecorateActor& Actor::GetDecorateActor() const
{
    return m_decorateActor;
}

void Actor::SetState(const DecorateStateId state, const uint32_t timestamp)
{
    if (m_decorateActor.states.find(state) != m_decorateActor.states.end())
    {
        m_stateId = state;
        m_timestamp = timestamp;
        m_animationFrame = 0;
        m_actionPerformed = false;
    }
}

void Actor::SetSolid(const bool solid)
{
    m_solid = solid;
}

bool Actor::IsActionPerformed() const
{
    return m_actionPerformed;
}

void Actor::SetActionPerformed(const bool performed)
{
    m_actionPerformed = performed;
}

void Actor::SetTimeToNextAction(const uint32_t timeToRise)
{
    m_timeToNextAction = timeToRise;
}

uint32_t Actor::GetTimeToNextAction() const
{
    return m_timeToNextAction;
}

void Actor::SetTemp1(const int16_t temp1)
{
    m_temp1 = temp1;
}

void Actor::SetTemp2(const int16_t temp2)
{
    m_temp2 = temp2;
}

float Actor::GetAngle() const
{
    return m_angle;
}

void Actor::SetAngle(const float angle)
{
    m_angle = angle;
    if (m_angle > 360.0f)
    {
        m_angle -= 360.0f;
    }
    else if (m_angle < 0.0f)
    {
        m_angle += 360.0f;
    }
}

void Actor::StoreToFile(std::ofstream& file) const
{
    const uint16_t id = m_decorateActor.id;
    file.write((const char*)&id, sizeof(id));
    file.write((const char*)&m_x, sizeof(m_x));
    file.write((const char*)&m_y, sizeof(m_y));
    file.write((const char*)&m_tileX, sizeof(m_tileX));
    file.write((const char*)&m_tileY, sizeof(m_tileY));
    file.write((const char*)&m_timestamp, sizeof(m_timestamp));
    file.write((const char*)&m_solid, sizeof(m_solid));
    file.write((const char*)&m_health, sizeof(m_health));
    file.write((const char*)&m_temp1, sizeof(m_temp1));
    file.write((const char*)&m_temp2, sizeof(m_temp2));
    uint8_t direction = (uint8_t)m_direction;
    file.write((const char*)&direction, sizeof(direction));
    file.write((const char*)&m_active, sizeof(m_active));
    uint8_t stateId = (uint8_t)m_stateId;
    file.write((const char*)&stateId, sizeof(stateId));
    file.write((const char*)&m_animationFrame, sizeof(m_animationFrame));
    file.write((const char*)&m_actionPerformed, sizeof(m_actionPerformed));
    file.write((const char*)&m_timeToNextAction, sizeof(m_timeToNextAction));
    file.write((const char*)&m_angle, sizeof(m_angle));
}