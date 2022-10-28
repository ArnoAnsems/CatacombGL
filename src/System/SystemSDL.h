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
#pragma once

#include "../Engine/ISystem.h"
#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>

class SystemSDL: public ISystem
{
public:
    SystemSDL() : m_configurationFile(FindConfigurationFile()) {};
    ~SystemSDL() = default;

    virtual std::string GetOSVersion() const override;
    virtual bool CreatePath(const std::filesystem::path& path) const override;
    virtual const std::filesystem::path& GetConfigurationFilePath() const override;
    virtual void GetSavedGameNamesFromFolder(
        const std::filesystem::path& path,
        std::vector<std::string>& filesFound
    ) const override;

    virtual bool isBuiltIn64Bit() const override {
        return sizeof(std::size_t) == 8;
    }

    void GetSubFolders(
        const std::filesystem::path selectedFolder,
        std::vector<std::filesystem::path>& subFolders
    ) const;

protected:
    std::filesystem::path FindConfigurationFile() const;

private:
    const std::filesystem::path m_configurationFile;
};

