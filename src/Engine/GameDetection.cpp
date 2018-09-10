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

#include "GameDetection.h"
#include <fstream>

GameDetection::GameDetection()
{
    m_latestReport.gameId = 0;
    m_latestReport.folder.clear();
    m_latestReport.score = UINT16_MAX;
    m_latestReport.infoString.clear();
    m_bestMatch.gameId = 0;
    m_bestMatch.folder.clear();
    m_bestMatch.score = UINT16_MAX;
    m_bestMatch.infoString.clear();
}

const DetectionReport& GameDetection::GetBestMatch() const
{
    return m_bestMatch;
}

const DetectionReport& GameDetection::GetDetectionReport(const uint8_t gameId, const std::string& folder, const std::map<std::string, uint32_t>& files)
{
    m_latestReport.gameId = gameId;
    m_latestReport.folder = folder;
    m_latestReport.score = 0;
    m_latestReport.infoString.clear();

    for(auto pair : files)
    {
        const std::string fullPath = folder + pair.first;
        std::ifstream file;
        file.open(fullPath);
        if (!file.is_open())
        {
            m_latestReport.score +=2;
            m_latestReport.infoString += "File not found: " + pair.first + "; ";
        }
        else
        {
            file.seekg (0, file.end);
            const uint32_t actualLength = (uint32_t)file.tellg();
            file.close();
            if (actualLength != pair.second)
            {
                m_latestReport.score += 1;
                m_latestReport.infoString += "File " + pair.first + " has incorrect length (Expected: " + std::to_string(pair.second) + ", Actual: " + std::to_string(actualLength) + "); ";
            }
        }
    }
    
    if (m_latestReport.score < m_bestMatch.score)
    {
        m_bestMatch.folder = m_latestReport.folder;
        m_bestMatch.gameId = m_latestReport.gameId;
        m_bestMatch.infoString = m_latestReport.infoString;
        m_bestMatch.score = m_latestReport.score;
    }

    return m_latestReport;
}