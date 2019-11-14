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
        file.close();
    }
    else
    {
        Logging::Instance().AddLogMessage("WARNING: File not found: " + fullPath);
        return false;
    }

    return true;
}