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

#include "HighScores.h"
#include "Logging.h"
#include <fstream>

static const std::vector<HighScores::HighScore> defaultHighScores =
{
    { "Sir Lancelot",500,0 },
    { "",0,0 },
    { "",0,0 },
    { "",0,0 },
    { "",0,0 },
    { "",0,0 },
    { "",0,0 }
};

HighScores::HighScores() :
    m_highscores(defaultHighScores)
{

}

HighScores::~HighScores()
{

}

const std::vector<HighScores::HighScore>& HighScores::Get() const
{
    return m_highscores;
}

bool HighScores::LoadFromFile(const std::string& path)
{
    struct ConfigHeader
    {
        char signature[4];
        uint16_t version;
    };

    const std::string fileName = "CONFIG.C3D";
    const std::string fullPath = path + "\\" + fileName;

    std::ifstream file;
    file.open(fullPath, std::ifstream::binary);
    if (file.is_open())
    {
        file.seekg(0, file.end);
        const uint32_t actualLength = (uint32_t)file.tellg();
        const uint32_t expectedLength = 474;
        if (actualLength != expectedLength)
        {
            Logging::Instance().AddLogMessage("WARNING: " + fileName + " has the wrong file size (expected: " + std::to_string(expectedLength) + ", actual: " + std::to_string(actualLength) + ")");
            file.close();
            return false;
        }

        file.seekg(0);
        ConfigHeader header;
        file.read((char*)&header, sizeof(header));
        if (file.fail())
        {
            Logging::Instance().AddLogMessage("WARNING: Unable to read header from " + fullPath);
            file.close();
            return false;
        }

        if (header.version != 1)
        {
            Logging::Instance().AddLogMessage("WARNING: " + fileName + " has an unsupported version (expected: 1, found: " + std::to_string(header.version) + ")");
            file.close();
            return false;
        }

        for (uint8_t i = 0; i < 7; i++)
        {
            char name[58];
            file.read(name, 58);
            uint32_t score = 0;
            file.read((char*)&score, sizeof(score));
            uint16_t level = 0;
            file.read((char*)&level, sizeof(level));
            if (file.fail())
            {
                Logging::Instance().AddLogMessage("WARNING: Unable to read high scores from " + fullPath);
                file.close();
                return false;
            }

            m_highscores.at(i).name = std::string(name);
            m_highscores.at(i).score = score;
            m_highscores.at(i).level = level;
        }
        file.read((char*)m_remainingConfigData, 20);
        file.close();
    }
    else
    {
        Logging::Instance().AddLogMessage("WARNING: File not found: " + fullPath);
        return false;
    }

    return true;
}

bool HighScores::StoreToFile(const std::string& path)
{
    const std::string fileName = "CONFIG.C3D";
    const std::string fullPath = path + "\\" + fileName;

    std::ofstream file;
    file.open(fullPath, std::ofstream::binary);
    if (file.is_open())
    {
        const char signature[] = "C3D";
        const uint16_t version = 1;
        file.write(signature, 4);
        file.write((const char*)&version, sizeof(version));
        for (uint8_t i = 0; i < 7; i++)
        {
            char name[58];
            memset(name, 0, 58);
            for (uint8_t n = 0; n < m_highscores.at(i).name.length(); n++)
            {
                name[n] = m_highscores.at(i).name.at(n);
            }
            file.write(name, 58);
            uint32_t score = m_highscores.at(i).score;
            file.write((const char*)&score, sizeof(score));
            uint16_t level = m_highscores.at(i).level;
            file.write((const char*)&level, sizeof(level));
        }
        file.write(m_remainingConfigData, 20);
        file.close();
    }
    else
    {
        return false;
    }

    return true;
}