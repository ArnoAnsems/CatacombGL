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
// GameDetectionAbyss
//
// Data structures for detecting the Catacomb Abyss game files.
// Currently version 1.13 and v1.24 are supported.
//
#pragma once

#include <stdint.h>
#include <map>
#include <string>

const std::map<std::string, uint32_t> abyssFilesv112 =
{
    std::make_pair("AUDIO.ABS",    6904),
    std::make_pair("BEGIN.ABS",    12742),
    std::make_pair("CREDITS.ABS",  15237),
    std::make_pair("EGAGRAPH.ABS", 320626),
    std::make_pair("FINALE.ABS",   11918),
    std::make_pair("GAMEMAPS.ABS", 20433),
    std::make_pair("SDLOGO.ABS",   1431),
    std::make_pair("TITLE.ABS",    17486)
};

const std::map<std::string, uint32_t> abyssFilesv113 =
{
    std::make_pair("AUDIO.ABS",    6904),
    std::make_pair("EGAGRAPH.ABS", 336143),
    std::make_pair("GAMEMAPS.ABS", 20433),
    std::make_pair("SHP01.ABS",    1376),
    std::make_pair("SHP02.ABS",    15181),
    std::make_pair("SHP03.ABS",    13158),
    std::make_pair("SHP04.ABS",    12648),
    std::make_pair("SHP05.ABS",    724),
    std::make_pair("SHP06.ABS",    9182),
    std::make_pair("SHP07.ABS",    9930),
    std::make_pair("SHP08.ABS",    398),
    std::make_pair("SHP09.ABS",    417),
    std::make_pair("SHP10.ABS",    329),
    std::make_pair("SHP11.ABS",    10595),
    std::make_pair("SHP12.ABS",    9182)
};

const std::map<std::string, uint32_t> abyssFilesv124 =
{
    std::make_pair("AUDIO.ABS",    6904),
    std::make_pair("EGAGRAPH.ABS", 335994),
    std::make_pair("GAMEMAPS.ABS", 20433),
    std::make_pair("SHP01.ABS",    1376),
    std::make_pair("SHP02.ABS",    14912),
    std::make_pair("SHP03.ABS",    13186),
    std::make_pair("SHP04.ABS",    12648),
    std::make_pair("SHP05.ABS",    724),
    std::make_pair("SHP06.ABS",    9182),
    std::make_pair("SHP07.ABS",    9930),
    std::make_pair("SHP08.ABS",    398),
    std::make_pair("SHP09.ABS",    417),
    std::make_pair("SHP10.ABS",    329),
    std::make_pair("SHP11.ABS",    10328),
    std::make_pair("SHP12.ABS",    1652)
};
