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

#include "CommandLineParser_Test.h"
#include "../Engine/CommandLineParser.h"
#include "../Engine/GameId.h"

TEST(CommandLineParser_Test, parseWithNoCommands)
{
    CommandLineParser commandLineParser;
    char* commands[] = { "test.exe" };
    commandLineParser.parse(1, commands);
    EXPECT_EQ(GameId::NotDetected, commandLineParser.gameIdToStart());
    EXPECT_TRUE(commandLineParser.getFilenameIni().empty());
    EXPECT_TRUE(commandLineParser.getFilenameLog().empty());
}

TEST(CommandLineParser_Test, startWithCatacomb3D)
{
    CommandLineParser commandLineParser;
    char* commands[] = {"test.exe", "--descent"};
    commandLineParser.parse(2, commands);
    EXPECT_EQ(GameId::Catacomb3Dv122, commandLineParser.gameIdToStart());
}

TEST(CommandLineParser_Test, startWithCatacombAbyssSharewareV113)
{
    CommandLineParser commandLineParser;
    char* commands[] = { "test.exe", "--abyss_sw13" };
    commandLineParser.parse(2, commands);
    EXPECT_EQ(GameId::CatacombAbyssv113, commandLineParser.gameIdToStart());
}

TEST(CommandLineParser_Test, startWithCatacombAbyssRegistered)
{
    CommandLineParser commandLineParser;
    char* commands[] = { "test.exe", "--abyss" };
    commandLineParser.parse(2, commands);
    EXPECT_EQ(GameId::CatacombAbyssv124, commandLineParser.gameIdToStart());
}

TEST(CommandLineParser_Test, startWithCatacombArmageddon)
{
    CommandLineParser commandLineParser;
    char* commands[] = { "test.exe", "--armageddon" };
    commandLineParser.parse(2, commands);
    EXPECT_EQ(GameId::CatacombArmageddonv102, commandLineParser.gameIdToStart());
}

TEST(CommandLineParser_Test, startWithCatacombApocalypse)
{
    CommandLineParser commandLineParser;
    char* commands[] = { "test.exe", "--apocalypse" };
    commandLineParser.parse(2, commands);
    EXPECT_EQ(GameId::CatacombApocalypsev101, commandLineParser.gameIdToStart());
}

TEST(CommandLineParser_Test, iniWithoutFilename)
{
    CommandLineParser commandLineParser;
    char* commands[] = { "test.exe", "--ini" };
    commandLineParser.parse(2, commands);
    EXPECT_TRUE(commandLineParser.getFilenameIni().empty());
}

TEST(CommandLineParser_Test, iniWithFilename)
{
    CommandLineParser commandLineParser;
    char* commands[] = { "test.exe", "--ini", "ini.txt" };
    commandLineParser.parse(3, commands);
    EXPECT_EQ("ini.txt", commandLineParser.getFilenameIni());
}

TEST(CommandLineParser_Test, logWithoutFilename)
{
    CommandLineParser commandLineParser;
    char* commands[] = { "test.exe", "--log" };
    commandLineParser.parse(2, commands);
    EXPECT_TRUE(commandLineParser.getFilenameLog().empty());
}

TEST(CommandLineParser_Test, logWithFilename)
{
    CommandLineParser commandLineParser;
    char* commands[] = { "test.exe", "--log", "log.txt" };
    commandLineParser.parse(3, commands);
    EXPECT_EQ("log.txt", commandLineParser.getFilenameLog());
}

TEST(CommandLineParser_Test, useCapitals)
{
    CommandLineParser commandLineParser;
    char* commands[] = { "test.exe", "--ABysS" };
    commandLineParser.parse(2, commands);
    EXPECT_EQ(GameId::CatacombAbyssv124, commandLineParser.gameIdToStart());
}