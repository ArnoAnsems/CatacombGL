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
#elif __linux__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif

namespace fs = std::filesystem;

const std::map<GameId, const std::map<std::string, uint32_t>*> GameFiles
{
    {GameId::CatacombAbyssv113, &abyssFilesv113},
    {GameId::CatacombAbyssv124, &abyssFilesv124},
    {GameId::CatacombArmageddonv102, &armageddonFiles},
    {GameId::CatacombApocalypsev101, &apocalypseFiles},
    {GameId::Catacomb3Dv100, &catacomb3Dv100Files},
    {GameId::Catacomb3Dv122, &catacomb3Dv122Files}
};

const std::map<GameId, const std::string> GOGFolders
{
    {GameId::CatacombAbyssv124, "Abyss"},
    {GameId::CatacombArmageddonv102, "Armageddon"},
    {GameId::CatacombApocalypsev101, "Apocalypse"},
    {GameId::Catacomb3Dv122, "Cat3D"}
};

const std::map<GameId, uint8_t> ConfigIds
{
    {GameId::CatacombAbyssv113, CVarIdPathAbyssv113},
    {GameId::CatacombAbyssv124, CVarIdPathAbyssv124},
    {GameId::CatacombArmageddonv102, CVarIdPathArmageddonv102},
    {GameId::CatacombApocalypsev101, CVarIdPathApocalypsev101},
    {GameId::Catacomb3Dv100, CVarIdPathCatacomb3Dv100},
    {GameId::Catacomb3Dv122, CVarIdPathCatacomb3Dv122}
};

const std::map<GameId, std::string> GameNames
{
    {GameId::CatacombAbyssv113, "Catacomb Abyss v1.13"},
    {GameId::CatacombAbyssv124, "Catacomb Abyss v1.24"},
    {GameId::CatacombArmageddonv102, "Catacomb Armageddon v1.02"},
    {GameId::CatacombApocalypsev101, "Catacomb Apocalypse v1.01"},
    {GameId::Catacomb3Dv100, "Catacomb 3-D v1.00"},
    {GameId::Catacomb3Dv122, "Catacomb 3-D v1.22"}
};

inline bool GetCatacombsPackGOGPath(fs::path& path)
{
#ifdef _WIN32
    constexpr size_t maxPathLength = 256;
    CHAR gog_catacombs_path[maxPathLength];
    memset(gog_catacombs_path, 0, maxPathLength);
    DWORD dwType = 0;
    DWORD dwSize = sizeof(gog_catacombs_path);
    // Check GOG Catacombs Pack - Legacy path
    LSTATUS status = SHGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\GOG.com\\GOGCATACOMBSPACK", "PATH", &dwType, gog_catacombs_path, &dwSize);
    bool isGogCatacombsPathFound = ((status == ERROR_SUCCESS) && (dwType == REG_SZ));
    if (!isGogCatacombsPathFound)
    {
        // Check GOG Catacombs Pack - Product ID 1207659189
        status = SHGetValueA(HKEY_LOCAL_MACHINE, "SOFTWARE\\WOW6432Node\\GOG.com\\GAMES\\1207659189", "PATH", &dwType, gog_catacombs_path, &dwSize);
        isGogCatacombsPathFound = ((status == ERROR_SUCCESS) && (dwType == REG_SZ));
    }

    if (isGogCatacombsPathFound)
    {
        gog_catacombs_path[maxPathLength - 1] = '\0';
        path.assign(std::string(gog_catacombs_path));
        Logging::Instance().AddLogMessage("Catacombs Pack is present in Windows registry: " + std::string(gog_catacombs_path));
    }

    return isGogCatacombsPathFound;
#elif __linux__
    // When the GOG Catacombs Pack is installed on Linux via Wine and the default target location is not modified;
    // it will end up in a specific path.
    const char* homePath = getenv("HOME");
    if (homePath == nullptr)
    {
        homePath = getpwuid(getuid())->pw_dir;
    }
    const std::string defaultWineLocationForCatacombsPack = "/.wine/dosdevices/c:/GOG Games/Catacombs Pack";
    const std::string fullCatacombsPackPath = std::string(homePath) + defaultWineLocationForCatacombsPack;
    const bool isGogCatacombsPathFound = std::filesystem::exists(fullCatacombsPackPath);

    if (isGogCatacombsPathFound)
    {
        path.assign(fullCatacombsPackPath);
        Logging::Instance().AddLogMessage("Catacombs Pack is present in default Wine target location");
    }

    return isGogCatacombsPathFound;
#else
    return false;
#endif
}

Finder::Finder(const ConfigurationSettings& config)
{
    for (GameId gameId : getAllGameIds())
    {
        GameDetection gameDetection;
        m_detector.insert(std::make_pair(gameId, gameDetection));
    }

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
    if (!GetCatacombsPackGOGPath(gogPath))
    {
        Logging::Instance().AddLogMessage("Path to GOG Catacombs Pack not found");
    }

    for ( const auto& folderIt : GOGFolders )
    {
        const GameId gameId = folderIt.first;
        const std::string gogFolder = folderIt.second;
        if( !gogFolder.empty() && GetGameScore(gameId) != 0 )
        {
            const fs::path path = gogPath / gogFolder;
            m_detector.at(gameId).GetDetectionReport(gameId, path, *GameFiles.at(gameId));
        }
    }
}

void Finder::FindInPath(const fs::path& path)
{
    for ( GameId gameId : getAllGameIds())
    {
        if( GetGameScore(gameId) != 0 )
        {
            m_detector.at(gameId).GetDetectionReport(gameId, path, *GameFiles.at(gameId) );
        }
    }
}

void Finder::LoadPaths(const ConfigurationSettings& config)
{
    for (GameId gameId : getAllGameIds())
    {
        if( GetGameScore(gameId) != 0 && !config.GetCVarString(ConfigIds.at(gameId)).Get().empty()  )
        {
            m_detector.at(gameId).GetDetectionReport(gameId, config.GetCVarString(ConfigIds.at(gameId)).Get(), *GameFiles.at(gameId) );
        }
    }
}

void Finder::SafePaths(ConfigurationSettings &config)
{
    // If game data was found, remember the path!
    for (GameId gameId : getAllGameIds())
    {
        if (GetGameScore(gameId) == 0)
        {
            config.GetCVarStringMutable(ConfigIds.at(gameId)).Set(GetGameFolder(gameId).string());
        }
    }
}

const GameDetection Finder::GetGameDetector(GameId ID) const
{
    return m_detector.at(ID);
}

const DetectionReport Finder::GetGameReport(GameId ID) const
{
    return GetGameDetector(ID).GetBestMatch();
}

const std::string Finder::GetGameName(GameId ID) const
{
    return GameNames.at(ID);
}

uint16_t Finder::GetGameScore(GameId ID) const
{
    return GetGameReport(ID).score;
}

const fs::path Finder::GetGameFolder(GameId ID) const
{
    return GetGameReport(ID).folder;
}

