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

#include "../Engine/ConfigurationSettings.h"
#include "../Engine/GameDetection.h"

#include "../Abyss/GameDetectionAbyss.h"
#include "../Armageddon/GameDetectionArmageddon.h"
#include "../Apocalypse/GameDetectionApocalypse.h"
#include "../Catacomb3D/GameDetectionCatacomb3D.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <map>
#include <array>
#include <vector>
#include <filesystem>

// Order matters! Please do not reorder 
namespace GameID
{
    enum
    {
        CatacombAbyssv113 = 0,
        CatacombAbyssv124,
        CatacombArmageddonv102,
        CatacombApocalypsev101,
        Catacomb3Dv122,
    
        Count,
        NotDetected
    };
};

class Finder
{
public:
    Finder() = default;
    Finder(const ConfigurationSettings& config);

    void FindGamesInWorkingPath();
    void FindInPath(const std::filesystem::path& path);
    void FindGOGPack();
    void LoadPaths(const ConfigurationSettings& config);
    void SafePaths(ConfigurationSettings& config);

    const GameDetection GetGameDetector(uint8_t ID) const;
    const DetectionReport GetGameReport(uint8_t ID) const;
    const std::string GetGameName(uint8_t ID) const;
    uint16_t GetGameScore(uint8_t ID) const;
    const std::filesystem::path GetGameFolder(uint8_t ID) const;

private:
    std::array<GameDetection, GameID::Count> m_detector;

};

