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
// FramesCounter
//
// Keeps track of the amount of frames rendered each second.
//
#pragma once
#include <stdint.h>

class FramesCounter
{
public:
    FramesCounter();
    ~FramesCounter();

    void AddFrame(const uint32_t timeStamp);
    uint32_t GetFramesPerSecond() const;

private:
   uint32_t m_startTimestamp;
   uint32_t m_frameCount;
   uint32_t m_framesPerSecond;
};
