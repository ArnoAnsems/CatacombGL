// Copyright (C) 2019 Arno Ansems
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

#include "IRenderer.h"
#include <vector>

enum GameDetectionState
{
    Detected,
    NotDetected,
    NotSupported
};

struct GameSelectionPresentation
{
    std::vector<std::pair<const std::string, const GameDetectionState>> gameListCatacombsPack;
    std::vector<std::pair<const std::string, const GameDetectionState>> gameListShareware;
    std::string searchFolder;
    std::vector<std::string> subFolders;
    uint32_t selectedSubFolder;
    uint32_t subFolderOffset;
};

class GameSelection
{
public:
    GameSelection(IRenderer& renderer);
    ~GameSelection();

    void Draw(const GameSelectionPresentation& presentation);

private:
    void DrawBox(const uint16_t x, const uint16_t y, const uint16_t width, const uint16_t height, const char* title);

    IRenderer& m_renderer;
};