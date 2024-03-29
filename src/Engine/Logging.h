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

#include <filesystem>
#include <string>
#include <vector>

class Logging
{
public:
    static Logging& Instance();

    void AddLogMessage(const std::string& logline);
    const std::vector<std::string>& GetAllLogMessages() const;
    void FatalError(const std::string& message);
    void SetLogFile(const std::filesystem::path traceFileName);

private:
    Logging();
    ~Logging();

    std::vector<std::string> m_allLogMessages;
    std::filesystem::path m_traceFileName;
};
