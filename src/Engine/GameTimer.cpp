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

#include "GameTimer.h"
#include <SDL_timer.h>
#include <fstream>

GameTimer::GameTimer()
{
    m_paused = true;
    uint32_t currentTime = GetCurrentTime();
    m_startTimeForPlayer = currentTime;
    m_pauseTime = currentTime;
    m_freezeStartTime = 0;
    m_totalFrozenTime = 0;
}

GameTimer::~GameTimer()
{

}

uint32_t GameTimer::GetMillisecondsForPlayer() const
{
    const uint32_t currentTime = m_paused ? m_pauseTime : (uint32_t)(GetCurrentTime() - m_startTimeForPlayer);
    return currentTime;
}

uint32_t GameTimer::GetTicksForPlayer() const
{
    return GetMillisecondsForPlayer() / 14;
}

uint32_t GameTimer::GetMilliSecondsForWorld()
{
    const uint32_t deltaMilliSeconds = GetMillisecondsForPlayer();
    if (m_freezeStartTime != 0)
    {
        if (m_freezeStartTime + GetStandardFreezePeriod() > deltaMilliSeconds)
        {
            return (uint32_t)(m_freezeStartTime - m_totalFrozenTime);
        }
        else
        {
            m_totalFrozenTime += (deltaMilliSeconds - m_freezeStartTime);
            m_freezeStartTime = 0;
        }
    }

    return (uint32_t)(deltaMilliSeconds - m_totalFrozenTime);
}

uint32_t GameTimer::GetTicksForWorld()
{
     return GetMilliSecondsForWorld() / 14;
}

void GameTimer::Reset()
{
    uint32_t currentTime = GetCurrentTime();
    m_startTimeForPlayer = currentTime;
    m_pauseTime = currentTime;
    m_freezeStartTime = 0;
    m_totalFrozenTime = 0;
    m_paused = false;
}

void GameTimer::Pause()
{
    if (m_paused)
    {
        return;
    }

    m_paused = true;
    m_pauseTime = (uint32_t)(GetCurrentTime() - m_startTimeForPlayer);
}

bool GameTimer::IsPaused() const
{
    return m_paused;
}

void GameTimer::Resume()
{
    if (m_paused)
    {
        m_paused = false;
        const uint32_t currentTime = (uint32_t)(GetCurrentTime() - m_startTimeForPlayer);
        uint32_t pausedMilliseconds = currentTime - m_pauseTime;
        m_startTimeForPlayer += pausedMilliseconds;
    }
}

void GameTimer::FreezeTime()
{
    const uint32_t playerTime = GetMillisecondsForPlayer();
    if (m_freezeStartTime != 0)
    {
        m_totalFrozenTime += (playerTime - m_freezeStartTime);
    }
    m_freezeStartTime = playerTime;
}

uint32_t GameTimer::GetCurrentTime()
{
    return SDL_GetTicks();
}

uint32_t GameTimer::GetRemainingFreezeTime()
{
    if (m_freezeStartTime == 0)
    {
        return 0;
    }

    int64_t delta = m_freezeStartTime + GetStandardFreezePeriod() - GetMillisecondsForPlayer();
    if (delta < 0)
    {
        return 0;
    }
    else
    {
        return (uint32_t)delta;
    }
}

uint32_t GameTimer::GetActualTime()
{
    return GetCurrentTime();
}

void GameTimer::StoreToFile(std::ofstream& file) const
{
    const uint32_t playerTime = GetMillisecondsForPlayer();
    file.write((const char*)&playerTime, sizeof(playerTime));
    file.write((const char*)&m_freezeStartTime, sizeof(m_freezeStartTime));
    file.write((const char*)&m_totalFrozenTime, sizeof(m_totalFrozenTime));
}

bool GameTimer::LoadFromFile(std::ifstream& file)
{
    uint32_t playerTime = 0;
    file.read((char*)&playerTime, sizeof(playerTime));
    file.read((char*)&m_freezeStartTime, sizeof(m_freezeStartTime));
    file.read((char*)&m_totalFrozenTime, sizeof(m_totalFrozenTime));

    m_startTimeForPlayer = GetCurrentTime() - playerTime;

    m_pauseTime = 0;
    m_paused = false;

    return true;
}

constexpr uint32_t GameTimer::GetStandardFreezePeriod()
{
    return ((3000 * 1000) / 70);
}
