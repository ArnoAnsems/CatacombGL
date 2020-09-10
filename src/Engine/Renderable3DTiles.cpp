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

#include "Renderable3DTiles.h"

Renderable3DTiles::Renderable3DTiles() :
    m_floorColor(EgaBlack),
    m_ceilingColor(EgaBlack),
    m_onlyFloor(false)
{
    m_tileCoordinates.clear();
}

void Renderable3DTiles::AddTile(const tileCoordinate coordinate)
{
    m_tileCoordinates.push_back(coordinate);
}

egaColor Renderable3DTiles::GetFloorColor() const
{
    return m_floorColor;
}

egaColor Renderable3DTiles::GetCeilingColor() const
{
    return m_ceilingColor;
}

void Renderable3DTiles::SetFloorColor(const egaColor color)
{
    m_floorColor = color;
}

void Renderable3DTiles::SetCeilingColor(const egaColor color)
{
    m_ceilingColor = color;
}

bool Renderable3DTiles::IsOnlyFloor() const
{
    return m_onlyFloor;
}

void Renderable3DTiles::SetOnlyFloor(const bool isOnlyFloor)
{
    m_onlyFloor = isOnlyFloor;
}

const std::vector<Renderable3DTiles::tileCoordinate>& Renderable3DTiles::GetTileCoordinates() const
{
    return m_tileCoordinates;
}

void Renderable3DTiles::Reset()
{
    m_tileCoordinates.clear();
}