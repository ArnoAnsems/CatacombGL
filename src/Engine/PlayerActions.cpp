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
    m_autoFireTimeStamp(0),
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
    m_autoFireTimeStamp = 0;
    m_boltsLeft = 0;
    m_shotPower = 0;
}

uint16_t PlayerActions::GetHandHeight() const
{
    return m_handHeight;
}

bool PlayerActions::UpdateShoot(const uint32_t timeStamp, const bool autoFire, ManaBar& manaBar)
{
    bool fireShot = false;
    m_shotPower = 0;
    if (m_controlActionActive[Shoot])
    {
        // Fire button is pressed
        if (m_shotFired == false && manaBar.FireShot())
        {
            // No shot fired yet, so fire and register the time.
            fireShot = true;
            m_shotFired = true;
            m_shotFiredHandHeight = m_handHeight;
            m_shotFiredTimeStamp = timeStamp;
        }
        else if (autoFire && m_shotFiredTimeStamp + 250 < timeStamp)
        {
            // Autofire is enabled and sufficient time has passed since the previous shot,
            // so allow for a new shot to be fired.
            m_shotFired = false;
        }

        // While the fire button is in a pressed state, keep raising the hand.
        const uint16_t deltaTicksSinceFire = (uint16_t)(((timeStamp - m_shotFiredTimeStamp) * 70) / 1000) / 2;
        m_handHeight = m_shotFiredHandHeight + (deltaTicksSinceFire + 2) * 6;
        if (m_handHeight > 72)
        {
            m_handHeight = 72;
        }
    }
    else
    {
        // Fire button is released
        if (m_shotFired)
        {
            // Fire button was just released, so register time of release.
            m_shotFiredTimeStamp = timeStamp;
            m_shotFired = false;
        }

        const uint16_t deltaTicksSinceRelease = (uint16_t)(((timeStamp - m_shotFiredTimeStamp) * 70) / 1000) / 2;
        if (deltaTicksSinceRelease < 60)
        {
            // Fire button was released less than 60 ticks ago. Keep hand height as-is.
            m_shotFiredHandHeight = m_handHeight;
        }
        else
        {
            // Fire button was released more than 60 ticks ago.
            if (m_handHeight > 0)
            {
                // Hand is still up. Lower hand.
                const uint16_t distanceToLower = (deltaTicksSinceRelease - 60) * 2;
                m_handHeight = (distanceToLower < m_shotFiredHandHeight) ? m_shotFiredHandHeight - distanceToLower : 0;
            }
        }
    }

    return fireShot;
}

bool PlayerActions::UpdateShootWithCharge(const uint32_t timeStamp, const bool autoFire, ManaBar& manaBar)
{
    bool fireShot = false;
    if (m_controlActionActive[Shoot])
    {
        // Fire button is pressed
        if (m_shotFired == false)
        {
            // Fire button was just pressed, so register the time.
            m_shotFired = true;
            m_shotFiredHandHeight = m_handHeight;
            m_shotFiredTimeStamp = timeStamp;
        }

        if (autoFire && m_autoFireTimeStamp + 250 < timeStamp && manaBar.FireShot())
        {
            fireShot = true;
            m_autoFireTimeStamp = timeStamp;
        }

        // While the fire button is in a pressed state, keep raising the hand.
        const uint16_t deltaTicksSincePress = (uint16_t)(((timeStamp - m_shotFiredTimeStamp) * 70) / 1000) / 2;
        m_shotPower = (deltaTicksSincePress > 56) ? 56 : deltaTicksSincePress;

        m_handHeight = m_shotFiredHandHeight + (deltaTicksSincePress + 2) * 6;
        if (m_handHeight > 72)
        {
            m_handHeight = 72;
        }
    }
    else
    {
        // Fire button is released
        if (m_shotFired)
        {
            // Fire button was just released
            m_shotFired = false;
            m_shotFiredTimeStamp = timeStamp;
            m_shotFiredHandHeight = m_handHeight;

            if ((!autoFire || m_shotPower == 56) && manaBar.FireShot())
            {
                fireShot = true;
            }
        }
        else
        {
            m_shotPower = 0;

            const uint16_t deltaTicksSinceRelease = (uint16_t)(((timeStamp - m_shotFiredTimeStamp) * 70) / 1000) / 2;
            if (deltaTicksSinceRelease < 60)
            {
                // Fire button was pressed less than 60 ticks ago. Keep hand height as-is.
            }
            else
            {
                // Fire button was released more than 60 ticks ago.
                if (m_handHeight > 0)
                {
                    // Hand is still up. Lower hand.
                    const uint16_t distanceToLower = (deltaTicksSinceRelease - 60) * 2;
                    m_handHeight = (distanceToLower < m_shotFiredHandHeight) ? m_shotFiredHandHeight - distanceToLower : 0;
                }
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
