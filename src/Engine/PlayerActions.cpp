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

#include "PlayerActions.h"

static const int32_t nukeDelay = 667; // ms
static const int32_t maxBolts = 10;
static const int32_t boltInterval = 100; // ms

PlayerActions::PlayerActions() :
    m_handHeight(0),
    m_shotFiredHandHeight(0),
    m_shotFired(false),
    m_shotFiredTimeStamp(false),
    m_nukeFired(false),
    m_lastNukeTimeStamp(0),
    m_lastBoltTimeStamp(0),
    m_boltsLeft(0),
    m_shotPower(0)
{
    for (uint8_t i = 0; i < MaxControlAction; i++)
    {
        m_controlActionActive[i] = false;
    }
}

PlayerActions::~PlayerActions()
{

}

bool PlayerActions::GetActionActive(const ControlAction action) const
{
    return m_controlActionActive[action];
}

void PlayerActions::SetActionActive(const ControlAction action, const bool active)
{
    m_controlActionActive[action] = active;
}

void PlayerActions::ResetForNewLevel()
{
    m_handHeight = 0;
    m_shotFired = 0;
    m_shotFiredTimeStamp = 0;
    m_nukeFired = false;
    m_lastNukeTimeStamp = 0;
    m_lastBoltTimeStamp = 0;
    m_boltsLeft = 0;
    m_shotPower = 0;
}

uint16_t PlayerActions::GetHandHeight() const
{
    return m_handHeight;
}

bool PlayerActions::UpdateShoot(const uint32_t timeStamp)
{
    bool fireShot = false;
    if (m_controlActionActive[Shoot])
    {
        if (m_shotFired == false)
        {
            fireShot = true;
            m_shotFired = true;
            m_shotFiredHandHeight = m_handHeight;
            m_shotFiredTimeStamp = timeStamp;
        }
    }
    else
    {
        m_shotFired = false;
    }

    const uint16_t deltaTicks = (uint16_t)(((timeStamp - m_shotFiredTimeStamp) * 60) / 1000) / 2;
    if (deltaTicks < 60)
    {
        m_handHeight = m_shotFiredHandHeight + deltaTicks * 4;
        if (m_handHeight > 72)
        {
            m_handHeight = 72;
        }
    }
    else
    {
        if (m_handHeight > 0)
        {
            if ((deltaTicks - 60) * 2 >= 72)
            {
                m_handHeight = 0;
            }
            else
            {
                m_handHeight = 72 - ((deltaTicks - 60) * 2);
            }
        }
    }

    return fireShot;
}

bool PlayerActions::UpdateShootWithCharge(const uint32_t timeStamp)
{
    bool fireShot = false;
    if (m_controlActionActive[Shoot])
    {
        if (m_shotFired == false)
        {
            m_shotFired = true;
            m_shotFiredHandHeight = m_handHeight;
            m_shotFiredTimeStamp = timeStamp;
        }
    }

    const uint16_t deltaTicks = (uint16_t)(((timeStamp - m_shotFiredTimeStamp) * 60) / 1000) / 2;
    if (m_shotFired)
    {
        m_shotPower = (deltaTicks > 56) ? 56 : deltaTicks;
        if (!m_controlActionActive[Shoot])
        {
            fireShot = true;
            m_shotFired = false;
        }
    }
    else
    {
        m_shotPower = 0;
    }

    if (deltaTicks < 60)
    {
        m_handHeight = m_shotFiredHandHeight + deltaTicks * 4;
        if (m_handHeight > 72)
        {
            m_handHeight = 72;
        }
    }
    else
    {
        if (m_handHeight > 0)
        {
            if ((deltaTicks - 60) * 2 >= 72)
            {
                m_handHeight = 0;
            }
            else
            {
                m_handHeight = 72 - ((deltaTicks - 60) * 2);
            }
        }
    }

    return fireShot;
}

bool PlayerActions::IsReadyToShootBolt(const uint32_t timeStamp)
{
    bool fireShot = false;
    if (m_controlActionActive[ShootZappper])
    {
        if (!m_boltFired && m_boltsLeft == 0)
        {
            m_boltFired = true;
            fireShot = true;
        }
    }
    else
    {
        m_boltFired = false;
    }

    return fireShot;
}

void PlayerActions::ShootBolt(const uint32_t timeStamp)
{
    m_boltsLeft = maxBolts;
}

bool PlayerActions::UpdateContinueBolt(const uint32_t timeStamp)
{
    if (m_boltsLeft > 0 && m_lastBoltTimeStamp + boltInterval < timeStamp)
    {
        m_boltsLeft--;
        m_lastBoltTimeStamp = timeStamp;
        return true;
    }

    return false;
}

bool PlayerActions::UpdateShootNuke(const uint32_t timeStamp)
{
    bool fireShot = false;
    if (m_controlActionActive[ShootXterminator])
    {
        if (!m_nukeFired && m_lastNukeTimeStamp + nukeDelay < timeStamp)
        {
            fireShot = true;
            m_nukeFired = true;
            m_lastNukeTimeStamp = timeStamp;
        }
    }
    else
    {
        m_nukeFired = false;
    }

    return fireShot;
}

const char* PlayerActions::GetStatusMessage() const
{
    if (m_shotFired)
    {
        return "Magick Missile";
    }
    else if (m_boltFired)
    {
        return "Zapper";
    }
    else if (m_nukeFired)
    {
        return "Xterminator";
    }
    else if (m_controlActionActive[MoveForward])
    {
        return "Advancing";
    }
    else if (m_controlActionActive[MoveBackward])
    {
        return "Retreating";
    }
    else if (m_controlActionActive[TurnLeft] || m_controlActionActive[TurnRight])
    {
        if (m_controlActionActive[QuickTurn])
        {
            return "Quick Turning";
        }
        else
        {
            return "Turning";
        }
    }
    else if (m_controlActionActive[StrafeLeft] || m_controlActionActive[StrafeRight])
    {
        return "Sidestepping";
    }
    else
    {
        return "Viewing";
    }
}

uint16_t PlayerActions::GetShotPower() const
{
    return m_shotPower;
}
