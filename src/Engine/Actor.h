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
// Actor
//
// An actor can be a monster, bonus item, decorative item, projectile, etc.
//
#pragma once

#include "Decorate.h"

typedef enum {north,east,south,west,northeast,southeast,southwest,northwest,nodir} actorDirection;

class Actor
{
public:
    Actor(const float x, const float y, const uint32_t timestamp, const DecorateActor& decorateActor);
    Actor(std::ifstream& file, const std::map<uint16_t, const DecorateActor>& decorateActors);
    ~Actor();

    const DecorateActor& GetDecorateActorFromFile(std::ifstream& file, const std::map<uint16_t, const DecorateActor>& decorateActors) const;

    float GetX() const;
    void SetX(const float x);
    float GetY() const;
    void SetY(const float y);
    uint8_t GetTileX() const;
    uint8_t GetTileY() const;
    void SetTile(const uint8_t x, const uint8_t y);
    uint16_t GetPictureIndex() const;
    bool IsSolid() const;
    bool WouldCollideWithActor(const float x, const float y, const float size) const;
    actorDirection GetDirection() const;
    void SetDirection(const actorDirection direction);
    float GetDistanceToTarget() const;
    bool TargetReached() const;
    actorAction GetAction() const;
    void Damage(const int16_t points);
    bool IsDead() const;
    bool IsActive() const;
    void SetActive(const bool active);
    void SetHealth(const int16_t health);
    int16_t GetHealth() const;
    void Think(const uint32_t currentTimestamp);

    int16_t GetTemp1() const;
    int16_t GetTemp2() const;
    void SetTemp1(const int16_t temp1);
    void SetTemp2(const int16_t temp2);

    void SetState(const DecorateStateId state, const uint32_t timestamp);
    void SetSolid(const bool solid);
    bool IsActionPerformed() const;
    void SetActionPerformed(const bool performed);
    void SetTimeToNextAction(const uint32_t timeToRise);
    uint32_t GetTimeToNextAction() const;
    float GetAngle() const;
    void SetAngle(const float angle);

    const DecorateActor& GetDecorateActor() const;
    void StoreToFile(std::ofstream& file) const;

protected:
    float m_x;
    float m_y;
    uint8_t m_tileX;
    uint8_t m_tileY;
    uint32_t m_timestamp;
    bool m_solid;
    int16_t m_health;
    int16_t m_temp1, m_temp2;
    actorDirection m_direction;
    bool m_active;
    DecorateStateId m_stateId;
    uint16_t m_animationFrame;
    bool m_actionPerformed;
    uint32_t m_timeToNextAction;
    float m_angle;
    const DecorateActor& m_decorateActor;
};
