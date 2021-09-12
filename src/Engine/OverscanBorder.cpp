// Copyright (C) 2021 Arno Ansems
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

#include "OverscanBorder.h"

OverscanBorder::OverscanBorder() :
    m_color(EgaBlack),
    m_timeStampWhenColorIsExpired(0)
{

}

OverscanBorder::~OverscanBorder()
{

}

void OverscanBorder::SetColor(const uint32_t timeStamp, const uint8_t egaSignal, const uint32_t durationInMs)
{
    m_color = ConvertEgaSignalToEgaColor(egaSignal);
    m_timeStampWhenColorIsExpired = timeStamp + durationInMs;
}

egaColor OverscanBorder::GetActiveColor(const uint32_t timeStamp) const
{
    return (timeStamp > m_timeStampWhenColorIsExpired) ? EgaBlack : m_color;
}

const uint16_t OverscanBorder::GetBorderWidth()
{
    return 4u;
}

const uint16_t OverscanBorder::GetBorderHeight()
{
    return 3u;
}

// This function is taken from BEL_ST_ConvertEGASignalToEGAEntry in Reflection Keen.
// It gets a value representing a 6 bit EGA signal and converts it into the 
// regular "Blue Green Red Intensity" 4 bit format.
egaColor OverscanBorder::ConvertEgaSignalToEgaColor(const uint8_t egaSignal)
{
    return (egaColor)((egaSignal & 7) | ((egaSignal & 16) >> 1));
}