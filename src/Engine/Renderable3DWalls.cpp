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

#include "Renderable3DWalls.h"

Renderable3DWalls::Renderable3DWalls()
{
    m_textureToWallsMap.clear();
}

void Renderable3DWalls::AddWall(const unsigned int textureId, const wallCoordinate coordinate)
{
    if (m_textureToWallsMap.find(textureId) == m_textureToWallsMap.end())
    {
        m_textureToWallsMap.insert(std::make_pair(textureId, std::vector<Renderable3DWalls::wallCoordinate>()));
    }
    m_textureToWallsMap.at(textureId).push_back(coordinate);
}

const std::map<unsigned int, std::vector<Renderable3DWalls::wallCoordinate>>& Renderable3DWalls::GetTextureToWallsMap() const
{
    return m_textureToWallsMap;
}

void Renderable3DWalls::Reset()
{
    m_textureToWallsMap.clear();
}