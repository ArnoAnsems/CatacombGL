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
#include "Logging.h"
#include <fstream>
#include <iostream>
#include <SDL_messagebox.h>

namespace fs = std::filesystem;

static Logging* m_instance = nullptr;

Logging& Logging::Instance()
{
    if (m_instance == nullptr)
    {
        m_instance = new Logging();
    }

    return *m_instance;
}

Logging::Logging() :
    m_traceFileName("")
{
    m_allLogMessages.clear();
}

Logging::~Logging()
{
    m_allLogMessages.clear();
}

void Logging::SetLogFile(const fs::path traceFileName)
{
    m_traceFileName = traceFileName;

    std::ofstream file;
    file.open(m_traceFileName, std::ofstream::out | std::ofstream::trunc);
    if (file.is_open())
    {
        file << "=== CatacombGL Log file ===\n";
        file.close();
    }
}

void Logging::AddLogMessage(const std::string& logline)
{
    m_allLogMessages.push_back(logline);
    std::ofstream file;
    file.open(m_traceFileName, std::ofstream::out | std::ios_base::app);
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

void Logging::FatalError(const std::string& message)
{
    AddLogMessage("FATAL ERROR: " + message);
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
        "CatacombGL",
        message.c_str(),
        nullptr);
    exit(1);
}
