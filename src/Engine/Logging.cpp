// Copyright (C) 2019 Arno Ansems
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
#pragma once

#include "Logging.h"
#include <fstream>
#include <iostream>

Logging::Logging(const std::string traceFileName) :
    m_traceFileName(traceFileName)
{
    m_allLogMessages.clear();

    std::ofstream file;
    file.open(m_traceFileName);
    if (file.is_open())
    {
        file << "=== CATACOMBGL Log file ===\n";
        file.close();
    }
}

Logging::~Logging()
{
    m_allLogMessages.clear();
}

void Logging::AddLogMessage(const std::string& logline)
{
    m_allLogMessages.push_back(logline);
    std::ofstream file;
    file.open(m_traceFileName, std::ios_base::app);
    if (file.is_open())
    {
        file << logline << "\n";
        file.close();
    }
}

const std::vector<std::string>& Logging::GetAllLogMessages() const
{
    return m_allLogMessages;
}