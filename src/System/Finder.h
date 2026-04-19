// Copyright (C) 2022 Arno Ansems
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

#include "../Engine/GameDetection.h"
#include "../Engine/GameId.h"
#include <stdint.h>
#include <string>
#include <map>
#include <filesystem>

class ConfigurationSettings;

class Finder
{
public:
    explicit Finder(const ConfigurationSettings& config);

    void FindGamesInWorkingPath();
    void FindInPath(const std::filesystem::path& path);
    void FindGOGPack();
    void LoadPaths(const ConfigurationSettings& config);
    void SafePaths(ConfigurationSettings& config);

    const GameDetection GetGameDetector(GameId Id) const;
    const DetectionReport GetGameReport(GameId Id) const;
    const std::string GetGameName(GameId Id) const;
    uint16_t GetGameScore(GameId Id) const;
    const std::filesystem::path GetGameFolder(GameId Id) const;

private:
    std::map<GameId, GameDetection> m_detector;

};

