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
// GameDetectionApocalypse
//
// Data structures for detecting the Catacomb Apocalypse game files.
//
#pragma once

#include <stdint.h>
#include <map>
#include <string>

const std::map<std::string, uint32_t> apocalypseFiles =
{
    std::make_pair("AUDIO.APC",    7366),
    std::make_pair("EGAGRAPH.APC", 382078),
    std::make_pair("GAMEMAPS.APC", 18855),
    std::make_pair("SHP1.APC",     1376),
    std::make_pair("SHP2.APC",     6329),
    std::make_pair("SHP3.APC",     13187),
    std::make_pair("SHP4.APC",     13592),
    std::make_pair("SHP5.APC",     12698),
    std::make_pair("SHP6.APC",     11131),
    std::make_pair("SHP7.APC",     13266),
    std::make_pair("SHP8.APC",     640),
    std::make_pair("SHP9.APC",     9182),
    std::make_pair("SHP10.APC",    9930),
    std::make_pair("SHP11.APC",    398),
    std::make_pair("SHP12.APC",    417),
    std::make_pair("SHP13.APC",    329),
    std::make_pair("SHP14.APC",    1668)
};
