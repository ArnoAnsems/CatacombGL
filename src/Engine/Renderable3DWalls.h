// Copyright (C) 2020 Arno Ansems
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
//
// Renderable3DWalls
//
// Contains a list of 3D walls that can be processed by the renderer.
//
#pragma once

#include <map>
#include <vector>
#include <cstdint>

class Renderable3DWalls
{
public:
    typedef struct
    {
        uint16_t x1;
        uint16_t y1;
        uint16_t x2;
        uint16_t y2;
    } wallCoordinate;

    Renderable3DWalls();
    void AddWall(const unsigned int textureId, const wallCoordinate coordinate);
    const std::map<unsigned int, std::vector<wallCoordinate>>& GetTextureToWallsMap() const;
    void Reset();

private:
    std::map<unsigned int, std::vector<wallCoordinate>> m_textureToWallsMap;
};
