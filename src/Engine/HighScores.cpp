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
    m_highscores(defaultHighScores),
    m_remainingConfigData()
{
    m_newScorePosition = (uint8_t)m_highscores.size();
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

bool HighScores::TryToAddNewScore(const uint32_t newScore, const uint16_t newLevel)
{
    uint8_t newPositionInList = 0;
    bool foundPosition = false;

    while (newPositionInList < m_highscores.size() && !foundPosition)
    {
        foundPosition = (newScore > m_highscores.at(newPositionInList).score);
        if (!foundPosition)
        {
            newPositionInList++;
        }
    }

    if (foundPosition)
    {
        // All scores below the new position must be shifted one down
        for (int8_t pos = (int8_t)m_highscores.size() - 1; pos > newPositionInList; pos--)
        {
            m_highscores.at(pos).score = m_highscores.at(pos - 1).score;
            m_highscores.at(pos).level = m_highscores.at(pos - 1).level;
            m_highscores.at(pos).name = m_highscores.at(pos - 1).name;
        }

        // Insert the new score
        m_highscores.at(newPositionInList).score = newScore;
        m_highscores.at(newPositionInList).level = newLevel;
        m_highscores.at(newPositionInList).name = "";
    }

    m_newScorePosition = newPositionInList;
    return foundPosition;
}

void HighScores::AddCharactersToNameOfNewScore(const std::string& characters)
{
    if (m_newScorePosition < m_highscores.size())
    {
        const size_t currentLength = m_highscores.at(m_newScorePosition).name.length();
        if (currentLength + characters.length() < 58)
        {
            m_highscores.at(m_newScorePosition).name += characters;
        }
    }
}

void HighScores::RemoveACharacterFromNameOfNewScore()
{
    if (m_newScorePosition < m_highscores.size())
    {
        const size_t currentLength = m_highscores.at(m_newScorePosition).name.length();
        if (currentLength > 0)
        {
            m_highscores.at(m_newScorePosition).name.erase(currentLength - 1);
        }
    }
}

void HighScores::FinishNameOfNewScore()
{
    m_newScorePosition = static_cast<uint8_t>(m_highscores.size());
}

void HighScores::ApplyEqualSpacingToNumbers(std::string& str)
{
    const uint16_t offsetInFontOfEqualSpacedNumbers = 129;
    for (uint16_t i = 0; i < str.length(); i++)
    {
        str.at(i) += (offsetInFontOfEqualSpacedNumbers - '0');
    }
}

void HighScores::Draw(IRenderer& renderer, EgaGraph& egaGraph, const uint32_t timeStamp, const uint16_t backgroundPic) const
{
    renderer.Render2DPicture(egaGraph.GetPicture(backgroundPic), 0, 0);
    uint16_t y = 68;
    for (uint16_t pos = 0; pos < m_highscores.size(); pos++)
    {
        const auto highScore = m_highscores.at(pos);
        std::string name = highScore.name;
        const bool drawCursor = (pos == m_newScorePosition) && ((timeStamp / 1000) % 2 == 0);
        if (drawCursor)
        {
            name += "|";
        }
        renderer.RenderTextLeftAligned(name.c_str(), egaGraph.GetFont(3), EgaBlue, 60, y);
        std::string levelStr = std::to_string(highScore.level);
        ApplyEqualSpacingToNumbers(levelStr);
        renderer.RenderTextLeftAligned(levelStr.c_str(), egaGraph.GetFont(3), EgaBlue, 192 - (8 * (uint16_t)levelStr.length()), y);
        std::string scoreStr = std::to_string(highScore.score);
        ApplyEqualSpacingToNumbers(scoreStr);
        renderer.RenderTextLeftAligned(scoreStr.c_str(), egaGraph.GetFont(3), EgaBlue, 264 - (8 * (uint16_t)scoreStr.length()), y);
        y += 16;
    }
}