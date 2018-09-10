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

#include "FramesCounter.h"

FramesCounter::FramesCounter() :
    m_framesPerSecond(0),
    m_startTimestamp(0),
    m_frameCount(0)
{
}

FramesCounter::~FramesCounter()
{

}

void FramesCounter::AddFrame(const uint32_t timeStamp)
{
    if (timeStamp >= m_startTimestamp + 1000)
    {
        if (m_startTimestamp != 0)
        {
            const uint32_t deltaTime = (uint32_t)(timeStamp - m_startTimestamp);
            m_framesPerSecond = (m_frameCount * 1000) / deltaTime;
        }
        
        m_frameCount = 1;
        m_startTimestamp = timeStamp;
    }
    else
    {
        m_frameCount++;
    }
}

uint32_t FramesCounter::GetFramesPerSecond() const
{
    return m_framesPerSecond;
}