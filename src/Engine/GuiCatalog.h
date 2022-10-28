// Copyright (C) 2021 Arno Ansems
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

#include "GuiElementBase.h"
#include "Shape.h"
#include <filesystem>

class GuiCatalog : public GuiElementBase
{
public:
    GuiCatalog(
        PlayerInput& playerInput,
        const IRenderer& renderer,
        const std::vector<std::string>& catalogFilenames,
        const std::filesystem::path& gameFolder);
    ~GuiCatalog();
    virtual const GuiEvent& ProcessInput() override;
    virtual void Draw(IRenderer& renderer) const override;

private:
    std::vector<Shape*> m_shapes;
    const std::vector<std::string>& m_catalogFilenames;
    const GuiEvent m_closeEvent = { GuiActionClose, 0 };
    uint16_t m_currentPage;
    uint16_t m_mostRecentPageWithFullscreenImage;
    const IRenderer& m_renderer;
    const std::filesystem::path m_gameFolder;
};
