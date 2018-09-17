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

//
// GameDetectionArmageddon
//
// Data structures for detecting the Catacomb Armageddon game files.
//
#pragma once

#include <stdint.h>
#include <map>
#include <string>

const std::map<std::string, uint32_t> armageddonFiles =
{
    std::make_pair("AUDIO.ARM",    7104),
    std::make_pair("EGAGRAPH.ARM", 355832),
    std::make_pair("GAMEMAPS.ARM", 17653),
    std::make_pair("SHP1.ARM",     1376),
    std::make_pair("SHP2.ARM",     3991),
    std::make_pair("SHP3.ARM",     15036),
    std::make_pair("SHP4.ARM",     15568),
    std::make_pair("SHP5.ARM",     13234),
    std::make_pair("SHP6.ARM",     10861),
    std::make_pair("SHP7.ARM",     9414),
    std::make_pair("SHP8.ARM",     749),
    std::make_pair("SHP9.ARM",     9182),
    std::make_pair("SHP10.ARM",    9930),
    std::make_pair("SHP11.ARM",    398),
    std::make_pair("SHP12.ARM",    417),
    std::make_pair("SHP13.ARM",    329),
    std::make_pair("SHP14.ARM",    1688)
};
