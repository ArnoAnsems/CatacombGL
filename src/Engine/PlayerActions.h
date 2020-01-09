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
// PlayerActions
//
// Orchestrates the player actions (walking, firing, etc), based on time and keyboard/mouse input.
//
#pragma once

#include <stdint.h>
#include "ControlsMap.h"

class PlayerActions
{
public:
    PlayerActions();
    ~PlayerActions();

    uint16_t GetHandHeight() const;
    bool UpdateShoot(const uint32_t timeStamp, const bool autoFire);
    bool UpdateShootWithCharge(const uint32_t timeStamp);
    bool IsReadyToShootBolt(const uint32_t timeStamp);
    void ShootBolt(const uint32_t timeStamp);
    bool UpdateContinueBolt(const uint32_t timeStamp);
    bool UpdateShootNuke(const uint32_t timeStamp);

    const char* GetStatusMessage() const;

    void ResetForNewLevel();

    bool GetActionActive(const ControlAction action) const;
    void SetActionActive(const ControlAction action, const bool active);

    uint16_t GetShotPower() const;

private:
    uint16_t m_handHeight;
    uint16_t m_shotFiredHandHeight;
    uint32_t m_shotFiredTimeStamp;
    bool m_shotFired;
    bool m_boltFired;
    bool m_nukeFired;
    uint32_t m_lastNukeTimeStamp;
    uint32_t m_lastBoltTimeStamp;

    int16_t m_boltsLeft;
    uint16_t m_shotPower;

    bool m_controlActionActive[MaxControlAction];
};
