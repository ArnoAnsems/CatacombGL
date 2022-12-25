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

#include "Finder.h"
#include "../Engine/Logging.h"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <filesystem>

#ifdef _WIN32
#include <shlwapi.h>
#include <windows.h>
#endif

namespace fs = std::filesystem;

// These 3 const members have to be in the same order as GameID
const std::array<const std::map<std::string, uint32_t>*, GameID::Count> GameFile
{
    &abyssFilesv113,
    &abyssFilesv124,
    &armageddonFiles,
    &apocalypseFiles,
    &catacomb3DFiles
};

const std::array<const std::string, GameID::Count> GOGFolder
{
    "",
    "Abyss",
    "Armageddon",
    "Apocalypse",
    "Cat3D"
};

const std::array<const uint8_t, GameID::Count> ConfigID
{
    CVarIdPathAbyssv113,
    CVarIdPathAbyssv124,
    CVarIdPathArmageddonv102,
    CVarIdPathApocalypsev101,
    CVarIdPathCatacomb3Dv122
};

const std::array<std::string, GameID::Count> GameName
{
    "Catacomb Abyss v1.13",
    "Catacomb Abyss v1.24",
    "Catacomb Armageddon v1.02",
    "Catacomb Apocalypse v1.01",
    "Catacomb 3-D v1.22"
};

inline bool GetCatacombsPackGOGPath(fs::path& path)
{
#ifdef _WIN32
    CHAR gog_catacombs_path[256];
    memset(gog_catacombs_path, 0, 256);
    DWORD dwType = 0;
    DWORD dwSize = sizeof(gog_catacombs_path);
    // Check GOG Catacombs Pack - Legacy path
    LSTATUS status = SHGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\GOG.com\\GOGCATACOMBSPACK", "PATH", &dwType, gog_catacombs_path, &dwSize);
    bool isGogCatacombsPathFound = ((status == ERROR_SUCCESS) && (dwType == REG_SZ));
    if (isGogCatacombsPathFound)
    {
        path.assign(std::string(gog_catacombs_path));
    }
    else
    {
        // Check GOG Catacombs Pack - Product ID 1207659189
        status = SHGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\GOG.com\\GAMES\\1207659189", "PATH", &dwType, gog_catacombs_path, &dwSize);
        isGogCatacombsPathFound = ((status == ERROR_SUCCESS) && (dwType == REG_SZ));
        if (isGogCatacombsPathFound)
        {
            path.assign(std::string(gog_catacombs_path));
        }
    }

    return isGogCatacombsPathFound;
#else
    return false;
#endif
}

Finder::Finder(const ConfigurationSettings& config)
{
    FindGamesInWorkingPath();
    LoadPaths(config);
    FindGOGPack();
}

void Finder::FindGamesInWorkingPath()
{
    const fs::path path = fs::current_path();
    FindInPath(path);
}

void Finder::FindGOGPack()
{
    fs::path gogPath = fs::current_path();
    if (GetCatacombsPackGOGPath(gogPath))
    {
        Logging::Instance().AddLogMessage("Catacombs Pack is present in Windows registry: " + gogPath.string());
        // Start from 1 as 0 is the shareware game and not included in the GOG Pack
    }

    for ( std::uint8_t i = 1; i < GameID::Count; ++i )
    {
        if( !GOGFolder[i].empty() && GetGameScore(i) != 0 )
        {
            const fs::path path = gogPath / GOGFolder[i];
            m_detector[i].GetDetectionReport(i, path, *GameFile[i] );
        }
    }
}

void Finder::FindInPath(const fs::path& path)
{
    for ( std::uint8_t i = 0; i < GameID::Count; ++i )
    {
        if( GetGameScore(i) != 0 )
        {
            m_detector[i].GetDetectionReport(i, path, *GameFile[i] );
        }
    }
}

void Finder::LoadPaths(const ConfigurationSettings& config)
{
    for ( std::uint8_t i = 0; i < GameID::Count; ++i )
    {
        if( GetGameScore(i) != 0 && !config.GetCVarString(ConfigID[i]).Get().empty()  )
        {
            m_detector[i].GetDetectionReport(i, config.GetCVarString(ConfigID[i]).Get(), *GameFile[i] );
        }
    }
}

void Finder::SafePaths(ConfigurationSettings &config)
{
    // If game data was found, remember the path!
    for ( std::uint8_t i = 0; i < GameID::Count; ++i )
    {
        if (GetGameScore(i) == 0)
        {
            config.GetCVarStringMutable(ConfigID[i]).Set(GetGameFolder(i).string());
        }
    }
}

const GameDetection Finder::GetGameDetector(uint8_t ID) const
{
    return m_detector.at(ID);
}

const DetectionReport Finder::GetGameReport(uint8_t ID) const
{
    return GetGameDetector(ID).GetBestMatch();
}

const std::string Finder::GetGameName(uint8_t ID) const
{
    return GameName.at(ID);
}

uint16_t Finder::GetGameScore(uint8_t ID) const
{
    return GetGameReport(ID).score;
}

const fs::path Finder::GetGameFolder(uint8_t ID) const
{
    return GetGameReport(ID).folder;
}

