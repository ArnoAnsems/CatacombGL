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
    SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_DEFAULT, NULL, &appDataPathWideStr);
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
    return CreateDirectory(path.c_str(), NULL) != 0 || GetLastError() == ERROR_ALREADY_EXISTS;
}

std::string SystemWin32::GetOSVersion() const
{
    // Based on winbits.cpp at https://github.com/nukeykt/NBlood
    OSVERSIONINFOEX osv;
    memset(&osv, 0, sizeof(osv));
    osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    std::string ver = "Windows";

    if (GetVersionEx((LPOSVERSIONINFOA)&osv))
    {
        switch (osv.dwPlatformId)
        {
        case VER_PLATFORM_WIN32_WINDOWS:
            if (osv.dwMinorVersion < 10)
                ver = "95";
            else if (osv.dwMinorVersion < 90)
                ver = "98";
            else
                ver = "ME";
            break;

        case VER_PLATFORM_WIN32_NT:
            switch (osv.dwMajorVersion)
            {
            case 5:
                switch (osv.dwMinorVersion)
                {
                case 0: ver = "2000"; break;
                case 1: ver = "XP"; break;
                case 2: ver = osv.wProductType == VER_NT_WORKSTATION ? "XP x64" : "Server 2003"; break;
                }
                break;

            case 6:
                switch (osv.dwMinorVersion)
                {
                case 0: ver = osv.wProductType == VER_NT_WORKSTATION ? "Vista" : "Server 2008"; break;
                case 1: ver = osv.wProductType == VER_NT_WORKSTATION ? "7" : "Server 2008 R2"; break;
                case 2: ver = osv.wProductType == VER_NT_WORKSTATION ? "8" : "Server 2012"; break;
                case 3: ver = osv.wProductType == VER_NT_WORKSTATION ? "8.1" : "Server 2012 R2"; break;
                }
                break;

            case 10:
                switch (osv.dwMinorVersion)
                {
                case 0: ver = osv.wProductType == VER_NT_WORKSTATION ? "10" : "Server 2016"; break;
                }
                break;
            }
            break;
        }

        ver = "Windows " + ver;

        // service packs
        if (osv.szCSDVersion[0])
        {
            ver = ver + " " + osv.szCSDVersion;
        }

        ver = ver + " (build " + std::to_string(osv.dwMajorVersion) + "." +
            std::to_string(osv.dwMinorVersion) + "." + std::to_string(osv.dwBuildNumber) + ")";
    }

    return ver;
}