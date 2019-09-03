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

//
// ViewPorts
//
#pragma once

#include <stdint.h>

class ViewPorts
{
public:
    typedef struct
    {
        double left;
        double right;
        double top;
        double bottom;
    } ViewPortRect2D;

    typedef struct
    {
        uint16_t left;
        uint16_t bottom;
        uint16_t width;
        uint16_t height;
    } ViewPortRect3D;

    static ViewPortRect2D GetOrtho2D(const uint16_t windowWidth, const uint16_t windowHeight, const bool helpWindow);
    static ViewPortRect3D Get3D(const uint16_t windowWidth, const uint16_t windowHeight, const float aspectRatio);
};
