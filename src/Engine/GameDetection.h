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
// GameDetections
//
// Determines whether a specific game is located in a specific folder, based on
// a list of known filenames and filesizes.
//
#pragma once

#include <stdint.h>
#include <string>
#include <map>

struct DetectionReport
{
    uint8_t gameId;
    std::string folder;
    uint16_t score;
    std::string infoString;
};

class GameDetection
{
public:
    GameDetection();

    const DetectionReport& GetBestMatch() const;
    const DetectionReport& GetDetectionReport(const uint8_t gameId, const std::string& folder, const std::map<std::string, uint32_t>& files);

private:
    DetectionReport m_latestReport;
    DetectionReport m_bestMatch;
};