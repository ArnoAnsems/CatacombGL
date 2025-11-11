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
// GameDetectionCatacomb3D
//
// Data structures for detecting the Catacomb 3D game files.
//
#pragma once

#include <stdint.h>
#include <map>
#include <string>

const std::map<std::string, uint32_t> catacomb3Dv100Files =
{
    std::make_pair("AUDIO.C3D",    5062),
    std::make_pair("EGAGRAPH.C3D", 258007),
    std::make_pair("GAMEMAPS.C3D", 14288)
};

const std::map<std::string, uint32_t> catacomb3Dv122Files =
{
    std::make_pair("AUDIO.C3D",    5062),
    std::make_pair("EGAGRAPH.C3D", 256899),
    std::make_pair("GAMEMAPS.C3D", 14288)
};
