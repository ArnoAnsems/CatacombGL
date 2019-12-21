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
// GameTimer
//
// Keeps track of the time. Used for animations, movement, etc.
//
#pragma once

#include <stdint.h>
#include <fstream>

class GameTimer
{
public:
    GameTimer();
    ~GameTimer();

    uint32_t GetMillisecondsForPlayer() const;
    uint32_t GetTicksForPlayer() const;
    uint32_t GetMilliSecondsForWorld();
    uint32_t GetTicksForWorld();
    uint32_t GetActualTime();

    void Reset();
    void Pause();
    void Resume();
    void FreezeTime();
    uint32_t GetRemainingFreezeTime();

    void StoreToFile(std::ofstream& file) const;
    bool LoadFromFile(std::ifstream& file);

private:
    static uint32_t GetCurrentTime();
    static constexpr uint32_t GetStandardFreezePeriod();

    int64_t m_startTimeForPlayer;
    uint32_t m_pauseTime;
    uint32_t m_freezeStartTime;
    uint32_t m_totalFrozenTime;
    bool m_paused;
};
