// Copyright (C) 2026 Arno Ansems
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
// GameMapsAbyssV112
//
// Data structures required for reading the Catacomb Abyss v1.12 maps repository.
// Compared to Catacomb Abyss v1.13 and v1.24, the EGAGRAPH file of Catacomb Abyss v1.12
// lacks the first picture (FINALEPIC). Therefore, all indexes to pictures need to be lowered by one.
// The conversion of picture indexes in the maps repository data is handled via this class.
//
#pragma once

#include "../Engine/GameMaps.h"

class GameMapsAbyssV112
{
public:
	GameMapsAbyssV112();
	const gameMapsStaticData& Get() const;

private:
	gameMapsStaticData m_gameMapsStaticData;
};