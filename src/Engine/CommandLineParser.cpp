// Copyright (C) 2024 Arno Ansems
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

#include "CommandLineParser.h"
#include "GameId.h"

const std::string paramDescent = "--descent";
const std::string paramAbyss = "--abyss";
const std::string paramArmageddon = "--armageddon";
const std::string paramApocalypse = "--apocalypse";
const std::string paramAbyssSW13 = "--abyss_sw13";

const std::string paramIniFile = "--ini";
const std::string paramLogFile = "--log";

CommandLineParser::CommandLineParser() :
    m_gameIdToStart(GameID::NotDetected)
{
}

bool CommandLineParser::compareStrings(const std::string& str1, const std::string& str2)
{
    if (str1.length() != str2.length())
    {
        return false;
    }

    for (int i = 0; i < str1.length(); ++i)
    {
        if (tolower(str1[i]) != tolower(str2[i]))
        {
            return false;
        }
    }

    return true;
}

void CommandLineParser::parse(const int argc, char* argv[])
{
    for (int i = 1; i < argc; i++)
    {
        std::string prev = argv[i - 1];
        std::string current = argv[i];

        if (compareStrings(current, paramDescent))
        {
            m_gameIdToStart = GameID::Catacomb3Dv122;
        }
        if (compareStrings(current, paramAbyss))
        {
            m_gameIdToStart = GameID::CatacombAbyssv124;
        }
        if (compareStrings(current, paramApocalypse))
        {
            m_gameIdToStart = GameID::CatacombApocalypsev101;
        }
        if (compareStrings(current, paramArmageddon))
        {
            m_gameIdToStart = GameID::CatacombArmageddonv102;
        }
        if (compareStrings(current, paramAbyssSW13))
        {
            m_gameIdToStart = GameID::CatacombAbyssv113;
        }

        if (compareStrings(prev, paramIniFile))
        {
            m_filenameIni = current;
        }

        if (compareStrings(prev, paramLogFile))
        {
            m_filenameLog = current;
        }
    };
}

const std::string& CommandLineParser::getFilenameIni() const
{
    return m_filenameIni;
}

const std::string& CommandLineParser::getFilenameLog() const
{
    return m_filenameLog;
}

uint8_t CommandLineParser::gameIdToStart() const
{
    return m_gameIdToStart;
}