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

#include "SystemWin32.h"
#include <shlobj.h>
#include <string>
#include <VersionHelpers.h>

SystemWin32::SystemWin32() :
    m_configurationFilePath(FindConfigurationFilePath())
{

}

SystemWin32::~SystemWin32()
{

}

const std::string& SystemWin32::GetConfigurationFilePath() const
{
    return m_configurationFilePath;
}

std::string SystemWin32::FindConfigurationFilePath() const
{
    PWSTR appDataPathWideStr;
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, nullptr, &appDataPathWideStr);
    char appDataPath[256];
    size_t returnValue;
    wcstombs_s(&returnValue, appDataPath, 256, appDataPathWideStr, 255);

    CoTaskMemFree(static_cast<void*>(appDataPathWideStr));

    const std::string filenamePath = std::string(appDataPath) + "\\CatacombGL\\";
    return filenamePath;
}

void SystemWin32::GetSavedGameNamesFromFolder(const std::string& path, std::vector<std::string>& filesFound) const
{
    filesFound.clear();
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;
    const std::string filesToFind = path + "\\*.sav";
    hFind = FindFirstFile(filesToFind.c_str(), &FindFileData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        bool nextFilePresent = true;
        while (nextFilePresent)
        {
            const std::string filename = std::string(FindFileData.cFileName);
            const std::string savedFileWithoutExtension = filename.substr(0, filename.length() - 4);
            filesFound.push_back(savedFileWithoutExtension);
            nextFilePresent = (FindNextFile(hFind, &FindFileData) != 0);
        }
        FindClose(hFind);
    }
}

bool SystemWin32::CreatePath(const std::string& path) const
{
    return CreateDirectory(path.c_str(), nullptr) != 0 || GetLastError() == ERROR_ALREADY_EXISTS;
}

std::string SystemWin32::GetOSVersion() const
{
    return
        IsWindows8OrGreater() ? "Windows 8 or newer" :
        IsWindows7SP1OrGreater() ? "Windows 7 SP1" :
        IsWindows7OrGreater() ? "Windows 7" :
        IsWindowsVistaSP2OrGreater() ? "Windows Vista SP2" :
        IsWindowsVistaSP1OrGreater() ? "Windows Vista SP1" :
        IsWindowsVistaOrGreater() ? "Windows Vista" :
        IsWindowsXPSP3OrGreater() ? "Windows XP SP3" :
        IsWindowsXPSP2OrGreater() ? "Windows XP SP2" :
        IsWindowsXPSP1OrGreater() ? "Windows XP SP1" :
        IsWindowsXPOrGreater() ? "Windows XP" :
            "Windows 2000 or older";
}

bool SystemWin32::isBuiltIn64Bit() const
{
#ifdef _WIN64
    return true;
#else
    return false;
#endif
}