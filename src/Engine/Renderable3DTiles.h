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
// Renderable3DTiles
//
// Contains a list of 3D tiles that can be processed by the renderer.
// Used for the floor and ceiling in the 3D view.
//
#pragma once

#include <vector>
#include "EgaColor.h"

class Renderable3DTiles
{
public:
    typedef struct
    {
        int16_t x;
        int16_t y;
    } tileCoordinate;

    Renderable3DTiles();
    void AddTile(const tileCoordinate coordinate);
    egaColor GetFloorColor() const;
    egaColor GetCeilingColor() const;
    void SetFloorColor(const egaColor color);
    void SetCeilingColor(const egaColor color);
    bool IsOnlyFloor() const;
    void SetOnlyFloor(const bool isOnlyFloor);
    const std::vector<tileCoordinate>& GetTileCoordinates() const;
    void Reset();

private:
    egaColor m_floorColor;
    egaColor m_ceilingColor;
    bool m_onlyFloor;
    std::vector<tileCoordinate> m_tileCoordinates;
};


