// Copyright (C) 2018 Arno Ansems
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

#include "SystemSDL.h"
#include "../Engine/Logging.h"
#include <SDL_platform.h>
#include <filesystem>
#include <string>
#include <vector>

#ifdef _WIN32
#include <shlobj.h>
#endif

namespace fs = std::filesystem;

std::string SystemSDL::GetOSVersion() const
{
    return SDL_GetPlatform();
}

bool SystemSDL::CreatePath(const fs::path& path) const
{
    if (path.empty())
    {
        return false;
    }

    fs::create_directories(path);
    return fs::exists(path);
}

const fs::path& SystemSDL::GetConfigurationFilePath() const
{
    return m_configurationFile;
}

void SystemSDL::GetSavedGameNamesFromFolder(
    const fs::path& path,
    std::vector<std::string>& filesFound
) const
{
    filesFound.clear();

    if( path.empty() )
    {
        return;
    }

    if( !fs::exists(path))
    {
        fs::create_directories(path);
    }

    for (const auto & entry : fs::directory_iterator(path))
    {
        if (!entry.is_directory() && entry.path().extension() == ".sav")
        {
            filesFound.push_back(entry.path().stem().string());
        }
    }
}

fs::path SystemSDL::FindConfigurationFile() const
{
    fs::path filenamePath = ""; 

#ifdef _WIN32
    PWSTR appDataPathWideStr;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, nullptr, &appDataPathWideStr);
    char appDataPath[256];
    size_t returnValue;
    wcstombs_s(&returnValue, appDataPath, 256, appDataPathWideStr, 255);

    CoTaskMemFree(static_cast<void*>(appDataPathWideStr));

    filenamePath = fs::path(appDataPath);

#elif defined (__linux__)
    filenamePath = fs::path(getenv("HOME")) / ".config";

#endif

    filenamePath /= "CatacombGL";
    return filenamePath;
}

void SystemSDL::GetSubFolders(
    const fs::path selectedFolder,
    std::vector<fs::path>& subFolders
) const
{
    subFolders.clear();
    fs::path workFolder = selectedFolder;
    
    if (selectedFolder.empty())
    {
#if defined (_WIN32)
        // No search folder. List all the logical drives instead.
        uint32_t logicalDrives = GetLogicalDrives();
        for (uint32_t driveIndex = 0; driveIndex < 26; driveIndex++)
        {
            const uint32_t logicalDriveBit = 1 << driveIndex;
            if (logicalDrives & logicalDriveBit)
            {
                char logicalDriveName[4];
                logicalDriveName[0] = 'A' + driveIndex;
                logicalDriveName[1] = ':';
                logicalDriveName[2] = '\\';
                logicalDriveName[3] = 0;
                subFolders.push_back(logicalDriveName);
            }
        }

        return;

#elif defined (__linux__)
        workFolder = getenv("HOME");

#else
        workfolder = fs::current_path;

#endif
    }

    subFolders.push_back(".."); // Make sure the "return to parent" folder ends up at the top of the list.
    if (fs::is_directory(workFolder)) {
        for (auto const& entry : fs::directory_iterator{workFolder})
        {
            if (entry.is_directory()) {
                subFolders.push_back(entry.path());
            }
        }
    }
}

